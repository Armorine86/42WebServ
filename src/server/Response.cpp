#include "Response.hpp"

Response::Response( RequestParser* request, Server* server) : 
request(request), server(server), autoindex(false), status_code(server->status_code)
{
	setConfig();
	path = lookForRoot(config.locations);
	
	MethodType type = getType();
	switch(type){
		case GET:
			responseGET();
			break;
		case POST:
			responsePOST();
			break;
		/*case DELETE:
			responseDELETE(); 
			break;*/
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

//This find and set in the map the right config
void Response::setConfig() 
{
	std::string host = request->getHost();
	StringVector host_vec = split(host, ":");
	int port = atoi(host_vec[1].c_str());
	
	for (size_t i = 0; i < server->sockets.size(); i++){
		server_info tmp = server->sockets.at(i).getServInfo();
		if (host_vec[0] == tmp.host && port == tmp.listen_port){
			config = tmp;
			break;
		}
	}
	if (lookForRoot(config.locations) == ""){
		int i = -1;
		if((i = findSocket()) >= 0){
			LocationVector tmp_location = server->sockets.at(i).getServInfo().locations;
			if (lookForRoot(tmp_location) != ""){
				config = server->sockets.at(i).getServInfo();
			}
			else
				status_code = "404";
		}
	}
}

void Response::responseGET()
{
	if (DEBUG)
		std::cout << BRED << path << END << std::endl;
	//* TODO Implement CGI scripting for Get Method
	if (request->getURL().find("/image") != std::string::npos ||
		request->getURL().find("favicon.ico") != std::string::npos)
		makeImage();
	else
	{
		if (autoindex)
			makeAutoindex(path);
		else
			readHTML(path);
	}
	if (bodySize > config.client_max_body_size)
		status_code = "413";
	makeHeader(status_code);
}

void Response::responsePOST()
{
	CGI cgi(request, config);
}

/*void Response::responseDELETE() 
{
	
} */

std::string Response::lookForRoot(LocationVector& location) 
{
	std::string path = "";
	StringVector url_vec = split(request->getURL(), "/");

	for (size_t i = 0; i < location.size() && path == ""; i++){
		if (!url_vec.empty() && location.at(i).name == "/" + url_vec[0]){
			path = setPath(location, url_vec, i, false);
		}
		if (location.at(i).name == "/"){
			path = setPath(location, url_vec, i, true);
		}
		if (!is_valid(path))
			path = "";
	}
	return path;
}

std::string Response::setPath(LocationVector& location, StringVector& url_vec, size_t i, bool var)
{
	std::string path = "";

	path = location.at(i).root;
	if (var && location.at(i).index != "" && request->getURL() == "/")
		path.append("/" + location.at(i).index);
	if (!var && location.at(i).index != "")
		path.append("/" + location.at(i).index);
	else{
		for (size_t y = 0; y < url_vec.size(); y++)
			path.append("/" + url_vec.at(y));
	}
	autoindex = location.at(i).autoindex;
	return path;
}

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
		status_code = "404";
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

// Find the location of the requested image
// Gets the Binary
// Writes the content to the Response body
void Response::makeImage() 
{
	LocationVector location = config.locations;

	ImgInfo img = getImageBinary(path.c_str());

	body.write(img.first, img.second);
	bodySize = img.second;
	content_type = "image/*";
	status_code = "200";

	delete[] img.first;
}

// Used to respond with passed HTML file 
void Response::readHTML(std::string filepath) 
{
	std::string line;
	std::fstream myfile;
	
	myfile.open(filepath.c_str(), std::ios::in);
	
	if (!myfile.good()){
		status_code = "404";
		if (DEBUG)
			std::cout << logEvent("file cannot open!\n") << std::endl;
		return;
	}
	status_code = "200";
	body.clear();
	while (getline(myfile, line))
		body << line << '\n';
	bodySize = body.str().length();
	content_type = "*/*";
}

void Response::makeAutoindex(std::string path) 
{
	DIR *dir;
	dirent *dirent;
	std::string	 line, value;

	if (!ends_with(path, "upload")){
		makeImage();
		return;
	}
	dir = opendir(path.c_str());
	if (!dir){
		status_code = "404";
		std::cout << BRED << "Autoindex error" << END << std::endl;
		return; 
	}
	status_code = "200";
	value.assign("<html>\n<head>\n<meta charset=\"utf-8\">\n"
			"<title>Directory Listing</title>\n</head>\n<body>\n<h1>"
			+ path + "</h1>\n<ul>");
	while ((dirent = readdir(dir)) != NULL)
	{
		value.append("<li><a href=\"");
		value.append(path);
		if (value[value.size() - 1] != '/')
			value.append("/");
		value.append(dirent->d_name);
		if(dirent->d_type == DT_DIR)
			value.append("/");
		value.append("\"> ");
		value.append(dirent->d_name);
		if(dirent->d_type == DT_DIR)
			value.append("/");
		value.append("</a></li>\n");
	}
	value.append("</ul></body></html>");
	closedir(dir);

	body.clear();
	body << value;
	bodySize = body.str().length();
}

// Return the sockets with the same server port that is not the one already in config OR return -1.
int Response::findSocket()
{
	for (size_t i = 0; i < server->sockets.size(); i++) {
		if (config.listen_port == server->sockets.at(i).getServInfo().listen_port
			&& config.server_names != server->sockets.at(i).getServInfo().server_names)
			return i;
	}
	return -1;
}
