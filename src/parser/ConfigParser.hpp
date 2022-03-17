#ifndef __CONFIGPARSER_H__
#define __CONFIGPARSER_H__

#include <fstream>
#include <iterator>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "config_fields.hpp"
#include "utils.hpp"
#include "defines.hpp"
#include "status_codes.hpp"

// **This class will parse the config file and build a vector with
// **all the infos of each server blocks found in the file passed as parameter	 
class ConfigParser {
public:
	ConfigParser();
	ConfigParser(const std::string &file_path);
	~ConfigParser() {}
	
	server_info getServersInfos(const int& index);
	int getServersSize() { return servers_config.size(); }

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

	// Switch case

	void fillServerFields(StringVector vec, server_info &serv_info, server_fields type);
	void fillLocationFields(StringVector vec, location_info &fields, location_fields type);
	void validLocation(location_info &fields, server_info &serv_info);
	server_fields getFieldType(std::string& line);
	location_fields getLocationType(std::string &line);
	server_fields type;
	location_fields loc_type;

	// Will fill Servers_Infos private member vector with all the information
	// found in the config file to relevant fields
	void fillConfigVector(ParserIterator start, ParserIterator end);
	
};

#endif // __CONFIGPARSER_H__
