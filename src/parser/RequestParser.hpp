#pragma once

#include "defines.hpp"
#include <iostream>

// Header containing all the information of the Client Request to the Server
class RequestParser {

public:
	RequestParser();
	RequestParser(std::string &request);
	RequestParser(const RequestParser& src) { *this = src; }
	~RequestParser();




private:
	std::string method; // Method
	std::string url;	// URL
	std::string host; // DNS of the Server. In our case: LOCALHOST
	//std::string protocol; // HTTP/1.1
	std::string user_agent;	// Browser
	StringVector accept;  // type of request.  i.e: text/html
	StringVector language; // en-US || en-CA || etc.
	StringVector char_set; // UTF-8
	bool connection;  // Keep-Alive || Close

	StringVector body; // Optional. Used for POST request
	
	// typedef std::vector<std::string>::iterator RequestIterator;

	void RequestInfo(StringVector& content);
	void ParseFirstLine(StringIterator& line); //Method, URL, Protocol
};