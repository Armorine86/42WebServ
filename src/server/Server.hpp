#pragma once

#include "defines.hpp"
#include "Socket.hpp"
#include "RequestParser.hpp"
#include <unistd.h>
#include <fstream>
#include <poll.h>

#define MAX_BODY_SIZE 1024

// **Server class
//
// **This is where the infinite loop occurs, with the poll() function to
// manage multiple socket fd.

class Sockets;

class Server {
public:
	Server() {}
	Server(server_info serv_info);
	~Server();
	
	void run(Sockets socket);
	
private:
	int		client_fd;

	//Sockets _socket;
	server_info config;
	RequestParser request;
	sockaddr_storage client_addr;
	char buffer[MAX_BODY_SIZE];
	// int poll(struct pollfd fds[], nfds_t nfds, int timeout);
	std::vector<pollfd> pfds;  //fd vectors to use with poll()
};
