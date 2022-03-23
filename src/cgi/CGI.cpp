#include "CGI.hpp"

CGI::CGI(RequestParser& request, server_info& server)
{
	(void)server;
	bzero(env, sizeof(env));

	setEnvVariables(request, server);
	convToCharPtr();
}

void CGI::setEnvVariables(RequestParser& request, server_info& server)
{
	envVar.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envVar.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVar.push_back("SERVER_SOFTWARE=webserv");
	envVar.push_back("REDIRECT_STATUS=200"); // used for PHP
	envVar.push_back("SERVER_NAME=" + server.server_names);
	envVar.push_back("SERVER_PORT=" + IntToString(server.listen_port));
	envVar.push_back("REQUEST_METHOD=" + request.getMethod());
	envVar.push_back("PATH_INFO=" + request.getScriptPath());
	envVar.push_back("SCRIPT_NAME=" + request.getScriptName());
	envVar.push_back("QUERY_STRING=" + request.getQuery());
	envVar.push_back("REMOTE_ADDR=" + server.host);
	envVar.push_back("PATH_TRANSLATED=" + request.getScriptPath());
	envVar.push_back("CONTENT_TYPE=" + request.getContentType());
	envVar.push_back("CONTENT_LENGTH=" + IntToString(request.getBody().length()));
}

// Converts the strings to char * for execve.
void CGI::convToCharPtr()
{
	for (int i = 0; i < N_ENV_VAR + 1; i++) {
		env[i] = const_cast<char *>(envVar[i].c_str());
	}
}
