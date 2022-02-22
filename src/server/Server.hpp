/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:26:11 by mmondell          #+#    #+#             */
/*   Updated: 2022/02/22 08:56:46 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"
#include "Socket.hpp"
#include <poll.h>

// **Server class
//
// **This is where the infinite loop occurs, with the select() function to accept
// multiple clients.
class Server {
public:
	Server() {}
	~Server() {}
	
	void run();
	
private:
	Sockets socket;

	// int poll(struct pollfd fds[], nfds_t nfds, int timeout);
	std::vector<pollfd> pfds;  //fd vectors to use with poll()
};
