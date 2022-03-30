#include "CGI.hpp"
#include <sys/wait.h>

CGI::CGI(RequestParser* request, server_info& server) : url(request->getURL()), body(request->getBody())
{
	req = request;
	setEnvVariables(server);
	execCGI(server);
}

std::string CGI::formatContentDisposition()
{
	std::string tmp(req->getBody());
	StringVector tab = split(tmp, ";");

	tmp.clear();
	tmp.append(tab[1]);
	tmp.append(tab[2]);

	return tmp;
}

const char* CGI::findScriptType(server_info& info)
{
	std::map<std::string, std::string>::iterator it;
	LocationVector vec = info.locations;

	for (size_t i = 0; i < vec.size(); i++) {
		it = vec[i].cgi_extensions.find(req->getScriptType());
		if (it->first != "")
			break;
	}
	return it->second.c_str();
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
		envVar.push_back("CONTENT_TYPE=" + req->getContentType());
	else
		envVar.push_back("CONTENT_TYPE=text/html");
	envVar.push_back("CONTENT_LENGTH=" + IntToString(req->getBody().length()));

}

char** CGI::setExecArgs(server_info& info)
{
	char **args = new char*[3];

	const char* temp = findScriptType(info);
	size_t len = strlen(temp);

	args[0] = new char[len + 1];
	strcpy(args[0], findScriptType(info));

	len = req->getScriptPath().length();

	args[1] = new char[len + 1];
	strcpy(args[1], req->getScriptPath().c_str());

	args[2] = NULL;
	// args[0] = strdup(findScriptType(info));
	// args[1] = strdup(req->getScriptPath().c_str());
	// args[2] = NULL;
	// argv.push_back(findScriptType(info));
	// argv.push_back(req->getScriptPath());

	return args;
}

// Converts the strings to char * for execve.
char** CGI::convToCharPtr()
{
	char **envp = new char*[N_ENV_VAR + 1];

	int i = 0;
	for (; i < N_ENV_VAR; i++) {
		envp[i] = new char[envVar[i].size() + 1];
		//envp[i] = strdup(envVar[i].c_str(());
		strcpy(envp[i], envVar[i].c_str());
	}
	envp[i] = NULL;

	return envp;
}

void CGI::createPipe()
{
	if (pipe(fd_pipe) == -1)
		perror("pipe: fd_pipe");
	dup2(fd_pipe[0], STDIN_FILENO);
	dup2(fd_pipe[1], STDOUT_FILENO);
}

// char* sTochar(const std::string& str) {
// 	char* arr = new char[str.size()];

// 	memcpy(arr, str.data(), str.size() + 1);
// 	return arr;
// }

void printarrays(char** args, char** envp) {
	for (size_t i = 0; i < 3; i++)
		std::cout << args[i] << std::endl;
	
	std::cout << std::endl;

	for (size_t i = 0; i < N_ENV_VAR; i++)
		std::cout << envp[i] << std::endl;
}

void freeArrays(char** args, char** envp) {

	for (size_t i = 0; i < N_ENV_VAR; i++)
		delete[] envp[i];
	delete[] envp;

	for (size_t i = 0; i < 3; i++)
		delete[] args[i];
	delete[] args;
}

void CGI::execCGI(server_info& info)
{
	char** args = setExecArgs(info);
	char** envp = convToCharPtr();

	// if (DEBUG)
	// 	printarrays(args, envp);
	createPipe();

	// std::vector<char *> tab;
	// std::transform(argv.begin(), argv.end(), std::back_inserter(tab), &sTochar);
	// tab.push_back(NULL);

	// if (req->isUpload() == true)
	// 	body = formatContentDisposition();
	// else
	std::string body;
	body = req->getBody();
	if (req->getMethod() == "POST")
		write(fd_pipe[1], body.data(), req->getBody().size());

	pid_t pid = fork();
	if (pid == -1)
		perror("fork");
	
	if (pid == 0) {
		execScript(args, envp);
	}
	int status;
	waitpid(pid, &status, 0);

	readFromChild();
	close(fd_pipe[0]);
	close(fd_pipe[1]);

	freeArrays(args, envp);
}

void CGI::execScript(char** args, char** envp)
{
	if (execve(args[0], args, envp) == -1) {
		std::cerr << BRED << "DIDN'T FUCKING WORK" << END << std::endl;
		perror("execve");
	}
	std::cerr << GREEN << "SUCCESS" << END << std::endl;
}

void CGI::readFromChild()
{
	char buffer[100000];
	bzero(buffer, 100000);

	int ret = read(fd_pipe[0], buffer, 100000);

	ret = 0;
	output.append(buffer);
}
