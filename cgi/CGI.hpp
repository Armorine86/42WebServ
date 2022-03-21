#pragma once

#include "defines.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"

#define N_ENV_VAR 14

class CGI {
	public:
		CGI() {}
		CGI(RequestParser& request, server_info& server);
		~CGI() {};
	
	private:
		std::string url;
		StringVector body;
		StringVector envVar;

		char* env[15];

		void setEnvVariables();
		void convToCharPtr();

}; // CGI
