#ifndef __REQUESTPARSER_H__
#define __REQUESTPARSER_H__

#include "defines.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>

// Header containing all the information of the Client Request to the Server
class RequestParser
{

public:
	RequestParser(){};
	RequestParser(std::string &request);
	RequestParser(const RequestParser &src) { *this = src; }
	~RequestParser(){};

	bool isUpload() { return isCGIUpload; }
	bool isCGIRequest() { return cgiRequest; }
	std::string getMethod(void) { return method; }
	std::string getURL(void) { return url; }
	std::string getHost(void) { return host; }
	std::string getBody(void) { return body; }
	std::string getContentType(void) { return contentType; }
	std::string getQuery(void) { return QueryString; }
	std::string getScriptName(void) { return scriptName; }
	std::string getScriptPath(void) { return scriptPath; }
	std::string getScriptType(void) { return scriptType; }
	StringVector getAccept(void) { return accept; }

private:
	std::string	scriptType; // .py || .pl || .php
	std::string method;		// GET || POST || DELETE	
	std::string url;		
	std::string host;		// DNS of the Server. In our case: LOCALHOST
	std::string user_agent; // Browser
	std::string contentType;
	std::string body; 		// Optional. Used for POST request
	std::string QueryString;
	std::string scriptName;
	std::string scriptPath; //! Try char * directly
	StringVector accept;	// type of request.  i.e: text/html
	StringVector language;	// en-US || en-CA || etc.
	StringVector char_set;	// UTF-8
	bool connection;		// Keep-Alive || Close
	bool cgiRequest;
	bool isCGIUpload;

	void cgiEnvGet(StringIterator& start);
	void cgiEnvPOST(StringIterator& start, StringVector& vec_str);
	void RequestInfo(StringVector &content);
	void ParseFirstLine(StringIterator &line); // Method, URL, Protocol
	std::string findScriptType(std::string& line);
};

#endif // __REQUESTPARSER_H__
