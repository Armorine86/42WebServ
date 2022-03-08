#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include "RequestParser.hpp"

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
	std::string header;
	std::stringstream body;
	size_t headerSize;
	size_t bodySize;

	std::ifstream imgBin();
	MethodType getType(RequestParser& request);

	void responseGET(RequestParser& request);
	//void responsePOST(RequestParser& const request);
	//void responseDELETE(RequestParser& const request);
};


