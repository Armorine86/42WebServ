#ifndef __CGI_H__
#define __CGI_H__

#include "defines.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <algorithm>

#define N_ENV_VAR 14
#define READ 0
#define WRITE 1

#define BUFFER_SIZE 570

class CGI {
	public:
		CGI() {}
		CGI(RequestParser& request, server_info& server);
		~CGI() {};
	
		std::string getCGIouput() { return output; }
	private:

		RequestParser req;
		int fd_pipe[2];
		
		
		std::string output; // final result to be sent back to Response body field
		StringVector envVar; // CGI Environment Variables


		StringVector argv;
		char* args[3]; // [0]/usr/bin/<language> [1]<ScriptPath>
		char* envp[N_ENV_VAR + 1];

		char* findScriptType(server_info& server);
		void setEnvVariables(server_info& server);
		void setExecArgs(server_info& server);
		void convToCharPtr();
		void execCGI(server_info& server);
		void execScript(char** argv);

		void readFromChild();

		void createPipe();
		void cleanPipes();

}; // CGI

#endif
