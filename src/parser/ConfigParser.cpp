/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:02:37 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/22 13:41:59 by mmondell         ###   ########.fr       */
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

std::vector<server_info> ConfigParser::getServInfos() 
{
	return servers_block;
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
		//TODO MAYBE add the validate line here with the format function.
		content.push_back(format_line(line));
	if (content.empty()) {
		std::cerr << logEvent("[PARSE ERROR] File is empty") << END << std::endl;
		exit(PARSING_ERROR);
	}
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
				fillVector(scopeStart, scopeEnd);
			else {
				std::cerr << logEvent("[PARSE ERROR] Scope isn't closed... Missing `}`") << END << std::endl;
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

server_fields ConfigParser::getFieldType(std::string line) 
{
	if (line.find("listen") != std::string::npos)
		return listen_port;
	else if (line.find("host") != std::string::npos)
		return host;
	else if (line.find("server_name") != std::string::npos)
		return server_name;
	else if (line.find("root") != std::string::npos)
		return root;
	else if (line.find("error") != std::string::npos)
		return error;
	return none;
}

void ConfigParser::fillVector(ParserIterator start, ParserIterator end) 
{
	server_info serv_info;
	
	for (; start != end; start++) {
		if ((*start).find("server") != std::string::npos) {
			start++;
			while ((*start).find("location") == std::string::npos){
				if (!(*start).empty()) {
					fillServerFields(split(*start, " "), serv_info, getFieldType(*start));
					start++;
				}
				start++;
			}
		} else {
			; //serv_info.locations
		}
	}
	servers_block.push_back(serv_info);
}

void ConfigParser::fillServerFields(StringVector vec, server_info &serv_info, server_fields type) 
{
	
	// ** Temporary setup ** 

	// TODO	Find a better way of splitting the lines to easily retrieve field values
	// TODO	When arriving here, lines should've already been validated
	// TODO Make a Parse Directive function
	
	switch (type) {
		case listen_port: {
			if (vec.size() == 2) {
				serv_info.listen_port = atoi(right_trim(vec[1], ";").c_str());
				break;
			}
			else if (vec.size() == 3) {
				if (vec[2] == "localhost" || vec[1] == LOCALHOST) {
					serv_info.host = vec[1];
					serv_info.listen_port = atoi(vec[2].c_str());
					break;
				} else {
					serv_info.host = vec[2];
					serv_info.listen_port = atoi(vec[1].c_str());
					break;
				}
			}
			else {
				std::cerr << logEvent("[PARSE ERROR] Missing Listen Directive Value...") << END << std::endl;
			}
		}
		case host: {
			serv_info.host = vec[1];
			break;
		}
		case server_name: {
			serv_info.server_names.push_back(vec[1]);
			break;
		}
		case root: {
			serv_info.root = vec[1];
			break;
		}
		case error: {
			if (vec.size() < 3) {
				vec = split(vec[1], ",");
				serv_info.error_pages.insert(std::pair<int, std::string>(atoi(vec[1].c_str()), vec[0]));
			} else {
				serv_info.error_pages.insert(std::pair<int, std::string>(atoi(vec[2].c_str()), vec[1]));
			}
			break;
			// parseServerDirective(serv_info);
		}
		case none: {
			std::cerr << logEvent("[PARSE ERROR] Unrecognized Server Directive") << END << std::endl;
		}
	}
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
