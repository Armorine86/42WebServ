/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 16:42:59 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/18 10:10:28 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>

class ServerSocket {
public:
	ServerSocket() {}
	ServerSocket(int port, int MaxQueuedClient);
	ServerSocket(const ServerSocket& src);
	~ServerSocket() {}
	ServerSocket operator=(const ServerSocket& rhs);
	void CreateServerSocket();
	void bind_port();

private:
	int listening_port;
	int server_fd;
	int MaxQueuedClient;
		
};
