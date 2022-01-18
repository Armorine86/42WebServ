/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 08:57:26 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/17 13:54:24 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "colors.hpp"

ServerSocket::ServerSocket(int port, int QueuedClient) :
	listening_port(port),
	server_fd(),
	QueuedClient(QueuedClient) {}

ServerSocket::ServerSocket(const ServerSocket& src) :
	listening_port(src.listening_port),
	server_fd(src.server_fd),
	QueuedClient(src.QueuedClient) {}
	
ServerSocket ServerSocket::operator=(const ServerSocket& rhs) 
{
	if (this == &rhs)
	{
		this->listening_port = rhs.listening_port;
		this->server_fd = rhs.server_fd;
		this->QueuedClient = rhs.QueuedClient;
		
		return *this;
	}
	return *this;
}

void ServerSocket::CreateServerSocket() 
{
	// AF_INET --> Ip Protocol family 
	// SOCK_STREAM --> Means this is a TCP socket
	if (server_fd = socket(AF_INET, SOCK_STREAM, 0) < 0)
			throw std::runtime_error("ServerSocket: Error getting Socket FD\n");
			
	try {
		bind_port();
	} catch (std::runtime_error &e) {
		std::cerr << RED << e.what() << END << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// QueuedClient--> nb of pending clients before a connection is refused
	listen(server_fd, QueuedClient);
}

void ServerSocket::bind_port() 
{
	struct sockaddr_in socketAddr;
	bzero((char * ) &socketAddr, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = INADDR_ANY;
	socketAddr.sin_port = htons(listening_port); // Converts short to network representation
	
	if (bind(server_fd, (struct sockaddr *)&socketAddr, sizeof(socketAddr)) < 0)
		throw std::runtime_error("ServerSocket: Error binding socket\n");
}

const char* ServerSocket::BadFdException::what() const throw()
{
	return "ServerSocket: Error getting File Descriptor";
}
