/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:02:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/20 12:27:47 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() : default_config_file(DEFAULT_CONFIG_FILE)
{
	parseConfigs(default_config_file);
}

ConfigParser::ConfigParser(const std::string &file_path) 
{
	parseConfigs(file_path);
}


void ConfigParser::parseConfigs(const std::string &file_path) 
{
	StringVector content;
	std::string line;
	std::ifstream file(file_path.c_str());
	
	if (!(file.is_open() && file.good())) {
		std::cerr << logEvent("Error: cannot open file") << END << std::endl;
		exit(EXIT_FAILURE);
	}

	while (std::getline(file, line))
		content.push_back(format_line(line));
	
}

// Validates fields and if a value is associated with it.
// If valid, returns
std::string ConfigParser::parseLine(std::string line) 
{
	
	if (!lineIsValid(line))
		std::cerr << logEvent("Error: Invalid Config Directive") << END << std::endl;
		
	return line;
}


bool ConfigParser::lineIsValid(std::string line) 
{
	(void)line;
	return true;
}

bool ConfigParser::isInServerScope(const std::string &line) 
{
	if ((line.find("server")) != std::string::npos) {
		return true;
	}
	return false;
}
