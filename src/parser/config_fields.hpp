/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_fields.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:22:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/21 12:49:52 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>

#include "defines.hpp"

struct location_directive {
	bool			autoindex;
	bool			upload;
	std::string 	client_max_body_size;
	std::string 	upload_max_size;
	std::string		upload_directory;
	std::string		default_error_file;
	std::string		cgi_path;
	std::string		name;
	std::string		root;
	StringVector	methods;
	StringVector	redirections;
	StringVector	cgi_extensions;
};

typedef std::vector<location_directive> LocationVector;

struct server_info {
	std::string 	index;
	std::string 	host; 	//either 127.0.0.1 || localhost
	std::string 	root;
	StringVector 	server_names;
	std::map<int, std::string> 	error_pages;
	LocationVector	locations;
	int listen_port;
	int	client_max_body_size;
};

typedef enum server_fields {
	//server,
	listen_port,
	host,
	server_name,
	root,
	error,
	none
} server_fields;

//enum location_fields {
	
// }
