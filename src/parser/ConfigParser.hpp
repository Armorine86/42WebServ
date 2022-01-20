/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:51:06 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/19 16:08:56 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iterator>
#include <cstdlib>
#include <iostream>
#include "config_fields.hpp"
#include "utils.hpp"

class ConfigParser {
public:
	ConfigParser() {}
	ConfigParser(const std::string &file_path);
	
private:
	// iterator for the config file content
	typedef std::vector<std::string>::iterator ParserIterator;

	// vector for multiple servers
	//std::vector<config_directive> servers;

	
	// private helper functions
	void parseConfigs(std::string content);
	std::string parseLine(std::string &line);
	bool lineIsValid(std::string line);
	
};
