#ifndef __CGI_H__
#define __CGI_H__

#include "defines.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <unistd.h>

#define N_ENV_VAR 14

class CGI {
	public:
		CGI() {}
		CGI(RequestParser& request, server_info& server);
		~CGI() {};
	
		std::string getCGIouput() { return output; }
	private:

		std::string output; // final result to be sent back to Response body field
		StringVector envVar; // CGI Environment Variables

		char* args[3]; // [0]/usr/bin/<language> [1]<ScriptPath>
		char* envp[15];

		char* findScriptType(RequestParser& request, server_info& server);
		void setEnvVariables(RequestParser& request, server_info& server);
		void setExecArgs(RequestParser& request, server_info& server);
		void convToCharPtr();
		void execCGI(/*???*/);

}; // CGI

#endif
