#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include "RequestParser.hpp"
#include "Server.hpp"
#include "status_codes.hpp"

#define MAX_IMAGESIZE 1000000

typedef enum MethodType {
	GET,
	POST,
	DELETE,
	NONE
} MethodType;

class Response {
public:
	Response() {}
	Response(RequestParser& request, server_info& config, short& status_code);
	~Response() {};

	std::string getResponseHeader() { return header; }
	std::string getResponseBody() { return body.str(); }
	size_t getHeaderSize() { return headerSize; }
	size_t getBodySize() { return bodySize; }

private:


	size_t headerSize;
	size_t bodySize;
	std::string header;
	std::string content_type;
	std::stringstream body;

	short status_code;

	typedef std::map<short, std::string>::iterator MapIterator;
	typedef std::vector<location_info>::iterator LocIterator;

	StatusCode status; // Status Code map

	MethodType getType(RequestParser& request);

	void responseGET(RequestParser& request, server_info& config);
	//void responsePOST(RequestParser& const request);
	//void responseDELETE(RequestParser& const request);
	void makeHeader(const short& code);
	void makeFavicon();
	void makeImage(RequestParser& request, server_info& config);
	void readHTML(std::string filepath);

	// Image Methods
	typedef std::pair<char *, std::streampos> ImgInfo;
	std::pair<char *, std::streampos> getImageBinary(const char* path);
	std::string findImagePath(LocationVector& location, RequestParser& request);
};

