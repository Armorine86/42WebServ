#pragma once

#include "defines.hpp"
#include "Socket.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
#include <unistd.h>
#include <fstream>
#include <poll.h>

#define MAX_BODY_SIZE 200000 // Don't make this too big

class Sockets;

// Server class
//
// This is where the infinite loop occurs
class Server {
public:
	Server() {}
	Server(SocketsVector sockvector);
	~Server() {}
	
	void run(SocketsVector sockets);
	
private:

	typedef std::vector<pollfd>::iterator PollIterator;

	int		client_fd;
	std::map<size_t, size_t> server_index; //client_index : server_index

	SocketsVector sockets;
	RequestParser request;
	sockaddr_storage client_addr;

	char buffer[MAX_BODY_SIZE];
	std::vector<pollfd> pfds;  //pollfd struct vector

	void handleEvents(PollIterator& it, size_t i);
	void handleClient(PollIterator& it, server_info serv_info);
	void sendResponse(std::string str_buffer, int sender_fd, server_info serv_info);

	bool checkBufferSize(const char* buffer);
	pollfd addToPollfd(int newfd);

};
