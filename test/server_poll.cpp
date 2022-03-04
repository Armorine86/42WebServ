/*
** pollserver.c -- a cheezy multiperson chat server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../includes/colors.hpp"

#define PORT 8080   // Port we're listening on

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void)
{
    int listener;     // Listening socket descriptor
    int yes=1;        // For setsockopt() SO_REUSEADDR, below
    int rv;

    // struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    // memset(&hints, 0, sizeof hints);
    // hints.ai_family = AF_UNSPEC;
    // hints.ai_socktype = SOCK_STREAM;
    // hints.ai_flags = AI_PASSIVE;
    // if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
    //     fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
    //     exit(1);
    // }

    struct sockaddr_in sa;
    bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET; //IPv4
	sa.sin_port = htons(PORT);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    for(;;) {
        listener = socket(AF_INET, SOCK_STREAM, 0);
        if (listener < 0) { 
            continue;
        }
        
        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        fcntl(listener, F_SETFL, O_NONBLOCK);

        if (bind(listener, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
            std::cout << "Can't bind" << std::endl;
            close(listener);
            continue;
        }
        break;
    }

    //freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
   // if (p == NULL) {
   //    return -1;
   // }

    // Listen
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *pfds = (pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}

// Remove an index from the set
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    // Copy the one from the end over this one
    pfds[i] = pfds[*fd_count-1];

    (*fd_count)--;
}

// Main
int main(void)
{
    int listener;     // Listening socket descriptor

    int newfd;        // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;


    char buf[1024];    // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_count = 0;
    int fd_size = 5;
    struct pollfd *pfds = (pollfd *)malloc(sizeof *pfds * fd_size);

	// Content Length must be exactly the the number of bytes in the body. Otherwise 
	const char *response = "HTTP/1.1 200 OK\nContent-Type: text/html \nContent-Length: 700\n\n\
		<!DOCTYPE html>\n\
		<html>\n\
		<head>\n\
		<!-- HTML Codes by Quackit.com -->\n\
		<title>\n\
		Les surfeurs du web</title>\n\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\
		<style>\n\
		body {background-color:#f55fff;background-image:url(/Surfer_Girl.jpg);background-repeat:repeat;background-position:top left;background-attachment:scroll;}\n\
		h1{font-family:Impact, sans-serif;color:#000000;background-color:#ffffff;}\n\
		p {font-family:Georgia, serif;font-size:14px;font-style:normal;font-weight:normal;color:#000000;background-color:#ffffff;}\n\
		</style>\n\
		</head>\n\
		<body>\n\
		<h1>Yo men bienvenue sur notre site trop swag!</h1>\n\
		<p>Random text bla bla bla</p>\n\
		<img src=\"Surfer_Girl.jpg\" alt=\"\">\n\
		</body>\n\
		</html>";
    
    //const char* img_buf = copy_file("test/Surfer_Girl.jpg");

    // std::ifstream fin("test/Surfer_Girl.jpg", std::ios::binary);
    // std::ostringstream oss;
    // oss << fin.rdbuf();
    //std::string data(oss.str());
    //size_t len = data.length();
    //std::string img_response = "HTTP/1.1 200 OK\nContent-Type: image/jpeg \nContent-Length: 409059\n\n" + data;

    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1) {
        std::cerr << "error getting listening socket" << std::endl;
        exit(1);
    }

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

    fd_count = 1; // For the listener

    // Main loop
    for(;;) {
		//std::cout << "+++++++++++ Waiting for New Connection +++++++++++\n" << std::endl;
        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for(int i = 0; i < fd_count; i++) {

            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN) { // We got one!!

                if (pfds[i].fd == listener) {
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof remoteaddr;

                    // Return a FD for the client
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);

                        std::cout   << "pollserver: new connection from "
                                    <<  inet_ntop(remoteaddr.ss_family,
                                        get_in_addr((struct sockaddr*)&remoteaddr),
                                        remoteIP, INET6_ADDRSTRLEN)
                                    << "on socket " << newfd << std::endl;
                    }
                } 
                else {
                    // If not the listener, we're just a regular client
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);
       				std::cout << GREEN << "+++ REQUEST +++ \n\n" << END << buf << std::endl;
                    int sender_fd = pfds[i].fd;

                    if (nbytes <= 0) {
                        // Got error or connection closed by client
                        if (nbytes == 0) {
                            // Connection closed
                            std::cout << "pollserver: socket " << sender_fd << " hung up" << std::endl;
                        } else {
                            perror("recv");
                        }

                        close(pfds[i].fd); // Bye!

                        del_from_pfds(pfds, i, &fd_count);

                    } else {
                        // We got some good data from a client
						if (pfds[i].fd == sender_fd){
                            std::string request = buf;
                            if (request.find("/Surfer_Girl") != std::string::npos){

                                //Build the header and image for the Response in two send
                                std::string headers = "HTTP/1.0 200 OK\r\nContent-type: image/jpeg \r\nContent-Length: 409059\r\n\r\n";
                                send (newfd, headers.data(), headers.length(), 0);

                                std::ifstream f("Surfer_Girl.jpg", std::ios::in|std::ios::binary|std::ios::ate);
                                if(!f.is_open()) perror ("bloody file is nowhere to be found. Call the cops");
                                std::streampos size = f.tellg();
                                char* image = new char [size];
                                f.seekg (0, std::ios::beg);
                                f.read (image, size);
                                f.close();

                                send (newfd, image, size, MSG_CONFIRM);
                                std::cout << GREEN << "+++ IMAGE RESPONSE +++\n\n" << END << headers << image << std::endl;

                            } else if (request.find("/favicon.ico") != std::string::npos) {

                                std::string headers = "HTTP/1.0 200 OK\r\nContent-type: image/jpeg \r\nContent-Length: 67646\r\n\r\n";
                                send (newfd, headers.data(), headers.length(), 0);

                                std::ifstream f("favicon.ico", std::ios::in|std::ios::binary|std::ios::ate);
                                if(!f.is_open()) perror ("bloody file is nowhere to be found. Call the cops");
                                std::streampos size = f.tellg();
                                char* image = new char [size];
                                f.seekg (0, std::ios::beg);
                                f.read (image, size);
                                f.close();

                                send (newfd, image, size, MSG_CONFIRM);
                                
                                std::cout << GREEN << "+++ FAVICON RESPONSE +++\n\n" << END << headers << image << std::endl;
                                } else {
        
                                std::cout << GREEN << "+++ RESPONSE +++\n\n" << END << response << std::endl;
                            	send(newfd, response, strlen(response), 0);
                            }
                        }

                    }
                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
    return 0;
}
