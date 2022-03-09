#include "Response.hpp"

// ****** FIRST REQUEST ******

// GET / HTTP/1.1
// Host: 127.0.0.1:8081
// User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:97.0) Gecko/20100101 Firefox/97.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
// Accept-Language: en-CA,en-US;q=0.7,en;q=0.3
// Accept-Encoding: gzip, deflate
// Connection: keep-alive
// Upgrade-Insecure-Requests: 1
// Sec-Fetch-Dest: document
// Sec-Fetch-Mode: navigate
// Sec-Fetch-Site: none
// Sec-Fetch-User: ?1


// ******* IMAGE REQUEST ***

// GET /Surfer_Girl.jpg HTTP/1.1
// Host: 127.0.0.1:8081
// User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:97.0) Gecko/20100101 Firefox/97.0
// Accept: image/avif,image/webp,*/*
// Accept-Language: en-CA,en-US;q=0.7,en;q=0.3
// Accept-Encoding: gzip, deflate
// Connection: keep-alive
// Referer: http://127.0.0.1:8081/
// Sec-Fetch-Dest: image
// Sec-Fetch-Mode: no-cors
// Sec-Fetch-Site: same-origin

// ode: navigate
// Sec-Fetch-Site: none
// Sec-Fetch-User: ?1


// ******* FAVICON REQUEST ***

// GET /favicon.ico HTTP/1.1
// Host: localhost:8081
// Connection: keep-alive
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36
// Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
// Sec-GPC: 1
// Sec-Fetch-Site: same-origin
// Sec-Fetch-Mode: no-cors
// Sec-Fetch-Dest: image
// Referer: http://localhost:8081/
// Accept-Encoding: gzip, deflate, br
// Accept-Language: en-GB,en-US;q=0.9,en;q=0.8

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
	std::string hardcode = "<!DOCTYPE html>\
<html>\
<head>\
<!-- HTML -->\
<title>\
Les surfeurs du web</title>\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
<style>\
body {background-color:#f55fff;background-image:url(/Surfer_Girl.jpg);background-repeat:repeat;background-position:top left;background-attachment:scroll;}\
h1{font-family:Impact, sans-serif;color:#000000;background-color:#ffffff;}\
p {font-family:Georgia, serif;font-size:14px;font-style:normal;font-weight:normal;color:#000000;background-color:#ffffff;}\
</style>\
</head>\
<body>\
<h1>Yo men bienvenue sur notre site trop swag!</h1>\
<p>Random text bla bla bla</p>\
<img src=\"Surfer_Girl.jpg\" alt=\"\">\
</body>\
</html>";

	if (request.getURL().find("Surfer_Girl") != std::string::npos)
	{
		std::ifstream f("Surfer_Girl.jpg", std::ios::in|std::ios::binary|std::ios::ate);
		if(!f.is_open()) perror ("bloody file is nowhere to be found. Call the cops");
		std::streampos ssize = f.tellg();
		char* image = new char [static_cast<long>(ssize)];
		f.seekg (0, std::ios::beg);
		f.read (image, ssize);
		f.close();

		body.write(image, ssize);
		bodySize = ssize;
		content_type = "image/*";
		makeHeader();
		
		delete[] image;
	} 
	else if (request.getURL().find("favicon.ico") != std::string::npos)
	{
		std::ifstream f("favicon.ico", std::ios::in|std::ios::binary|std::ios::ate);
		if(!f.is_open()) perror ("bloody file is nowhere to be found. Call the cops");
		std::streampos ssize = f.tellg();
		char* image = new char [static_cast<long>(ssize)];
		f.seekg (0, std::ios::beg);
		f.read (image, ssize);
		f.close();

		body.write(image, ssize);
		bodySize = ssize;
		content_type = "image/*";
		makeHeader();
		
		delete[] image;
	}
	else
	{
		body << hardcode;
		bodySize = body.str().length();
		content_type = "text/html";
		makeHeader();
	}
}

/* void Response::responsePOST()
{
	
}

void Response::responseDELETE() 
{
	
} */

void Response::makeHeader() 
{
	std::stringstream s_header;

	s_header << "HTTP/1.1 200 OK\r\n"
	<< "Content-type: " << content_type
	<< "\r\nContent-Length: " << bodySize << "\r\n\r\n";

	headerSize = s_header.str().length();
	header = s_header.str();
}
