/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 08:15:22 by mmondell          #+#    #+#             */
/*   Updated: 2022/02/22 08:47:36 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

#include <cerrno>

// Creates a socket for the server with the infos gathered in the parser.
// Makes the sockets Non-Blocking with fcntl(), and binds it to the port.
// If an error occurs, the program exits with appropriate code.
Sockets::Sockets(server_info& info) : serv_info(info)
{
	address.sin_family = AF_INET; //IPv4
	address.sin_port = htons(serv_info.listen_port);
	address.sin_addr.s_addr = inet_addr(serv_info.host.c_str());
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
		
		std::cerr << logEvent("[SOCKET] Invalid fd: ") << strerror(errno) << END << std::endl;
		exit(INV_SOCKET);
	}
	
	// Sets socket to non-blocking
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	
	int yes = 1;
	
	// Makes the port reusable if a socket lingers in the Kernel
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {

		std::cerr << logEvent("[SOCKET] Could not set Options: ") << strerror(errno) << END << std::endl;
		exit(OPT_ERR);
	}
	
	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		
		std::cerr << logEvent("[SOCKET] Binding Error: ") << strerror(errno) << END << std::endl;
		exit(BINDING_ERR);
	}

	if (listen(socket_fd, BACKLOG) < 0) {
		
		std::cerr << logEvent("[SOCKET] Could not listen on port: ") << strerror(errno) << END << std::endl;
		exit(LISTEN_ERR);
		
	}
}

int Sockets::getServFD()
{
	return socket_fd;
}
