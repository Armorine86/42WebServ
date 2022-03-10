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
		// TODO MAYBE add the validate line here with the format function.
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
			while (validLine(*scopeEnd) && scopeLevel > 0)
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

bool ConfigParser::validLine(std::string &line)
{
	(void)line;
	return true;
}

// Return Enum type depending on the Server field
server_fields ConfigParser::getFieldType(std::string &line)
{
	if (line.find("listen") != std::string::npos)
		return listen_field;
	// else if (line.find("host") != std::string::npos)
	// 	return host;
	else if (line.find("server_name") != std::string::npos)
		return server_name;
	else if (line.find("root") != std::string::npos)
		return root;
	else if (line.find("body_size") != std::string::npos)
		return body_size;
	else if (line.find("error") != std::string::npos)
		return error;
	return none;
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
		{
			start++;
			while ((*start).find("location") == std::string::npos)
			{
				if (!(*start).empty())
				{
					fillServerFields(split(*start, " "), serv_info, getFieldType(*start));
					start++;
				}
				else
				{
					start++;
				}
			}
		}
		else
		{
			; // TODO fillLocationFields(serv_info.location)
		}
	}
	servers_config.push_back(serv_info);
}

// Fill serv_info struct Server part with information
// parsed from the config file.
void ConfigParser::fillServerFields(StringVector vec, server_info &serv_info, server_fields type)
{

	// ** Temporary setup **

	// TODO	Find a better way of splitting the lines to easily retrieve field values
	// TODO	When arriving here, lines should've already been validated
	// TODO Make a Parse Directive function

	switch (type) {
		case listen_field:
		{
			// Listen field contains both host ip and listen port
			if (vec.size() == 2)
			{
				StringVector tmp = split(vec[1], ":");
				serv_info.host = tmp[0];
				serv_info.listen_port = atoi(right_trim(tmp[1], ";").c_str());
				break;
			}
			// if the listen and host are splitted in the conf file
			else if (vec.size() == 3)
			{
				if (vec[2] == "localhost" || vec[1] == LOCALHOST)
				{
					serv_info.host = vec[1];
					serv_info.listen_port = atoi(vec[2].c_str());
					break;
				}
				else
				{
					serv_info.host = vec[2];
					serv_info.listen_port = atoi(vec[1].c_str());
					break;
				}
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
			// TODO parseServerDirective(serv_info);
		}

		case none:
		{
			std::cerr << logEvent("[PARSE ERROR] Unrecognized Server Directive") << END << std::endl;
			throw std::runtime_error("Parse error: Unrecognized Server Field");
		}
	}
}

// Checks if at beginning of a server block
bool ConfigParser::isInServerScope(const std::string &line)
{
	if (line.find("server") == std::string::npos)
	{
		std::cerr << logEvent("[PARSE ERROR] Invalid Server Scope") << END << std::endl;
		return false;
	}
	if (line.find("{") == std::string::npos)
	{
		std::cerr << logEvent("[PARSE ERROR] Missing Opening Bracket") << END << std::endl;
		return false;
	}
	return true;
}
