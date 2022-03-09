#include "Server.hpp"
#include "Response.hpp"

Server::Server(server_info serv_info)
{
	Sockets socket(serv_info);
	//_socket = socket;
	//Sockets socket = Sockets(serv_info);
	//socket = new Sockets(serv_info);
	run(socket);
}

pollfd Server::addToPollfd(int newfd) {
	pollfd new_pfd;

	new_pfd.fd = newfd;
	new_pfd.events = POLLIN;

	return new_pfd;
}

void Server::run(Sockets socket)
{
	pfds.push_back(addToPollfd(socket.getServFD()));
	//pfds[0].fd = socket.getServFD();
	pfds[0].events = POLLIN;
	// const char *response = "HTTP/1.1 200 OK\nContent-Type: text/html \nContent-Length: 700\n\n\
	// 	<!DOCTYPE html>\n\
	// 	<html>\n\
	// 	<head>\n\
	// 	<!-- HTML -->\n\
	// 	<title>\n\
	// 	Les surfeurs du web</title>\n\
	// 	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\
	// 	<style>\n\
	// 	body {background-color:#f55fff;background-image:url(/Surfer_Girl.jpg);background-repeat:repeat;background-position:top left;background-attachment:scroll;}\n\
	// 	h1{font-family:Impact, sans-serif;color:#000000;background-color:#ffffff;}\n\
	// 	p {font-family:Georgia, serif;font-size:14px;font-style:normal;font-weight:normal;color:#000000;background-color:#ffffff;}\n\
	// 	</style>\n\
	// 	</head>\n\
	// 	<body>\n\
	// 	<h1>Yo men bienvenue sur notre site trop swag!</h1>\n\
	// 	<p>Random text bla bla bla</p>\n\
	// 	<img src=\"Surfer_Girl.jpg\" alt=\"\">\n\
	// 	</body>\n\
	// 	</html>";
	//const char *response = "HTTP/1.1 301 Moved Permanently\nLocation: http://cyborgab.com/\n\n";

	std::cout << YELLOW << "Server is listening on: " << socket.getHostName() << END << std::endl;
	while(true) {
		if (poll(&(pfds.front()), pfds.size(), 60000) < 0) {
			perror("poll"); 
			exit(1);
		}

		for (std::vector<pollfd>::iterator it = pfds.begin(); it != pfds.end(); it++) {
			if ((*it).revents & POLLIN) {
				if ((*it).fd == socket.getServFD()) {
					socklen_t addrlen = sizeof(client_addr);

					client_fd = accept(pfds[0].fd, (struct sockaddr*)&client_addr, &addrlen);
		
					if (client_fd == -1)
						perror("accept");
					else {
						pfds.push_back(addToPollfd(client_fd));
						it = pfds.begin();
					}
				} 
				else 
				{
					int bytes = recv((*it).fd, buffer, sizeof(buffer), 0);
					//if buf is bigger than max_body_size : error 416 Request Entity Too Large
					// TODO Integrate RequestParser Class
					std::string str_buffer = buffer;

					if (DEBUG)
						std::cout << TEAL << str_buffer << END << std::endl;

					int sender_fd = (*it).fd;

					if (bytes <= 0)
					{
						if (bytes == 0)
							std::cout << "pollserver: socket " << sender_fd << " hung up" << std::endl; 
						else 
							perror("recv");
						close((*it).fd); // Bye!
                   		pfds.erase(it);
						it = pfds.begin();
					}

					// look on which server the request is suposed to go 
					else
					{
			//tout ce bout la peux etre une autre method ------------------------------------------
						RequestParser request(str_buffer);
						if ((*it).fd == sender_fd)
						{
							Response response(request);
							char * buffer = new char[1000000];
							bzero(buffer, 1000000);
							std::string header = response.getResponseHeader();
							std::string body = response.getResponseBody();
							size_t headerSize = response.getHeaderSize();
							size_t bodySize = response.getBodySize();

							for (size_t i = 0; i < headerSize; ++i)
								buffer[i] = header[i];
							for (size_t i = headerSize; i < headerSize + bodySize && i < 1000000; ++i)
								buffer[i] = body[i - headerSize];
							send(client_fd, buffer, headerSize + bodySize, 0);
			//----------------------------------------------------------------------------------------
						}
					}
				}
			}
		}
	}
}

Server::~Server() {
	//delete socket;
}
