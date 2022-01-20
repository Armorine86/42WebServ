/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 16:56:31 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/19 15:22:27 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.hpp"
#include "utils.hpp"
#include "ConfigParser.hpp"

void close_serv(int sig)
{
	(void)sig;
	std::cout << YELLOW << "Closing Server...." << END << std::endl;
}

int main(int argc, char** argv)
{
	std::string config_path;
	
	signal(SIGINT, close_serv);
	signal(SIGQUIT, close_serv);
	
	if (argc <= 2){
		
		config_path = (argc == 2) ? argv[1] : DEFAULT_CONFIG_FILE; // No arguments = default file
		
		ConfigParser config(config_path);

	} else {
		std::cerr << RED << "Error: Too Many Arguments" << END << std::endl;
		exit(EXIT_FAILURE);
	}
}
