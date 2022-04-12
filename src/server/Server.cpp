#include "Server.hpp"
#include "Response.hpp"

// Builds the pollfd vector with the server sockets and runs the servers
Server::Server(SocketsVector sockvector) : client_fd(0), status_code("200"), sockets(sockvector), isChunked(false), tropBeaucoup(false)
{
	for (size_t i = 0; i < sockets.size(); i++)
	{
		pollfd newfd = addToPollfd(sockets.at(i).getServFD());
		pfds.push_back(newfd);
		pfds[i].events = POLLIN;
	}

	run();
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
std::string clientIP(const int &client_fd, socklen_t addrlen)
{

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
void Server::handleEvents(PollIterator &it, size_t i)
{
	socklen_t addrlen = sizeof(client_addr);

	client_fd = accept(pfds[i].fd, (struct sockaddr *)&client_addr, &addrlen);

	switch (client_fd) {
		case -1:
		{
			std::cerr 	<< logEvent("[ACCEPT] Could not Accept Connection")
						<< strerror(errno) << END << std::endl;
			break;
		}
		default:
		{
			PRINT_CLIENT_IP

			pfds.push_back(addToPollfd(client_fd));
			it = pfds.begin();

			while (it->fd != pfds[i].fd)
				it++;

		}
	}
}

// Handles Client requests. If the FD in the pollfd vector is
// the sender FD (client), we can send the response to the same FD
void Server::handleClient(PollIterator &it)
{
	sender_fd = it->fd;

	char buffer[RECV_BUFSIZE];
	bzero(buffer, sizeof(buffer));

	bytes = recv(it->fd, buffer, RECV_BUFSIZE, 0);

	switch (bytes) {

		case -1:
		{
			perror("recv");
			std::cerr 	<< logEvent("[RECV] Error Receiving Data")
						<< strerror(errno) << END << std::endl;
			closeSocket(it);
			break;
		}

		case 0:
		{
			closeSocket(it);
			break;
		}

		default:
		{
			for (int i = 0; i < bytes; i++)
				str_buffer.push_back(buffer[i]);

			DEBUG_DISPLAY_HEADER

			if (tropBeaucoup)
			{
				str_buffer = "";
				if (bytes != RECV_BUFSIZE)
					tropBeaucoup = false;
				return;
			}
			
			if (it->fd == sender_fd)
			{
				sendResponse(str_buffer, sender_fd, buffer);
				bzero(buffer, sizeof(buffer));
				str_buffer.clear();
			}
		}
	}
}

// Parse the request sent by the client and builds a response.
// Memcpy the Header and the Body into a buffer to use with send().
void Server::sendResponse(std::string &str_buffer, int sender_fd, char *buf)
{
	RequestParser request(str_buffer, buf);
	Response response(&request, this);

	if (isChunked == false)
	{
		std::string header = response.getResponseHeader();
		std::string body = response.getResponseBody();

		size_t MAX_SEND = response.getBodySize() + response.getHeaderSize() + 1;

		char *buffer = new char[MAX_SEND];
		bzero(buffer, MAX_SEND);

		memcpy(buffer, header.data(), header.length());
		memcpy(buffer + header.length(), body.data(), body.length());

		DEBUG_DISPLAY_RESP_HEADER

		if (send(sender_fd, buffer, MAX_SEND, 0) == -1) {
			std::cerr 	<< logEvent("[SEND] Error Sending Data")
						<< strerror(errno) << END << std::endl;
		}

		delete[] buffer;
		bin_boundary = "";
		status_code = "200";
	}
}

void Server::closeSocket(PollIterator &it)
{
	std::cout << "pollserver: socket " << sender_fd << " hung up" << std::endl;

	close(it->fd);
	pfds.erase(it);
	it = pfds.begin();

	status_code = "200";
}

// Main Loop
void Server::run()
{
	for (size_t i = 0; i < sockets.size(); i++)
		std::cout 	<< YELLOW << logEvent("Server is listening on: " + sockets.at(i).getHostName() + "\n")
					<< END << std::endl;

	while (true)
	{
		int ret = 0;

		if ((ret = poll(&(pfds.front()), pfds.size(), 10000)) <= 0)
			(ret == -1) ? status_code = "500" : status_code = "408";

		if (status_code == "500")
			std::cout << BRED << "INTERNAL SERVER ERROR [500]" << END << std::endl;

		for (PollIterator it = pfds.begin(); it != pfds.end(); it++)
		{
			if (it->revents & POLLIN)
			{
				for (size_t i = 0; i < pfds.size(); i++)
				{
					if (it->fd == pfds.at(i).fd && it->fd != client_fd && i < sockets.size())
					{
						handleEvents(it, i);
						break;
					}
					if (it->fd == pfds.at(i).fd)
					{
						handleClient(it);
						break;
					}
				}
			}
		}
	}
}
