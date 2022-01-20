/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:51:06 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/20 10:14:01 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iterator>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "config_fields.hpp"
#include "utils.hpp"
#include "defines.hpp"

class ConfigParser {
public:
	ConfigParser();
	ConfigParser(const std::string &file_path);
	~ConfigParser() {}
	
private:

	std::string default_config_file;
	// iterator for the config file content
	typedef std::vector<std::string>::iterator ParserIterator;

	// vector holding multiple servers blocks
	//std::vector<server_directive> servers;

	
	// Parser Functions
	std::string parseLine(std::string line);
	void sanitize_FileContent(StringVector &content);
	void parseConfigs(const std::string &file_path);
	bool lineIsValid(std::string line);
	bool isInServerScope(const std::string &line);
};
