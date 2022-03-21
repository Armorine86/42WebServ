#ifndef __SERVER_H__
#define __SERVER_H__

#include "defines.hpp"
#include "Socket.hpp"
#include "RequestParser.hpp"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <poll.h>

class Sockets;
class Reponse;
// Server class
//
// This is where the infinite loop occurs
class Server {
public:

	Server() {}
	Server(SocketsVector sockvector);
	~Server() {}
	
	void run();

protected:

	friend class Response;
	int		client_fd;
	std::string	status_code;		
	SocketsVector sockets;
	ServerIndex server_index; // client_fd : server_index
	std::vector<pollfd> pfds;  //pollfd struct vector
	RequestParser request;
	sockaddr_storage client_addr;

	typedef std::vector<pollfd>::iterator PollIterator;

	void handleEvents(PollIterator& it, size_t i);
	void handleClient(PollIterator& it, server_info serv_info);
	void sendResponse(std::string str_buffer, int sender_fd, server_info serv_info);

	bool checkBufferSize(const char* buffer);

	pollfd addToPollfd(int newfd);

};

#endif // __SERVER_H__
