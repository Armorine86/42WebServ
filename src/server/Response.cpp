#include "Response.hpp"

Response::Response(RequestParser& request) 
{

	MethodType type = getType(request);

	switch(type){
		case GET:
			responseGET(request);
			break;
		/* case POST:
			responsePOST(request);
			break;
		case DELETE:
			responseDELETE(request); 
			break;*/
		default:
			std::cerr << logEvent("Response: [405] Method not Allowed") << END << std::endl;
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
	//if it's got a dot in the url FOR NOW it will think it's a image
	if (request.getURL().find(".") != std::string::npos)
	{
		if (request.getURL().find("favicon") != std::string::npos)
			makeFavicon();
		else
			makeImage(request);
	}
	else
	{
		//va falloir que le pathfile soit adapte selon la request... o_O
		readHTML("resources/index.html");
		//readHTML("resources/error/error403.html");
		bodySize = body.str().length();
		content_type = "text/html";
	}
	makeHeader();
}

/* void Response::responsePOST()
{
	
}

void Response::responseDELETE() 
{
	
} */

// Generates a Header for the Response
void Response::makeHeader() 
{
	std::stringstream s_header;

	s_header << "HTTP/1.1 200 OK\r\n"
	<< "Content-type: " << content_type
	<< "\r\nContent-Length: " << bodySize << "\r\n\r\n";

	headerSize = s_header.str().length();
	header = s_header.str();
}

void Response::makeFavicon() 
{
	std::ifstream f("resources/favicon.ico", std::ios::in|std::ios::binary|std::ios::ate);

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
	content_type = "image/x-icon";		
	delete[] image;
}

void Response::makeImage(RequestParser& request) 
{
	std::ifstream f(request.getURL().erase(0,1).c_str(), std::ios::in|std::ios::binary|std::ios::ate);

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

void Response::readHTML(std::string filepath) 
{
	std::string line;
	std::fstream myfile;
	myfile.open(filepath.c_str(), std::ios::in);
	if (!myfile.good())
		std::cout << "file cannot open!";
	while (getline(myfile, line))
		body << line << '\n';
}
