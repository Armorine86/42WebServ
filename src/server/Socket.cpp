/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 08:15:22 by mmondell          #+#    #+#             */
/*   Updated: 2022/02/21 12:21:51 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Socket.hpp"

Sockets::Sockets() {
	
	struct sockaddr_in sa;

	sa.sin_family = AF_INET; //IPv4
	sa.sin_port = htons(serv_info.listen_port);
	sa.sin_addr.s_addr = INADDR_ANY;
	memset(sa.sin_zero, '/0', sizeof(sa.sin_zero));

	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) <= 0) {
		
		//TODO Error Handling here
	}
	
	if (bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		
		//TODO Error Handling here
	}
}

