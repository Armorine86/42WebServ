#include "RequestParser.hpp"
#include "utils.hpp"

#include <string>

RequestParser::RequestParser(std::string &request){
	StringVector content = split(request, "\n");

	for (StringIterator it = content.begin(); it != content.end(); it++)	
		content.push_back(format_line(*it));
	if (content.empty()) {
		std::cerr << logEvent("[PARSE ERROR] Request is empty") << END << std::endl;
		exit(PARSING_ERROR);
	}
	RequestInfo(content);
}

void RequestParser::RequestInfo(StringVector& content){
	StringIterator start, end;

	start = content.begin();
	end = content.end();
	
	for (; start != end; start++) {
		if ((*start).find("GET") || (*start).find("POST") || (*start).find("DELETE"))
			ParseFirstLine(start);
		else if ((*start).find("Host:"))
			host = (*start).find_first_not_of(("Host: "));
		else if ((*start).find("User-Agent:"))
			user_agent = (*start).find_first_not_of(("User-Agent: "));
		else if ((*start).find("Accept:"))
			accept = split((*start), ",");
		else if ((*start).find("Accept-Language:"))
			language = split((*start), ",");
		else if ((*start).find("Accept-Charset:"))
			char_set = split((*start), ",");
		else if ((*start).find("Connection:")) {
			if ((*start).find("keep-alive")) {
				connection = true; }
			else {
				connection = false; }
		}
	}
}

//Parse the first line of the request
void RequestParser::ParseFirstLine(StringIterator& line){
	StringVector vec_str;
	StringIterator start, end;
	
	vec_str = split((*line), " ");
	start = vec_str.begin();
	end = vec_str.end();

	for (; start != end; start++) {
		if ((*start).find("GET") || (*start).find("POST") || (*start).find("DELETE")){
			if ((*start).find("GET"))
				method = "GET";
			else if((*start).find("POST"))
				method = "POST";
			else if((*start).find("DELETE"))
				method = "DELETE";
			else{
				std::cerr << logEvent("[405] Method not Allowed") << END << std::endl;
				return;
			}
		}
		else if((*start).at(0) == '/')
			url = *start;
		else if((*start).find("HTTP") && (*start) != "HTTP/1.1"){
			std::cerr << logEvent("[505] HTTP Version Not Supported") << END << std::endl;
			return;
		}
		else{
			std::cerr << logEvent("[400] Bad Request") << END << std::endl;
			return;
		}
	}
}
