/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 08:06:44 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/21 11:07:57 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "ConfigParser.hpp"

//typedef std::vector<Sockets *> SocketVector;

// Builds Sockets based on the informations collected by the ConfigParser class
// This class lives within the Server Class
class Sockets {
public:
	Sockets();
	Sockets(const Sockets& src);
	Sockets(server_info serv_info);
	~Sockets();

	int getServFD();
	int getClientFP();
	server_info getServConfig();

private:
	int socket_fd;
	int fd_opt;
	int addrlen;
	struct sockaddr_in address;
	server_info serv_info;  // Contain server infos from Config file
};
