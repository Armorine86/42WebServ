#include "ConfigParser.hpp"

ConfigParser::ConfigParser() : default_config_file(DEFAULT_CONFIG_FILE)
{
	parseFile(default_config_file);
}

ConfigParser::ConfigParser(const std::string &file_path)
{
	parseFile(file_path);
}

server_info ConfigParser::getServersInfos(const int &index)
{
	return servers_config.at(index);
}

// Parse the config file, formats each line and build a String Vector
void ConfigParser::parseFile(const std::string &file_path)
{
	StringVector content;
	std::string line;
	std::ifstream file(file_path.c_str());

	if (!(file.is_open() && file.good()))
	{
		std::cerr << logEvent("[PARSE ERROR] Cannot open file") << END << std::endl;
		throw std::runtime_error("Filestream error: Cannot open file");
	}

	while (std::getline(file, line))
		content.push_back(format_line(line));

	if (content.empty())
	{
		std::cerr << logEvent("[PARSE ERROR] File is empty") << END << std::endl;
		throw std::runtime_error("File is empty");
	}
	parseConfig(content);
}

// Declare two iterators that will encapsulate a Serve scope
// including all it's location field
void ConfigParser::parseConfig(StringVector &content)
{
	ParserIterator scopeStart, scopeEnd;

	scopeStart = content.begin();
	scopeEnd = content.begin();

	for (size_t i = 0; i < content.size(); ++i)
	{
		if ((*scopeEnd).empty())
		{
			scopeEnd++;
			scopeStart = scopeEnd;
		}
		if (isInServerScope(*scopeStart))
		{
			int scopeLevel = 1;
			scopeEnd++;
			i++;
			while (scopeLevel > 0)
			{
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
				fillConfigVector(scopeStart, scopeEnd);
			else
			{
				std::cerr << logEvent("[PARSE ERROR] Scope isn't closed... Missing `}`") << END << std::endl;
				throw std::runtime_error("Parse Error: Bad Config File Syntax");
			}
		}
		else
			throw std::runtime_error("Parse Error: Not in server Scope");
	}
}

// Receives a Server Scope delimited by two iterators
// Builds the serv_info structs and the location_directive structs
//
// When serv_info structs is filled with relevent infos
// Pushes the serv_info into the servers_config Vector
//
// A Config File may have several Server Blocks with different configs
void ConfigParser::fillConfigVector(ParserIterator start, ParserIterator end)
{
	server_info serv_info;

	for (; start != end; start++)
	{
		if ((*start).find("server") != std::string::npos)
			start++;
		while ((*start).find("location") == std::string::npos)
		{
			if (!(*start).empty() && (*start).find("}") == std::string::npos)
			{
				fillServerFields(split(*start, " "), serv_info, getFieldType(*start));
				start++;
			}
			else
				break;
		}
		if ((*start).find("location") != std::string::npos){
			location_info fields;
			while ((*start).find("}") == std::string::npos){
				fillLocationFields(split(*start, " "), fields, getLocationType(*start));
				start++;
			}
			serv_info.locations.push_back(fields);
		}
	}
	servers_config.push_back(serv_info);
}

// Fill serv_info struct Server part with information
// parsed from the config file.
void ConfigParser::fillServerFields(StringVector vec, server_info &serv_info, server_fields type)
{
	switch (type) {
		case listen_field:
		{
			// Listen field contains both host ip and listen port
			if (vec.size() == 2)
			{
				StringVector tmp = split(vec[1], ":");
				for (size_t i = 0; i < tmp.size(); i++){
					if (tmp[i].size() > 5)
						serv_info.host = right_trim(tmp[i], ";");
					else
						serv_info.listen_port = atoi(right_trim(tmp[i], ";").c_str());
				}
				if (serv_info.host.empty())
					serv_info.host = LOCALHOST;
				if (serv_info.listen_port == 0)
					serv_info.listen_port = LISTENPORT;
			}
			else
			{
				std::cerr << logEvent("[PARSE ERROR] Missing Listen Directive Value...") << END << std::endl;
				throw std::runtime_error("Parse error: Missing Listen Field");
			}
		}
		case server_name:
		{
			serv_info.server_names = vec[1];
			// TODO serv_info.server_names.push_back(vec[1]);
			break;
		}

		case root:
		{
			serv_info.root = vec[1];
			break;
		}

		case body_size:
		{
			serv_info.client_max_body_size = atoi(right_trim(vec[1], ";").c_str());
			break;
		}

		case error:
		{
			if (vec.size() == 3){
				vec[2].erase(vec[2].end() - 1);
				serv_info.error_pages.insert(std::pair<std::string, std::string>(vec[1], vec[2]));
			}
			break;
		}

		case none:
		{
			std::cerr << logEvent("[PARSE ERROR] Unrecognized Server Directive") << END << std::endl;
			throw std::runtime_error("Parse error: Unrecognized Server Field");
		}
	}
}

void ConfigParser::fillLocationFields(StringVector vec, location_info &fields, location_fields loc_type) 
{
	switch (loc_type) {
		case name:
		{
			if (vec[1].find('{') != std::string::npos)
				vec[1].erase(vec[1].size() - 1);
			fields.name = vec[1];
			break;
		}
		case loc_root:
		{
			fields.root = vec[1].erase(vec[1].size() - 1);
			break;
		}
		case loc_index:
		{
			fields.index = vec[1].erase(vec[1].size() - 1);
			break;
		}
		case cgi_extensions:
		{
			fields.cgi_extensions[vec[1]] = vec[2].erase(vec[2].size() - 1);
			break;
		}
		case autoindex:
		{
			if (vec[1].find("on") != std::string::npos)
				fields.autoindex = true;
			break;
		}
		case upload_directory:
		{
			fields.upload_directory = vec[1].erase(vec[1].size() - 1);
			break;
		}
		case upload_max_size:
		{
			fields.upload_max_size = atoi(vec[1].erase(vec[1].size() - 1).c_str());
			break;
		}
		case allowed_request:
		{
			for (StringIterator it = vec.begin() + 1; it < vec.end(); ++it){
				if (it == vec.end()){
					std::string& str = *it; 
					str.erase(str.size() - 1);
				}
       			fields.allowed_request.push_back(*it);
			}
			break;
		}
		case redirections:
		{
			fields.redirections = vec[1];
			break;
		}
		case loc_none:
		{
			std::cerr << logEvent("[PARSE ERROR] Unrecognized Server Location Directive") << END << std::endl;
			throw std::runtime_error("Parse error: Unrecognized Server Location Field");
		}
	}
	//TODO set default value if none
}
