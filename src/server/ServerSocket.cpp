/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 08:57:26 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/18 14:45:24 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "ServerSocket.hpp"
#include "colors.hpp"
#include "utils.hpp"

// 1. Create the socket
// 2. Identify the socket
// 3. On the server, wait for an incoming connection
// 4. Send and receive messages
// 5. Close the socket
ServerSocket::ServerSocket(int port, int QueuedClient) :
	listening_port(port),
	server_fd(),
	MaxQueuedClient(QueuedClient)
{
	// Address the server will be listening on
	bzero((char *) &socketAddr, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = INADDR_ANY;
	socketAddr.sin_port = htons(listening_port); // Converts short to network representation
}

ServerSocket::ServerSocket(const ServerSocket& src) :
	listening_port(src.listening_port),
	server_fd(src.server_fd),
	MaxQueuedClient(src.MaxQueuedClient),
	socketAddr(src.socketAddr) {}
	
ServerSocket ServerSocket::operator=(const ServerSocket& rhs) 
{
	if (this == &rhs)
	{
		this->listening_port = rhs.listening_port;
		this->server_fd = rhs.server_fd;
		this->MaxQueuedClient = rhs.MaxQueuedClient;
		
		return *this;
	}
	return *this;
}

void ServerSocket::CreateServerSocket() 
{
	// AF_INET --> Ipv4 Protocol family (domain)
	// SOCK_STREAM --> Means this is a TCP socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("ServerSocket: Error getting Socket FD\n");
	logEvent("server FD assigned succesfully");

	bind_port();
	
	// QueuedClient--> nb of pending clients before a connection is refused
	listen(server_fd, MaxQueuedClient);
	logEvent("listening...");
}

void ServerSocket::bind_port() 
{
	if (bind(server_fd, (struct sockaddr *)&socketAddr, sizeof(socketAddr)) < 0)
		throw std::runtime_error("ServerSocket: Error binding socket\n");
	logEvent("socket binding successful");
}

int ServerSocket::getFD() 
{
	return server_fd;
}

int ServerSocket::getPort() 
{
	return listening_port;
}

struct sockaddr_in ServerSocket::getAddress() 
{
	return socketAddr;
}
