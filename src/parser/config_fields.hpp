/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_fields.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcollet <gcollet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:22:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/03/11 16:25:27 by gcollet          ###   ########.fr       */
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
	std::string		name;
	std::string		index;
	std::string		root;
	cgiMap			cgi_extensions;
	bool			autoindex;
	bool			upload;
	int				upload_max_size;
	std::string		upload_directory;
	StringVector	allowed_request;
	int				redirections;
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
