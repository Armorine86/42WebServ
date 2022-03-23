#include "Response.hpp"

Response::Response( RequestParser& request, server_info& config, Server* server) : config(config), server(server), autoindex(false)
{
	MethodType type = getType(request);

	switch(type){
		case GET:
			responseGET(request);
			break;
		case POST:
			responsePOST(request);
			break;
		/*case DELETE:
			responseDELETE(request); 
			break;*/
		default:
			std::cerr << logEvent("Response: [405] Method not Allowed\n") << END << std::endl;
	}
}

MethodType Response::getType(RequestParser& request)
{
	std::string method = request.getMethod();

	if (method.find("GET") != std::string::npos)
		return GET;
	else if (method.find("POST") != std::string::npos)
		return POST;
	else if (method.find("DELETE") != std::string::npos)
		return DELETE;
	return NONE;
}

void Response::responseGET(RequestParser& request)
{
	if (request.getURL().find("cgi") != std::string::npos) {
		CGI cgi(request, config, server->sender_fd, server->pfds[0].fd);
		//! find a better way of passing the server FD    --^
		// TODO body = cgi.getOutput();
		makeHeader(server->status_code);
		return;
	}
	if (request.getURL().find("/image") != std::string::npos ||
		request.getURL().find("favicon.ico") != std::string::npos)
		makeImage(request);
	else
	{
		LocationVector location = config.locations;
		std::string path;
		int i = 0;
	
		path = lookForRoot(location, request);
		if (path == "")
			if((i = findSocket()) >= 0){
				LocationVector tmp_location = server->sockets.at(i).getServInfo().locations;
				path = lookForRoot(tmp_location, request);
				
				if (path != ""){
					std::pair<int, size_t>p1(server->sender_fd, i);
					server->server_index[p1.first] = p1.second;
				}
			}

		if (DEBUG)
			std::cout << BRED << path << END << std::endl;
		if (autoindex)
			makeAutoindex(path);
		else
			readHTML(path);
		content_type = "*/*";
	}
	if (bodySize > config.client_max_body_size)
		server->status_code = "413";
	makeHeader(server->status_code);
}

void Response::responsePOST(RequestParser &request)
{
	CGI cgi(request, config, server->sender_fd, server->pfds[0].fd);
	// TODO body = cgi.getOutput();
}

/*void Response::responseDELETE() 
{
	
} */

// Generates a Header for the Response matching the parameter to
// the wanted code in StatusCode map
void Response::makeHeader(std::string& code_status)
{
	MapIterator it = status.code.find(code_status);
	std::stringstream s_header;

	if (code_status != "200"){
		errorBody(code_status);
	}
	s_header << "HTTP/1.1 " << (*it).first << (*it).second << "\r\n"
	<< "Content-type: " << content_type
	<< "\r\nContent-Length: " << bodySize << "\r\n\r\n";

	headerSize = s_header.str().length();
	header = s_header.str();
}

// Retrieves the requested image binary and sets the informations in a pair
std::pair<char *, std::streampos> Response::getImageBinary(const char* path)
{
	ImgInfo img_info;
	std::ifstream f(path, std::ios::in|std::ios::binary|std::ios::ate);
	
	if (!f.is_open()) { // if file not found
		server->status_code = "404";
		img_info.first = NULL;
		return img_info;
	}

	img_info.second = f.tellg();

	img_info.first = new char [static_cast<long>(img_info.second)];

	f.seekg (0, std::ios::beg);
	f.read (img_info.first, img_info.second);
	f.close();

	return img_info;
}

// Retrieves the requested image path in the directories
std::string Response::findImagePath(LocationVector& location, RequestParser& request)
{
	std::string path;

	for (size_t i = 0; i < location.size(); i++){
		if (location.at(i).name.find("/image") != std::string::npos){
			path = location.at(i).root;
			break;
		}
	}

	if (request.getURL().find("favicon.ico") != std::string::npos)
		path.append("/images");

	path.append(request.getURL());

	return path;
}

// Find the location of the requested image
// Gets the Binary
// Writes the content to the Response body
void Response::makeImage(RequestParser& request) 
{
	LocationVector location = config.locations;

	std::string img_path(findImagePath(location, request));

	ImgInfo img = getImageBinary(img_path.c_str());

	body.write(img.first, img.second);
	bodySize = img.second;
	content_type = "image/*";

	delete[] img.first;
}

// Used to respond with passed HTML file 
void Response::readHTML(std::string filepath) 
{
	std::string line;
	std::fstream myfile;
	
	myfile.open(filepath.c_str(), std::ios::in);
	
	if (!myfile.good()){
		server->status_code = "404";
		if (DEBUG)
			std::cout << logEvent("file cannot open!\n") << std::endl;
		return;
	}
	server->status_code = "200";
	body.clear();
	while (getline(myfile, line))
		body << line << '\n';
	bodySize = body.str().length();
}

void Response::makeAutoindex(std::string path) 
{
	DIR *dir;
	dirent *fic;
	std::string	 line, value;

	dir = opendir(path.c_str());
	if (!dir){
		server->status_code = "404";
		std::cout << BRED << "Autoindex error" << END << std::endl;
		return; 
	}
	server->status_code = "200";
	value.assign("<html>\n<head>\n<meta charset=\"utf-8\">\n"
			"<title>Directory Listing</title>\n</head>\n<body>\n<h1>"
			+ path + "</h1>\n<ul>");
	while ((fic = readdir(dir)) != NULL)
	{
		value.append("<li><a href=\"");
		value.append(path);
		if (value[value.size() - 1] != '/')
			value.append("/");
		value.append(fic->d_name);
		if(fic->d_type == DT_DIR)
			value.append("/");
		value.append("\"> ");
		value.append(fic->d_name);
		if(fic->d_type == DT_DIR)
			value.append("/");
		value.append("</a></li>\n");
	}
	value.append("</ul></body></html>");
	closedir(dir);

	body.clear();
	body << value;
	bodySize = body.str().length();
}

int Response::findSocket()
{
	for (size_t i = 0; i < server->sockets.size(); i++) {
		if (config.listen_port == server->sockets.at(i).getServInfo().listen_port
			&& config.server_names != server->sockets.at(i).getServInfo().server_names)
			return i;
	}
	return -1;
}

std::string Response::lookForRoot(LocationVector& location, RequestParser& request) 
{
	std::string path = "";

	for (size_t i = 0; i < location.size(); i++){
		if (location.at(i).name == request.getURL()) {
			path = location.at(i).root;
			autoindex = location.at(i).autoindex;
			if (location.at(i).index != "")
				path.append("/" + location.at(i).index);
			break;
		}
	}
	if (path == "")
		path = lookForContent(location, request);

	return path;
}

std::string Response::lookForContent(LocationVector& location, RequestParser& request) 
{
	std::string path = "";
	StringVector tmp = split(request.getURL(), "/");

	if (!tmp.empty()){
		for (size_t i = 0; i < location.size(); i++){
			if (location.at(i).name.find(tmp[0]) != std::string::npos){
				path = location.at(i).root;
				autoindex = location.at(i).autoindex;
				path.append(request.getURL());
				break;
			}
		}
	}
	return path;
}



