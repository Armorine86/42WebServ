/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:02:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/20 13:32:31 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() : default_config_file(DEFAULT_CONFIG_FILE)
{
	parseFile(default_config_file);
}

ConfigParser::ConfigParser(const std::string &file_path) 
{
	parseFile(file_path);
}

// Parse the config file, formats each line and build a String Vector
void ConfigParser::parseFile(const std::string &file_path) 
{
	StringVector content;
	std::string line;
	std::ifstream file(file_path.c_str());
	
	if (!(file.is_open() && file.good())) {
		std::cerr << logEvent("[PARSE ERROR] Cannot open file") << END << std::endl;
		exit(EXIT_FAILURE);
	}

	while (std::getline(file, line))
		content.push_back(format_line(line));
	if (content.empty())
		std::cerr << logEvent("[PARSE ERROR] File is empty") << END << std::endl;
	parseConfig(content);
}

void ConfigParser::parseConfig(StringVector &content) 
{
	ParserIterator iter;
	
	iter = content.begin();
	bool inServerScope = isInServerScope(*iter);

	
}

bool ConfigParser::isInServerScope(const std::string &line) 
{
	StringVector tmp = split(line, " ");
	
	if (tmp.size() < 2) {
		std::cerr << logEvent("[PARSE ERROR] Invaline Server Scope") << END << std::endl;
		exit(EXIT_FAILURE);
	}
	
	if (tmp[0] == "server") {
		if (tmp[1] == "{") {
			return true;	
		}
	}
	
	return false;
}
