#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include "RequestParser.hpp"

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

	std::string getResponse() { return response.str(); }
	size_t getSize() { return size; }

private:
	std::stringstream response;
	size_t size;

	std::ifstream imgBin();
	MethodType getType(RequestParser& request);

	void responseGET(RequestParser& request);
	//void responsePOST(RequestParser& const request);
	//void responseDELETE(RequestParser& const request);
};


