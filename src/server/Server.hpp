#pragma once

#include "defines.hpp"
#include "Socket.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
#include <unistd.h>
#include <fstream>
#include <poll.h>

#define MAX_BODY_SIZE 200000

class Sockets;

// Server class
//
// This is where the infinite loop occurs
class Server {
public:
	Server() {}
	Server(server_info serv_info);
	~Server() {}
	
	void run(Sockets socket);
	
private:

	typedef std::vector<pollfd>::iterator PollIterator;

	int		client_fd;
	
	server_info config;
	RequestParser request;
	sockaddr_storage client_addr;

	char buffer[MAX_BODY_SIZE];
	std::vector<pollfd> pfds;  //pollfd struct vector

	void handleEvents(PollIterator& it);
	void handleClient(PollIterator& it);
	void sendResponse(std::string str_buffer, int sender_fd);

	bool checkBufferSize(const char* buffer);
	pollfd addToPollfd(int newfd);

};
