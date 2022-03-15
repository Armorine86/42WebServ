#include "Response.hpp"

Response::Response(RequestParser& request, server_info& config) 
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
	if (request.getURL().find("/images") != std::string::npos)
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
		// Then look if its looking for the index
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
	makeHeader("200");
}

/* void Response::responsePOST()
{
	
}

void Response::responseDELETE() 
{
	
} */

// Generates a Header for the Response matching the parameter to
// the wanted code in StatusCode map
void Response::makeHeader(const std::string& code) 
{
	MapIterator it = status.code.find(code);
	std::stringstream s_header;

	s_header << "HTTP/1.1 " << (*it).first << (*it).second << "\r\n"
	<< "Content-type: " << content_type
	<< "\r\nContent-Length: " << bodySize << "\r\n\r\n";

	headerSize = s_header.str().length();
	header = s_header.str();
}

// Retrieves the image binary
void Response::makeImage(RequestParser& request, server_info& config) 
{
	LocationVector location = config.locations;
	std::string path;
	
	for (size_t i = 0; i < location.size(); i++){
		if (location.at(i).name.find("/image") != std::string::npos){
			path = location.at(i).root;
			break;
		}
	}
	path.append(request.getURL());
	std::ifstream f(path.c_str(), std::ios::in|std::ios::binary|std::ios::ate);

	if(!f.is_open()){
		perror ("bloody file is nowhere to be found. Call the cops");
		return;
	}

	std::streampos ssize = f.tellg();
	char* image = new char [static_cast<long>(ssize)];
	f.seekg (0, std::ios::beg);
	f.read (image, ssize);
	f.close();

	body.write(image, ssize);
	bodySize = ssize;
	content_type = "image/*";
	delete[] image;
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
