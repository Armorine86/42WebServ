#ifndef __SERVER_H__
#define __SERVER_H__

#include "defines.hpp"
#include "Socket.hpp"
#include "RequestParser.hpp"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <poll.h>

#define RECV_BUFSIZE 4096

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

	friend class 	Response;
	int				client_fd;
	int				sender_fd;
	int				bytes;
	std::string		bin_boundary;
	std::string 	str_buffer;
	std::string 	upload_path;
	std::string		status_code;	
	SocketsVector 	sockets;
	std::vector<pollfd> pfds;  //pollfd struct vector
	RequestParser 	request;
	sockaddr_storage client_addr;
	bool			isChunked;
	bool			tropBeaucoup;

	typedef std::vector<pollfd>::iterator PollIterator;

	void handleEvents(PollIterator& it, size_t i);
	void handleClient(PollIterator& it);
	void sendResponse(std::string& str_buffer, int sender_fd, char* buf);

	bool checkBufferSize(const char* buffer);
	void closeSocket(PollIterator& it);

	pollfd addToPollfd(int newfd);

};

#endif // __SERVER_H__
