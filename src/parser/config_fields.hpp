/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_fields.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:22:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/03/16 11:50:10 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>

#include "defines.hpp"

typedef std::map<std::string, std::string> cgiMap;

// All the parameters contained in the different Location blocks in the config file
struct location_info {
	cgiMap			cgi_extensions;
	StringVector	allowed_request;
	std::string		name;
	std::string		index;
	std::string		root;
	std::string		upload_directory;
	std::string		redirections;
	int				upload_max_size;
	bool			upload;
	bool			autoindex;
};

typedef std::vector<location_info> LocationVector;

// All the parameters contained in the server portion of the config file
struct server_info {
	std::string		root;	//root of the server
	std::string 	host; 	//either 127.0.0.1 || localhost
	std::string 	server_names;
	std::map<std::string, std::string> 	error_pages;
	LocationVector	locations;
	int listen_port;
	unsigned int	client_max_body_size; //in megabyte

	server_info() {
		listen_port = 0;
		client_max_body_size = 0;
	}
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

typedef enum location_fields {
	name,
	loc_root,
	loc_index,
	cgi_extensions,
	autoindex,
	upload_directory,
	upload_max_size,
	allowed_request,
	redirections,
	loc_none
} location_fields;
