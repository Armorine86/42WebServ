/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_fields.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:22:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/20 10:16:32 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

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

struct server_directive {
	std::string 	listen_port;
	std::string 	index;
	std::string 	host; 	//either 127.0.0.1 || localhost
	std::string 	root;
	StringVector 	server_names;
	StringVector 	error_pages;
	LocationVector	locations;
	unsigned int 	port;
	unsigned int	client_max_body_size;
};

// const char *server_fields[] = {
// 	"server",
// 	"listen",
// 	"host",
// 	"server_name",
// 	"root",
// 	"error",
// 	NULL,
// };
