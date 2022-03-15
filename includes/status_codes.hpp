#pragma once

#include <map>
#include <string>

#define INSERT_CODE code.insert(std::pair<short, std::string>

typedef struct StatusCode {
	
	std::map<short, std::string> code;

	StatusCode() {
		INSERT_CODE(100, " Continue"));
		INSERT_CODE(101, " Switching Protocols"));
		INSERT_CODE(102, " Processing"));
		INSERT_CODE(200, " Ok"));
		INSERT_CODE(201, " Created"));
		INSERT_CODE(202, " Accepted"));
		INSERT_CODE(203, " Non-Authoritative Information"));
		INSERT_CODE(204, " No Content"));
		INSERT_CODE(205, " Reset Content"));
		INSERT_CODE(206, " Partial Content"));
		INSERT_CODE(207, " Multi-Status"));
		INSERT_CODE(208, " Already Reported"));
		INSERT_CODE(226, " IM Used"));
		INSERT_CODE(300, " Multiple Choices"));
		INSERT_CODE(301, " Moved Permanently"));
		INSERT_CODE(302, " Found"));
		INSERT_CODE(303, " See Other"));
		INSERT_CODE(304, " Not Modified"));
		INSERT_CODE(305, " Use Proxy"));
		INSERT_CODE(307, " Temporary Redirect"));
		INSERT_CODE(308, " Permanent Redirect"));
		INSERT_CODE(400, " Bad Request"));
		INSERT_CODE(401, " Unauthorized"));
		INSERT_CODE(402, " Payment Required"));
		INSERT_CODE(403, " Forbidden"));
		INSERT_CODE(404, " Page not found"));
		INSERT_CODE(405, " Method Not Allowed"));
		INSERT_CODE(406, " Not Acceptable"));
		INSERT_CODE(407, " Proxy Authentification Required"));
		INSERT_CODE(408, " Request Timeout"));
		INSERT_CODE(409, " Conflict"));
		INSERT_CODE(410, " Gone"));
		INSERT_CODE(411, " Length Required"));
		INSERT_CODE(412, " Precondition Failed"));
		INSERT_CODE(413, " Request Entity Too Large"));
		INSERT_CODE(414, " Request URL Too Long"));
		INSERT_CODE(415, " Unsupported Media Type"));
		INSERT_CODE(416, " Requested Range not Satisfiable"));
		INSERT_CODE(417, " Expectation Failed"));
		INSERT_CODE(418, " I'm a Teapot"));
		INSERT_CODE(420, " Enhance Your Calm"));
		INSERT_CODE(422, " Unprocessable Entity"));
		INSERT_CODE(423, " Locked"));
		INSERT_CODE(424, " Failed Dependency"));
		INSERT_CODE(426, " Upgrade Required"));
		INSERT_CODE(428, " Precondition Required"));
		INSERT_CODE(429, " Too Many Request"));
		INSERT_CODE(431, " Request Header Fields Too Large"));
		INSERT_CODE(444, " No Response"));
		INSERT_CODE(449, " Retry With"));
		INSERT_CODE(450, " Blocked By Windows Parental Control"));
		INSERT_CODE(451, " Unavailable for Legal Reasons"));
		INSERT_CODE(499, " Client Closed Request"));
		INSERT_CODE(500, " Internal Server Error"));
		INSERT_CODE(501, " Not Implemented"));
		INSERT_CODE(502, " Bad Gateway"));
		INSERT_CODE(503, " Service Unavailable"));
		INSERT_CODE(504, " Gateway Timeout"));
		INSERT_CODE(505, " HTTP Version Not Supported"));
		INSERT_CODE(506, " Variant Also Negociates"));
		INSERT_CODE(507, " Insufficient Storage"));
		INSERT_CODE(508, " Loop Detected"));
		INSERT_CODE(509, " Bandwidth Limit Exceeded"));
		INSERT_CODE(510, " Not Extended"));
		INSERT_CODE(511, " Network Authentification Required"));
		INSERT_CODE(598, " Network Read Timeout Error"));
		INSERT_CODE(599, " Network Connect Timeout Error"));
	}
} StatusCode;
