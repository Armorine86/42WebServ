#include "RequestParser.hpp"

RequestParser::RequestParser(std::string &request){
	StringVector content = split(request, "\r\n");

	if (content.empty()) {
		std::cerr << logEvent("[PARSE ERROR] Request is empty\n") << END << std::endl;
		throw std::runtime_error("Empty Request");
	}
	RequestInfo(content);
}

// Populate all the relevant variables while parsing the request header
// line by line
void RequestParser::RequestInfo(StringVector& content){
	StringIterator start, end;

	start = content.begin();
	end = content.end();
	
	for (; start != end; start++) {
		if ((*start).find("GET") != std::string::npos 
			|| (*start).find("POST") != std::string::npos 
			|| (*start).find("DELETE") != std::string::npos)
			ParseFirstLine(start);
		else if ((*start).find("Host:") != std::string::npos)
			host = (*start).erase(0,6);
		else if ((*start).find("User-Agent:") != std::string::npos)
			user_agent = (*start).erase(0, 12);
		else if ((*start).find("Accept:") != std::string::npos)
			accept = split((*start).erase(0,8), ",");
		else if ((*start).find("Accept-Language:") != std::string::npos)
			language = split((*start).erase(0,17), ",");
		else if ((*start).find("Accept-Charset:") != std::string::npos)
			char_set = split((*start), ",");
		else if ((*start).find("Content-Type:") != std::string::npos)
			contentType = (*start).erase(0, strlen("Content-Type: "));
		else if ((*start).find("Connection:") != std::string::npos) {
			if ((*start).find("keep-alive") != std::string::npos) {
				connection = true; }
			else {
				connection = false; }
		}
	}
	if (method == "POST") {
		body.append(*(start - 1)); // Adds the request Body if the method is POST
		QueryString.append(body);
	}
}

// Collect infos on the first line of the Request Header
void RequestParser::ParseFirstLine(StringIterator& line){
	StringVector vec_str;
	StringIterator start, end;
	
	vec_str = split((*line), " ");
	start = vec_str.begin();
	end = vec_str.end();

	for (; start != end; start++)
	{
		if ((*start).find("cgi") != std::string::npos) {
			if ((*start).find("?") != std::string::npos) {
				url = *start;
				StringVector tmp = split((*start), "?");
				scriptPath.append("./src/");
				scriptPath.append(tmp[0]);
				QueryString = tmp[1];
				tmp = split((*start), "/");
				scriptName = tmp[1].erase(tmp[1].find("?"), tmp[1].length());
				scriptType = findScriptType((*start));
				continue;
			}
			scriptPath.append("./src");
			scriptPath.append(vec_str[1]);
			StringVector tmp = split((*start), "/");
			scriptName = tmp[1];
			scriptType = findScriptType((*start)); 
		}
		else if ((*start).find("GET") != std::string::npos
			|| (*start).find("POST") != std::string::npos
			|| (*start).find("DELETE") != std::string::npos){
			if ((*start).find("GET") != std::string::npos)
				method = "GET";
			else if((*start).find("POST") != std::string::npos)
				method = "POST";
			else if((*start).find("DELETE") != std::string::npos)
				method = "DELETE";
			else{
				std::cerr << logEvent("[405] Method not Allowed\n") << END << std::endl;
				return;
			}
		}
		else if((*start).at(0) == '/') {
			url = *start;
		}
		else if((*start).find("HTTP") != std::string::npos) {
			if ((*start).find("HTTP/1.1") == std::string::npos) {
				std::cerr << logEvent("[505] HTTP Version Not Supported\n") << END << std::endl;
				return;
			}
		}
		else if ((*start).find("?") != std::string::npos) {
			StringVector tmp = split((*start), "?");
			QueryString = tmp[1];
		}
		else{
			std::cerr << logEvent("[400] Bad Request\n") << END << std::endl;
			return;
		}
	}
}

std::string RequestParser::findScriptType(std::string& line)
{
	if (line.find(".py") != std::string::npos)
		return ".py";
	else if (line.find(".pl") != std::string::npos)
		return ".pl";
	else if (line.find(".php") != std::string::npos)
		return ".php";
	return "";
}
