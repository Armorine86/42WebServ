#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <poll.h>
#include <sstream>

#include "defines.hpp"
#include "config_fields.hpp"

#define BACKLOG 10

// Builds Sockets based on the informations collected by the ConfigParser class
// This class lives within the Server Class
class Sockets {
public:
	Sockets();
	Sockets(server_info& serv_info);
	Sockets(const Sockets& src) { *this = src; }
	~Sockets() {}

	void init_sockaddr(const server_info& serv_info);
	int getServFD() { return server_fd; }
	std::string getHostName(); //return IP, and port#
	sockaddr_in& getAddress() { return address; }

private:
	int server_fd;

	struct sockaddr_in address;
	server_info serv_info;  // Contain server infos from Config file
};
