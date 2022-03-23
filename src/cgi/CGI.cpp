#include "CGI.hpp"

CGI::CGI(RequestParser& request, server_info& server)
{
	setEnvVariables(request, server);
	convToCharPtr();
	setExecArgs(request, server);
}

char* CGI::findScriptType(RequestParser& request, server_info& server)
{
	std::map<std::string, std::string>::iterator it;
	LocationVector vec = server.locations;

	for (size_t i = 0; i < vec.size(); i++) {
		it = vec[i].cgi_extensions.find(request.getScriptType());
		if ((*it).first != "")
			break;
	}
	return const_cast<char*>((*it).second.c_str());
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

void CGI::setExecArgs(RequestParser& request, server_info& server)
{
	(void)request;
	bzero(args, sizeof(args));
	args[0] = findScriptType(request, server);
	args[1] = const_cast<char*>(request.getScriptPath().c_str());
}

// Converts the strings to char * for execve.
void CGI::convToCharPtr()
{
	bzero(envp, sizeof(envp));

	for (int i = 0; i < N_ENV_VAR + 1; i++) {
		envp[i] = const_cast<char *>(envVar[i].c_str());
	}
}

void CGI::execCGI(/*???*/)
{
	
}
