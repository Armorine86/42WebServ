#include "Server.hpp"

Server::Server(server_info serv_info)
{
	Sockets socket(serv_info);
	//_socket = socket;
	//Sockets socket = Sockets(serv_info);
	//socket = new Sockets(serv_info);
	run(socket);
}

pollfd addToPollfd(int newfd) {
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
	const char *response = "HTTP/1.1 200 OK\nContent-Type: text/html \nContent-Length: 700\n\n\
		<!DOCTYPE html>\n\
		<html>\n\
		<head>\n\
		<!-- HTML Codes by Quackit.com -->\n\
		<title>\n\
		Les surfeurs du web</title>\n\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\
		<style>\n\
		body {background-color:#f55fff;background-image:url(/Surfer_Girl.jpg);background-repeat:repeat;background-position:top left;background-attachment:scroll;}\n\
		h1{font-family:Impact, sans-serif;color:#000000;background-color:#ffffff;}\n\
		p {font-family:Georgia, serif;font-size:14px;font-style:normal;font-weight:normal;color:#000000;background-color:#ffffff;}\n\
		</style>\n\
		</head>\n\
		<body>\n\
		<h1>Yo men bienvenue sur notre site trop swag!</h1>\n\
		<p>Random text bla bla bla</p>\n\
		<img src=\"Surfer_Girl.jpg\" alt=\"\">\n\
		</body>\n\
		</html>";


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
					else 
						pfds.push_back(addToPollfd(client_fd));
				} 
				else {
					int bytes = recv((*it).fd, buffer, sizeof(buffer), 0);
					//if buf is bigger than max_body_size : error 416 Request Entity Too Large
					// TODO Integrate RequestParser Class

					int sender_fd = (*it).fd;

					if (bytes <= 0) {
						if (bytes == 0) {
							std::cout << "pollserver: socket " << sender_fd << " hung up" << std::endl;
						} else {
							perror("recv");
						}

						close((*it).fd);

						pfds.erase(it);
					}
					else {
						if ((*it).fd == sender_fd) {
							std::string request = buffer;
							if (request.find("/Surfer_Girl") != std::string::npos) {
								std::string header = "HTTP/1.0 200 OK\r\nContent-type: image/jpeg \r\nContent-Length: 409059\r\n\r\n";
								send(client_fd, header.data(), header.length(), 0);

								std::ifstream f("favicon.ico", std::ios::in|std::ios::binary|std::ios::ate);
                                if(!f.is_open()) perror ("bloody file is nowhere to be found. Call the cops");
                            	std::streampos size = f.tellg();
                                char* image = new char [static_cast<long>(size)];
                                f.seekg (0, std::ios::beg);
                                f.read (image, size);
                                f.close();

                                send (client_fd, image, size, MSG_CONFIRM);
                                
                                std::cout << GREEN << "+++ FAVICON RESPONSE +++\n\n" << END << header << image << std::endl;
                                } else {
        
                                std::cout << GREEN << "+++ RESPONSE +++\n\n" << END << response << std::endl;
                            	send(client_fd, response, strlen(response), 0);
							}
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
