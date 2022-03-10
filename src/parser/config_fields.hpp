/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_fields.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:22:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/03/10 15:46:04 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>

#include "defines.hpp"

// All the parameters contained in the different Location blocks in the config file
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
	StringVector	allowed_request;
};

typedef std::vector<location_directive> LocationVector;

// All the parameters contained in the server portion of the config file
struct server_info {
	std::string		root;
	std::string 	host; 	//either 127.0.0.1 || localhost
	std::string 	server_names;
	std::map<std::string, std::string> 	error_pages;
	LocationVector	locations;
	int listen_port;
	int	client_max_body_size;
};

// Used for the switch case function to build the server_info struct
typedef enum server_fields {
	listen_field,
	server_name,
	root,
	error,
	body_size,
	none
} server_fields;
