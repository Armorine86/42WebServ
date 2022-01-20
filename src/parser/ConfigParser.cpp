/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:02:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/20 16:16:28 by mmondell         ###   ########.fr       */
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
		exit(FILESTREAM_ERROR);
	}

	while (std::getline(file, line))
		content.push_back(format_line(line));
	if (content.empty())
		std::cerr << logEvent("[PARSE ERROR] File is empty") << END << std::endl;
	parseConfig(content);
}

void ConfigParser::parseConfig(StringVector &content) 
{
	ParserIterator scopeStart, scopeEnd;
	
	scopeStart = content.begin();
	scopeEnd = content.begin();
	
	for (size_t i = 0; i < content.size(); ++i) {
		if ((*scopeEnd).empty()){
			scopeEnd++;
			scopeStart = scopeEnd;
		}
		if (isInServerScope(*scopeStart)) {
			int scopeLevel = 1;
			scopeEnd++;
			i++;
			while (validLine(*scopeEnd) && scopeLevel > 0){
				if ((*scopeEnd).find('{') != std::string::npos)
					scopeLevel++;
				else if ((*scopeEnd).find('}') != std::string::npos)
					scopeLevel--;
				if (scopeEnd == content.end())
					break;
				scopeEnd++;
				i++;
			}
			if (scopeLevel == 0)
				createServer(scopeStart, scopeEnd);
			else {
				std::cerr << logEvent("[PARSE ERROR] Invalid Scope.. Missing `}`") << END << std::endl;
				exit(PARSING_ERROR);
			}
		}
	}
}

bool ConfigParser::validLine(std::string &line) 
{
	(void)line;
	return true;	
}

void ConfigParser::createServer(ParserIterator start, ParserIterator end) 
{
	
}

bool ConfigParser::isInServerScope(const std::string &line) 
{
	StringVector tmp = split(line, " ");
	
	if (tmp.size() < 2) {
		std::cerr << logEvent("[PARSE ERROR] Invalid Server Scope") << END << std::endl;
		exit(PARSING_ERROR);
	}
	
	if (tmp[0] == "server") {
		if (tmp[1] == "{") {
			return true;	
		}
		else {
			std::cerr << logEvent("[PARSE ERROR] Missing opening `{`") << END << std::endl;
			exit(PARSING_ERROR);
		}
	}
	return false;
}
