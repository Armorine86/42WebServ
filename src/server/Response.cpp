#include "Response.hpp"

Response::Response(RequestParser *request, Server *server) : autoindex(false), bodySize(0), request(request), status_code(server->status_code), server(server)
{
	if (server->isChunked == true)
	{
		responseMultipart();
		return;
	}

	setConfig();
	path = lookForRoot(config.locations);

	MethodType type = getType();
	switch (type)
	{
	case GET:
		responseGET();
		break;
	case POST:
		responsePOST();
		break;
	case DELETE:
		responseDELETE();
		break;
	default:
		std::cerr << logEvent("Response: [405] Method not Allowed\n") << END << std::endl;
	}
}

MethodType Response::getType()
{
	std::string method = request->getMethod();

	if (method.find("GET") != std::string::npos)
		return GET;
	else if (method.find("POST") != std::string::npos)
		return POST;
	else if (method.find("DELETE") != std::string::npos)
		return DELETE;
	return NONE;
}

// This find and set in the map the right config
void Response::setConfig()
{
	std::string host = request->getHost();
	StringVector host_vec = split(host, ":");

	if (host_vec.empty())
		return;

	int port = 0;

	port = atoi(host_vec[1].c_str());

	for (size_t i = 0; i < server->sockets.size(); i++)
	{
		server_info tmp = server->sockets.at(i).getServInfo();
		if ((host_vec[0] == tmp.host || host_vec[0] == "localhost") && port == tmp.listen_port)
		{
			config = tmp;
			break;
		}
	}
	if (lookForRoot(config.locations) == "")
	{
		int i = -1;
		if ((i = findSocket()) >= 0)
		{
			LocationVector tmp_location = server->sockets.at(i).getServInfo().locations;
			if (lookForRoot(tmp_location) != "")
			{
				config = server->sockets.at(i).getServInfo();
			}
			else
				status_code = "404";
		}
	}
}

// Look for the asked root in the config file
std::string Response::lookForRoot(LocationVector &location)
{
	std::string path = "";
	StringVector url_vec = split(request->getURL(), "/");

	for (size_t i = 0; i < location.size() && path == ""; i++)
	{
		if (!url_vec.empty() && location.at(i).name == "/" + url_vec[0])
		{
			path = setPath(location, url_vec, i, false);
		}
		if (location.at(i).name == "/")
		{
			path = setPath(location, url_vec, i, true);
		}
		if (path == "/redirection")
			break;
		if (!is_valid(path))
			path = "";
	}
	return path;
}

// Create the path that the request asked for using the config file
std::string Response::setPath(LocationVector &location, StringVector &url_vec, size_t i, bool var)
{
	std::string path = "";

	path = location.at(i).root;
	if (var && location.at(i).index != "" && request->getURL() == "/")
		path.append("/" + location.at(i).index);
	if (!var && location.at(i).index != "")
		path.append("/" + location.at(i).index);
	else
	{
		for (size_t y = 0; y < url_vec.size(); y++)
			path.append("/" + url_vec.at(y));
	}
	autoindex = location.at(i).autoindex;
	redirection = location.at(i).redirections;
	return path;
}

// Generates a Header for the Response matching the parameter to
// the wanted code in StatusCode map
void Response::makeHeader(std::string &code_status)
{
	MapIterator it = status.code.find(code_status);
	std::stringstream s_header;

	if (code_status != "200" && code_status != "301")
	{
		errorBody(code_status);
	}
	s_header << "HTTP/1.1 " << it->first << it->second
			 << "\r\nContent-Length: " << bodySize;
	if (code_status == "301")
		s_header << "\r\nLocation: http://127.0.0.1:4242";
	s_header << "\r\nSet-Cookie:UserID = XYZ;"
			 << "\r\n\r\n";
	if (DEBUG)
		std::cout << s_header.str() << std::endl;
	headerSize = s_header.str().length();
	header = s_header.str();
}

// Return the sockets with the same server port that is not the one already in config OR return -1.
int Response::findSocket()
{
	for (size_t i = 0; i < server->sockets.size(); i++)
	{
		if (config.listen_port == server->sockets.at(i).getServInfo().listen_port && config.server_names != server->sockets.at(i).getServInfo().server_names)
			return i;
	}
	return -1;
}

void Response::handleCGI()
{
	CGI cgi(request, config);

	body.clear();
	bodySize = cgi.getCGIouput().length();
	body << cgi.getCGIouput();
	status_code = "200";
	makeHeader(status_code);
}
