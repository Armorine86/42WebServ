#include "CGI.hpp"

CGI::CGI(RequestParser& request, server_info& server) : url(request.getURL()), body(request.getBody())
{
	bzero(env, sizeof(env));

	setEnvVariables();
	convToCharPtr();
}

void CGI::setEnvVariables()
{
	envVar.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envVar.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVar.push_back("SERVER_SOFTWARE=webserv");
	envVar.push_back("REDIRECT_STATUS=200");
	envVar.push_back("SERVER_NAME=");
	envVar.push_back("SERVER_PORT=");
	envVar.push_back("REQUEST_METHOD=");
	envVar.push_back("PATH_INFO=");
	envVar.push_back("SCRIPT_NAME=");
	envVar.push_back("QUERY_STRING=");
	envVar.push_back("REMOTE_ADDR=");
	envVar.push_back("PATH_TRANSLATED=");
	envVar.push_back("CONTENT_TYPE=");
	envVar.push_back("CONTENT_LENGTH=");
}

// Converts the strings to char * for execve.
void CGI::convToCharPtr()
{
	for (int i = 0; i < N_ENV_VAR + 1; i++) {
		env[i] = const_cast<char *>(envVar[i].c_str());
	}
}
