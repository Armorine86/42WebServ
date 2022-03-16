#include "Server.hpp"

// Builds the pollfd vector with the server sockets and runs the servers
Server::Server(SocketsVector sockvector) : client_fd(0), status_code(200), sockets(sockvector)
{
	for (size_t i = 0; i < sockets.size(); i++){
		pollfd newfd = addToPollfd(sockets.at(i).getServFD());
		pfds.push_back(newfd);
		pfds[i].events = POLLIN;
	}

	run(sockvector);
}

// Creates a new Pollfd, adds the new Client fd
// and sets events to POLLIN
pollfd Server::addToPollfd(int newfd)
{
	pollfd new_pfd;

	new_pfd.fd = newfd;
	new_pfd.events = POLLIN;
	new_pfd.revents = 0;

	return new_pfd;
}

// Retrieves the Client IP for the logger
std::string clientIP(const int& client_fd, socklen_t addrlen) {

	char myIP[16];
	struct sockaddr_in client_ip;

	getsockname(client_fd, (struct sockaddr *)&client_ip, &addrlen);
	inet_ntop(AF_INET, &client_ip.sin_addr, myIP, sizeof(myIP));
	std::string ip = myIP;
	
	return ip;
}

// Handle Events sent to the server socket. Accepts a connection
// and receives a client FD.
//
// The FD is then pushed_back to the Pollfd struct vector.
// A map keeps track of the client fd and which server got the client request
void Server::handleEvents(PollIterator& it, size_t i)
{
	socklen_t addrlen = sizeof(client_addr);

	if ((client_fd = accept(pfds[i].fd, (struct sockaddr*)&client_addr, &addrlen)) == -1)
		perror("accept");
	else {
		pfds.push_back(addToPollfd(client_fd));
		it = pfds.begin();

		while (it->fd != pfds[i].fd)
			it++;

		std::pair<int, size_t> p1(client_fd, i);
		server_index[p1.first] = p1.second;

		std::cout << YELLOW << logEvent("Accepted Connection from: " + clientIP(client_fd, addrlen) + "\n") << END << std::endl;
	}
}

// Checks if buffer size exceeded permitted max body size 
bool Server::checkBufferSize(const char* buffer)
{
	for (size_t i = 0; buffer[i]; ++i) {
		if (i > MAX_BODY_SIZE)
			return true;
	}
	return false;
}

// Handles Client requests. If the FD in the pollfd vector is
// the sender FD (client), we can send the response to the same FD
void Server::handleClient(PollIterator& it, server_info serv_info)
{
	int bytes = recv((*it).fd, buffer, sizeof(buffer), 0);
	
	if (checkBufferSize(buffer))
		status_code = 413;

	if (DEBUG) {
		std::string str_buffer = buffer;
		std::cout	<< TEAL << "\n+++ REQUEST HEADER +++\n\n"
					<< END << YELLOW << "client fd: " << (*it).fd
					<< END << "\n"
					<< TEAL << str_buffer << END << std::endl;
	}

	int sender_fd = (*it).fd;

	if (bytes <= 0)
	{
		if (bytes == 0)
			std::cout << "pollserver: socket " << sender_fd << " hung up" << std::endl; 
		else 
			perror("recv");
		close((*it).fd); // Bye !
		pfds.erase(it);
		it = pfds.begin();
	}
	else if ((*it).fd == sender_fd){
		std::string str_buffer(buffer);

		bzero(buffer, sizeof(buffer));
		sendResponse(str_buffer, sender_fd, serv_info);
	}
}

// Parse the request sent by the client and builds a response.
// Memcpy the Header and the Body into a buffer to use with send().
void Server::sendResponse(std::string str_buffer, int sender_fd, server_info serv_info)
{
	RequestParser request(str_buffer);
	Response response(request, serv_info, status_code);
	
	std::string header = response.getResponseHeader();
	std::string body = response.getResponseBody();
	size_t MAX_SEND = response.getBodySize() + response.getHeaderSize() + 1;

	char * buffer = new char[MAX_SEND];
	bzero(buffer, MAX_SEND);

	memcpy(buffer, header.data(), header.length());
	memcpy(buffer + header.length(), body.data(), body.length());
	
	send(sender_fd, buffer, MAX_SEND, 0);
	if (DEBUG)
		std::cout << GREEN << "+++ RESPONSE +++\n\n" << END << buffer << std::endl;

	delete [] buffer;
	status_code = 200;
}

// Server Main Loop. This is where the magic operates
//
// 1. Push_back the binded server socket to the Pollfd vector and sets events to POLLIN.
//
// 2. Wait for an incoming connection from a client.
//
// 3. accept() the incoming connection, opens a FD for the client and adds it to the Pollfd vector.
//
// 4. Handle the client request. recv() the request header and puts it in a buffer. If recv() return 0
// the resquest has been resolved and the client FD is closed and removed from the vector. 
//
// 5. Parse the request header and build a response relevent to the information received.
//
// 6. Allocate a char* type buffer (handling image binary) the size of the response header + body.
// Copy the header AND the body to the buffer. send() the response to the sender (client fd).
//
// 7. Rince and Repeat
void Server::run(SocketsVector sockets)
{
	for (size_t i = 0; i < sockets.size(); i++)
		std::cout << YELLOW << logEvent("Server is listening on: " + sockets.at(i).getHostName() + "\n") << END << std::endl;

	while(true) {
		int ret = 0;
		if ((ret = poll(&(pfds.front()), pfds.size(), 10000)) <= 0) {
			(ret == -1) ? status_code = 500 : status_code = 408;
		}

		for (PollIterator it = pfds.begin(); it != pfds.end(); it++) {
			if ((*it).revents & POLLIN) 
			{
				for (size_t i = 0; i < pfds.size(); i++){
					if ((*it).fd == pfds.at(i).fd && (*it).fd != client_fd && i < sockets.size()){
						handleEvents(it, i);
						break;
					}
					if ((*it).fd == pfds.at(i).fd){
						int server_i = server_index.at(it->fd);
						handleClient(it, sockets.at(server_i).getServInfo());
						break;
					}
				}
			}
		}
	}
}

