#pragma once

#include "defines.hpp"
#include "Socket.hpp"
#include <poll.h>

// **Server class
//
// **This is where the infinite loop occurs, with the poll() function to
// manage multiple socket fd.
class Server {
public:
	Server() {}
	~Server() {}
	
	void run();
	
private:
	Sockets socket;
	server_info config;

	// int poll(struct pollfd fds[], nfds_t nfds, int timeout);
	std::vector<pollfd> pfds;  //fd vectors to use with poll()
};
