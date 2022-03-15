#include "ConfigParser.hpp"

// Return Enum type depending on the Server field
server_fields ConfigParser::getFieldType(std::string &line)
{
	if (line.find("listen") != std::string::npos)
		return listen_field;
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

// Return Enum type depending on the Location field
location_fields ConfigParser::getLocationType(std::string &line)
{
	if (line.find("location") != std::string::npos)
		return name;
	else if (line.find("root") != std::string::npos)
		return loc_root;
	else if (line.find("index") != std::string::npos && line.find("autoindex") == std::string::npos)
		return loc_index;
	else if (line.find("cgi_ext") != std::string::npos)
		return cgi_extensions;
	else if (line.find("autoindex") != std::string::npos)
		return autoindex;
	else if (line.find("upload_dir") != std::string::npos)
		return upload_directory;
	else if (line.find("upload_max_size") != std::string::npos)
		return upload_max_size;
	else if (line.find("allow_request") != std::string::npos)
		return allowed_request;
	else if (line.find("return") != std::string::npos)
		return redirections;
	return loc_none;
}

// Checks if at beginning of a server block
bool ConfigParser::isInServerScope(const std::string &line)
{
	if (line.find("server") == std::string::npos)
	{
		std::cerr << logEvent("[PARSE ERROR] Invalid Server Scope\n") << END << std::endl;
		return false;
	}
	if (line.find("{") == std::string::npos)
	{
		std::cerr << logEvent("[PARSE ERROR] Missing Opening Bracket\n") << END << std::endl;
		return false;
	}
	return true;
}
