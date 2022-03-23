#ifndef __CGI_H__
#define __CGI_H__

#include "defines.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"
#include "utils.hpp"

#define N_ENV_VAR 14

class CGI {
	public:
		CGI() {}
		CGI(RequestParser& request, server_info& server);
		~CGI() {};
	
	private:
		StringVector envVar;
		char* env[15];

		void setEnvVariables(RequestParser& request, server_info& server);
		void convToCharPtr();

}; // CGI

#endif
