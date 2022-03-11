#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include "RequestParser.hpp"
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
	Response(RequestParser& request);
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

	typedef std::map<std::string, std::string>::iterator MapIterator;
	StatusCode status;

	std::ifstream imgBin();
	MethodType getType(RequestParser& request);

	void responseGET(RequestParser& request);
	//void responsePOST(RequestParser& const request);
	//void responseDELETE(RequestParser& const request);
	void makeHeader(const std::string& code);
	void makeFavicon();
	void makeImage(RequestParser& request);
	void readHTML(std::string filepath);
};


