/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 16:56:31 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/18 09:10:26 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <exception>
#include <list>

#include "colors.hpp"
#include "ServerSocket.hpp"

#define DEFAULT_PORT 4242

// STEP 1. Client requests a web page from the server
// STEP 2. Server sends an html file to the client
// STEP 3. Client request .css / .js files to display web page correctly
// STEP 4. Server Sends requested file to the client
// STEP 5. Client requests Image files if needed
// STEP 6. Server sends requested files to the client
int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << RED << "Error: Invalid arguments" << END << std::endl;
		exit(EXIT_FAILURE);
	}

	// Pass in argv[1] --> Config File
	//TODO ConfigParser config(argv[1])
			
	// Server Socket init
	ServerSocket serverSock(4242, 5);

	try {
		serverSock.CreateServerSocket();
	} catch (std::runtime_error &e) {
		std::cerr << RED <<  e.what() << END << std::endl;
		exit(EXIT_FAILURE);
	}
}
