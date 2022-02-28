#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <poll.h>
#include <netdb.h>

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
	pollfd pfd[fd_count];

	signal(SIGINT, exit_server);
	signal(SIGQUIT, exit_server);

	bzero(&address, sizeof(address));
	address.sin_family = AF_INET; //IPv4
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	for (int i = 0; i < fd_count; i++)
	{
		listen_fd = socket(AF_INET, SOCK_STREAM, 0);

		// Makes the port reusable if a socket lingers in the Kernel
		// Gets rid of the "Address already in use" message
		setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

		bind(listen_fd, (struct sockaddr *)&address, sizeof(address));
	}

	listen(pfd[3].fd, 10);

	pfd[0].events = POLLIN;
	pfd[1].events = POLLIN;
	pfd[2].events = POLLIN;
	
	// Sets socket to non-blocking
	//fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	
	while(1)
    {
		addrinfo info;

		int new_socket;
		long valread;
		const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		int addrlen = sizeof(address);
        char buffer[30000] = {0};
       
	    printf("\n+++++++ Waiting for new connection ++++++++\n\n");

		int fd_count = 3;
		if (poll(pfd, fd_count, -1) < 0)
		{
			std::cout << "Poll is sad!" << std::endl;
		}

		for (int i = 0 ; i < fd_count; i++) {

			if (pfd[i].revents & POLLIN){

			}
		}
	    if ((new_socket = accept(socket_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))< 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
       
	    write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
       
	    close(new_socket);
    }
}
