#include "Response.hpp"

Response::Response(RequestParser& request, server_info& config, short& status_code) : status_code(status_code)
{
	MethodType type = getType(request);

	switch(type){
		case GET:
			responseGET(request, config);
			break;
		/* case POST:
			responsePOST(request);
			break;
		case DELETE:
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

void Response::responseGET(RequestParser& request, server_info& config)
{
	if (request.getURL().find("/image") != std::string::npos ||
		request.getURL().find("favicon.ico") != std::string::npos)
		makeImage(request, config);
	else
	{
		LocationVector location = config.locations;
		std::string path;
	
		// Look for the exact path
		for (size_t i = 0; i < location.size(); i++){
			if (location.at(i).name == request.getURL()) {
				path = location.at(i).root;
				break;
			}
		}
		// Then check if its looking for the index
		if (request.getURL() ==  "/")
		{
			for (size_t i = 0; i < location.size(); i++){
				if (location.at(i).name == "/"){
					path.append("/");
					path.append(location.at(i).index);
					break;
				}
			}
		}
		// Then take the given path
		else
			path.append(config.root + request.getURL());
		readHTML(path);
		bodySize = body.str().length();
		content_type = "*/*";
	}
	makeHeader(status_code);

}

/* void Response::responsePOST()
{
	
}

void Response::responseDELETE() 
{
	
} */

// Generates a Header for the Response matching the parameter to
// the wanted code in StatusCode map
void Response::makeHeader(const short& code) 
{
	MapIterator it = status.code.find(code);
	std::stringstream s_header;

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
		status_code = 404;
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
void Response::makeImage(RequestParser& request, server_info& config) 
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
	
	if (!myfile.good())
		std::cout << logEvent("file cannot open!\n") << std::endl;

	while (getline(myfile, line))
		body << line << '\n';
}
