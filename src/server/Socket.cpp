#include "Socket.hpp"

#include <cerrno>
#include <unistd.h>
#include <poll.h>

// Creates a socket for the server with the infos gathered in the parser.
// Makes the sockets Non-Blocking with fcntl(), and binds it to the port.
//
// If an error occurs, the program exits with appropriate code.
Sockets::Sockets()
{
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		
		std::cerr << logEvent("[SOCKET] Invalid fd: ")
				  << strerror(errno) << END << std::endl;
		exit(INV_SOCKET);
	}
	
	// Sets socket to non-blocking
	//fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	
	int yes = 1;
	
	// Makes the port reusable if a socket lingers in the Kernel
	// Gets rid of the "Address already in use" message
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {

		std::cerr << logEvent("[SOCKET] Could not set Options: ")
				  << strerror(errno) << END << std::endl;
		exit(OPT_ERR);
	}
	
	init_sockaddr();
	
	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		
		std::cerr << logEvent("[SOCKET] Binding Error: ")
				  << strerror(errno) << END << std::endl;
		exit(BINDING_ERR);
	}

	if (listen(socket_fd, BACKLOG) < 0) {
		
		std::cerr << logEvent("[SOCKET] Could not listen on port: ")
				  << strerror(errno) << END << std::endl;
		exit(LISTEN_ERR);
	}
}

// Inits sockaddr_in struct. Struct has been bzero()ed before hand
//
// sin_family = AF_INET (IPv4)
// sin_port = htons(listen_port) 127.0.0.1 | Localhost
// sin_addr.s_addr = inet_addr(host)
//
// according to beej's tutorial, sin_zero doesn't need to be touched at all as it changes nothing
void Sockets::init_sockaddr()
{
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET; //IPv4
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
}

int Sockets::getServFD()
{
	return socket_fd;
}
