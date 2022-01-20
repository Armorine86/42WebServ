/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:02:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/19 16:10:28 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &file_path) 
{
	StringVector content;
	std::string line;
	std::ifstream file(file_path.c_str());
	
	if (!(file.is_open() && file.good())) {
		std::cerr << logEvent("Error: cannot open file") << END << std::endl;
		exit(EXIT_FAILURE);
	}

	while (std::getline(file, line))
		content.push_back(parseLine(line));
	std::cout << "test" << std::endl;
}

void ConfigParser::parseConfigs(std::string content) 
{
	(void)content;
	//ParserIterator iter;
	
}

std::string ConfigParser::parseLine(std::string &line) 
{
	std::string cleaned_line = line;

	
	if (!lineIsValid(cleaned_line))
		std::cerr << logEvent("Error: Invalid Config Directive") << END << std::endl;
		
	return cleaned_line;
}

bool ConfigParser::lineIsValid(std::string line) 
{
	(void)line;
	return true;
}
