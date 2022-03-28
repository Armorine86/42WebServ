#include "CGI.hpp"
#include <sys/wait.h>

CGI::CGI(RequestParser* request, server_info& server) : url(request->getURL()), body(request->getBody())
{
	req = request;
	setEnvVariables(server);
	convToCharPtr();
	execCGI(server);
}

char* CGI::findScriptType(server_info& info)
{
	std::map<std::string, std::string>::iterator it;
	LocationVector vec = info.locations;

	for (size_t i = 0; i < vec.size(); i++) {
		it = vec[i].cgi_extensions.find(req->getScriptType());
		if ((*it).first != "")
			break;
	}
	return const_cast<char*>((*it).second.c_str());
}

void CGI::setEnvVariables(server_info& info)
{
	envVar.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envVar.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVar.push_back("SERVER_SOFTWARE=webserv");
	envVar.push_back("REDIRECT_STATUS=200"); // used for PHP
	envVar.push_back("SERVER_NAME=" + info.server_names);
	envVar.push_back("SERVER_PORT=" + IntToString(info.listen_port));
	envVar.push_back("REQUEST_METHOD=" + req->getMethod());
	envVar.push_back("PATH_INFO=" + req->getScriptPath());
	envVar.push_back("SCRIPT_NAME=" + req->getScriptName());
	envVar.push_back("QUERY_STRING=" + req->getQuery());
	envVar.push_back("REMOTE_ADDR=" + info.host);
	envVar.push_back("PATH_TRANSLATED=" + req->getScriptPath());
	if (req->getMethod() == "POST")
		envVar.push_back("CONTENT_TYPE=application/x-www-form-urlencoded")/* + req->getContentType())*/;
	else
		envVar.push_back("CONTENT_TYPE=text/html")/* + req->getContentType())*/;
	envVar.push_back("CONTENT_LENGTH=" + IntToString(req->getBody().length()));
}

void CGI::setExecArgs(server_info& info)
{
	bzero(args, sizeof(args));

	argv.push_back(findScriptType(info));
	argv.push_back(req->getScriptPath());
}

// Converts the strings to char * for execve.
void CGI::convToCharPtr()
{
	bzero(envp, sizeof(envp));

	int i = 0;
	for (; i < N_ENV_VAR; i++) {
		envp[i] = const_cast<char*>(envVar[i].c_str());
	}
	envp[i] = NULL;
}

void CGI::createPipe()
{
	if (pipe(fd_pipe) == -1)
		perror("pipe: fd_pipe");
	dup2(fd_pipe[0], STDIN_FILENO);
	dup2(fd_pipe[1], STDOUT_FILENO);
}

char* sTochar(const std::string& str) {
	char* arr = new char[str.size()];

	memcpy(arr, str.data(), str.size() + 1);
	return arr;
}

void CGI::execCGI(server_info& info)
{
	createPipe();
	setExecArgs(info);

	std::vector<char *> tab;
	std::transform(argv.begin(), argv.end(), std::back_inserter(tab), &sTochar);
	tab.push_back(NULL);

	if (req->getMethod() == "POST")
		write(fd_pipe[1], req->getBody().data(), req->getBody().size());

	pid_t pid = fork();
	if (pid == -1)
		perror("fork");
	
	if (pid == 0) {
		execScript(tab.data());
	}
	int status;
	waitpid(pid, &status, 0);

	readFromChild();
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	for (size_t i = 0; i < tab.size(); i++) {
		delete[] tab[i];
	}
}

void CGI::execScript(char** argv)
{
	if (execve(argv[0], argv, envp) == -1) {
		perror("execve");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void CGI::readFromChild()
{
	char buffer[BUFFER_SIZE + 1];
	bzero(buffer, sizeof(buffer));

	int ret = read(fd_pipe[0], buffer, BUFFER_SIZE);

	ret = 0;
	output.append(buffer);
}
