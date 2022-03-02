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

// **This class will parse the config file and build a vector with
// **all the infos of each server blocks found in the file passed as parameter	 
class ConfigParser {
public:
	ConfigParser();
	ConfigParser(const std::string &file_path);
	~ConfigParser() {}
	
	std::vector<server_info> getServersInfos();

private:
	// iterator for the config file content
	typedef std::vector<std::string>::iterator ParserIterator;

	std::string default_config_file;
	
	// vector holding multiple servers blocks
	std::vector<server_info> servers_config;


	// Parser Functions
	std::string parseLine(std::string line);
	void parseConfig(StringVector &content);
	void parseFile(const std::string &file_path);
	bool isInServerScope(const std::string &line);
	bool validLine(std::string &line);

	// Switch
	server_fields type;
	server_fields getFieldType(std::string line);

	// Will fill Servers_Infos private member vector with all the information
	// found in the config file to relevant fields
	void fillVector(ParserIterator start, ParserIterator end);
	void fillServerFields(StringVector vec, server_info &serv_info, server_fields type);
	
	
};
