#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <poll.h>
#include <netdb.h>
#include <fcntl.h>

// Test code that works for Only one server socket

void exit_server(int sig)
{
	(void)sig;
	std::cout << "Closing Server..." << std::endl;
	exit(1);
}

int main(void)
{
	int fd_count = 3;
	int yes = 1;
	int listen_fd;
	sockaddr_in address;

	signal(SIGINT, exit_server);
	signal(SIGQUIT, exit_server);

	bzero(&address, sizeof(address));
	address.sin_family = AF_INET; //IPv4
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	// Makes the port reusable if a socket lingers in the Kernel
	// Gets rid of the "Address already in use" message
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	bind(listen_fd, (struct sockaddr *)&address, sizeof(address));

	listen(listen_fd, 10);
	
	// Sets socket to non-blocking
	//fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	
	while(1)
    {
		addrinfo info;

		int new_socket;
		//long valread;
		const char *request = "HTTP/1.1 200 OK\nContent-Type: html\nContent-Length: 1084\n\n\
		<!DOCTYPE html>\n\
		<html>\n\
		<head>\n\
		<!-- HTML Codes by Quackit.com -->\n\
		<title>\n\
		Les surfeurs du web</title>\n\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\
		<style>\n\
		body {background-color:#f55fff;background-image:url(https://c4.wallpaperflare.com/wallpaper/500/442/354/outrun-vaporwave-hd-wallpaper-preview.jpg);background-repeat:repeat;background-position:top left;background-attachment:scroll;}\n\
		h1{font-family:Impact, sans-serif;color:#000000;background-color:#ffffff;}\n\
		p {font-family:Georgia, serif;font-size:14px;font-style:normal;font-weight:normal;color:#000000;background-color:#ffffff;}\n\
		</style>\n\
		</head>\n\
		<body>\n\
		<h1>Yo men bienvenue sur notre site trop swag!</h1>\n\
		<p>Random text bla bla bla</p>\n\
		</body>\n\
		</html>";
		int addrlen = sizeof(address);
        char buffer[30000] = {0};
       
	    printf("\n+++++++ Waiting for new connection ++++++++\n\n");

		// int fd_count = 3;
		// if (poll(pfd, fd_count, -1) < 0)
		// {
		// 	std::cout << "Poll is sad!" << std::endl;
		// }

		// for (int i = 0 ; i < fd_count; i++) {

		// 	if (pfd[i].revents & POLLIN){

		// 	}
		// }
	    if ((new_socket = accept(listen_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))< 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        } else {
			std::cout << "Connection Accepted on Socket: " << new_socket << std::endl;
		}
        
       // valread = read( new_socket , buffer, 30000);
       // printf("%s\n",buffer );
       
		write(new_socket , request , strlen(request));
      //  printf("------------------Hello message sent-------------------\n");
       
	    close(new_socket);
    }
}
