#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include "defines.hpp"
#include <sstream>
#include <fstream>
#include "RequestParser.hpp"
#include "status_codes.hpp"
#include "Server.hpp"
#include "CGI.hpp"

#define MAX_IMAGESIZE 1000000

typedef enum MethodType {
	GET,
	POST,
	DELETE,
	NONE
} MethodType;

class Server;

class Response : public Server{

public:
	Response() {}
	Response(RequestParser& request, server_info& config, Server* server);
	~Response() {};

	std::string getResponseHeader() { return header; }
	std::string getResponseBody() { return body.str(); }
	size_t getHeaderSize() { return headerSize; }
	size_t getBodySize() { return bodySize; }

private:
	server_info config;
	Server *server;
	size_t headerSize;
	size_t bodySize;
	std::string header;
	std::string content_type;
	std::stringstream body;
	// ServerIndex server_index;
	StatusCode status; // Status Code map

	typedef std::map<std::string, std::string>::iterator MapIterator;
	typedef std::vector<location_info>::iterator LocIterator;

	MethodType getType(RequestParser& request);

	void responseGET(RequestParser& request);
	void responsePOST(RequestParser& request);
	//void responseDELETE(RequestParser& const request);
	void makeHeader(std::string& code);
	void makeFavicon();
	void makeImage(RequestParser& request);
	void readHTML(std::string filepath);
	int findSocket();
	std::string lookForRoot(LocationVector& location, RequestParser& request);
	std::string lookForContent(LocationVector& location, RequestParser& request);

	// Image Methods
	typedef std::pair<char *, std::streampos> ImgInfo;
	std::pair<char *, std::streampos> getImageBinary(const char* path);
	std::string findImagePath(LocationVector& location, RequestParser& request);

	// Error
	void errorBody(std::string& code);
};


#endif // __RESPONSE_H__
