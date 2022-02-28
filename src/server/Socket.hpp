#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>

#include "defines.hpp"
#include "config_fields.hpp"

#define BACKLOG 10

// Builds Sockets based on the informations collected by the ConfigParser class
// This class lives within the Server Class
class Sockets {
public:
	Sockets();
	Sockets(const Sockets& src) { *this = src; }
	~Sockets() {}

	void init_sockaddr();
	int getServFD(); // returns socket_fd

private:
	int socket_fd;
	//int addrlen;
	struct sockaddr_in address;
	server_info serv_info;  // Contain server infos from Config file
};
