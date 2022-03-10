#pragma once

#include <map>
#include <string>

#define INSERT codes.insert(std::make_pair<std::string, std::string>

typedef struct StatusCode {

	std::map<std::string, std::string> codes;

	StatusCode() {
		INSERT("100", " Continue"));
		INSERT("101", " Switching Protocols"));
		INSERT("102", " Processing"));
		INSERT("200", " Ok"));
		INSERT("201", " Created"));
		INSERT("202", " Accepted"));
		INSERT("203", " Non-Authoritative Information"));
		INSERT("204", " No Content"));
		INSERT("205", " Reset Content"));
		INSERT("206", " Partial Content"));
		INSERT("207", " Multi-Status"));
		INSERT("208", " Already Reported"));
		INSERT("226", " IM Used"));
		INSERT("300", " Multiple Choices"));
		INSERT("301", " Moved Permanently"));
		INSERT("302", " Found"));
		INSERT("303", " See Other"));
		INSERT("304", " Not Modified"));
		INSERT("305", " Use Proxy"));
		INSERT("307", " Temporary Redirect"));
		INSERT("308", " Permanent Redirect"));
		INSERT("400", " Bad Request"));
		INSERT("401", " Unauthorized"));
		INSERT("402", " Payment Required"));
		INSERT("403", " Forbidden"));
		INSERT("404", " Page not found"));
		INSERT("405", " Method Not Allowed"));
		INSERT("406", " Not Acceptable"));
		INSERT("407", " Proxy Authentification Required"));
		INSERT("408", " Request Timeout"));
		INSERT("409", " Conflict"));
		INSERT("410", " Gone"));
		INSERT("411", " Length Required"));
		INSERT("412", " Precondition Failed"));
		INSERT("413", " Request Entity Too Large"));
		INSERT("414", " Request URL Too Long"));
		INSERT("415", " Unsupported Media Type"));
		INSERT("416", " Requested Range not Satisfiable"));
		INSERT("417", " Expectation Failed"));
		INSERT("418", " I'm a Teapot"));
		INSERT("420", " Enhance Your Calm"));
		INSERT("422", " Unprocessable Entity"));
		INSERT("423", " Locked"));
		INSERT("424", " Failed Dependency"));
		INSERT("426", " Upgrade Required"));
		INSERT("428", " Precondition Required"));
		INSERT("429", " Too Many Request"));
		INSERT("431", " Request Header Fields Too Large"));
		INSERT("444", " No Response"));
		INSERT("449", " Retry With"));
		INSERT("450", " Blocked By Windows Parental Control"));
		INSERT("451", " Unavailable for Legal Reasons"));
		INSERT("499", " Client Closed Request"));
		INSERT("500", " Internal Server Error"));
		INSERT("501", " Not Implemented"));
		INSERT("502", " Bad Gateway"));
		INSERT("503", " Service Unavailable"));
		INSERT("504", " Gateway Timeout"));
		INSERT("505", " HTTP Version Not Supported"));
		INSERT("506", " Variant Also Negociates"));
		INSERT("507", " Insufficient Storage"));
		INSERT("508", " Loop Detected"));
		INSERT("509", " Bandwidth Limit Exceeded"));
		INSERT("510", " Not Extended"));
		INSERT("511", " Network Authentification Required"));
		INSERT("598", " Network Read Timeout Error"));
		INSERT("599", " Network Connect Timeout Error"));
	}
} StatusCode;
