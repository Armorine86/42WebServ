#include "CGI.hpp"

CGI::CGI(RequestParser& request, server_info& info, int& sender, int& serv_fd) :
	client_fd(sender), server_fd(serv_fd)
{
	setEnvVariables(request, info);
	convToCharPtr();
	setExecArgs(request, info);
	execCGI(request, info);
}

char* CGI::findScriptType(RequestParser& request, server_info& info)
{
	std::map<std::string, std::string>::iterator it;
	LocationVector vec = info.locations;

	for (size_t i = 0; i < vec.size(); i++) {
		it = vec[i].cgi_extensions.find(request.getScriptType());
		if ((*it).first != "")
			break;
	}
	return const_cast<char*>((*it).second.c_str());
}

void CGI::setEnvVariables(RequestParser& request, server_info& info)
{
	envVar.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envVar.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVar.push_back("SERVER_SOFTWARE=webserv");
	envVar.push_back("REDIRECT_STATUS=200"); // used for PHP
	envVar.push_back("SERVER_NAME=" + info.server_names);
	envVar.push_back("SERVER_PORT=" + IntToString(info.listen_port));
	envVar.push_back("REQUEST_METHOD=" + request.getMethod());
	envVar.push_back("PATH_INFO=" + request.getScriptPath());
	envVar.push_back("SCRIPT_NAME=" + request.getScriptName());
	envVar.push_back("QUERY_STRING=" + request.getQuery());
	envVar.push_back("REMOTE_ADDR=" + info.host);
	envVar.push_back("PATH_TRANSLATED=" + request.getScriptPath());
	envVar.push_back("CONTENT_TYPE=" + request.getContentType());
	envVar.push_back("CONTENT_LENGTH=" + IntToString(request.getBody().length()));
}

void CGI::setExecArgs(RequestParser& request, server_info& info)
{
	bzero(args, sizeof(args));

	args[0] = findScriptType(request, info);
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

void CGI::execCGI(RequestParser& request, server_info& info)
{
	(void)client_fd;
	(void)server_fd;
	(void)request;
	(void)info;
	// client_fd = 0;
	// server_fd = 0;
	// request.getURL();
	// std::string host = info.host;
	if (access(args[0], F_OK) == -1)
		perror(args[0]);
	
	//TODO if (execve(args[0], &args, envp) == -1)
}
