#ifndef __CGI_H__
#define __CGI_H__

#include "RequestParser.hpp"
#include "Server.hpp"
#include "defines.hpp"
#include "utils.hpp"
#include <algorithm>
#include <unistd.h>

#define N_ENV_VAR 14
#define READ 0
#define WRITE 1

#define BUFFER_SIZE 570

class CGI {
  public:
    CGI() {}
    CGI(RequestParser* request, server_info& server);
    ~CGI(){};

    std::string getCGIouput() { return output; }

  private:
    RequestParser* req;
    int fd_pipe[2];
    std::string url;
    std::string body;

    std::string output;  // final result to be sent back to Response body field
    StringVector envVar; // CGI Environment Variables

    // StringVector argv;

    std::string formatContentDisposition();
    std::string& findScriptType(server_info& server);
    void setEnvVariables(server_info& server);
    char** setExecArgs(server_info& server);
    char** convToCharPtr();
    void execCGI(server_info& server);
    void execScript(char** args, char** envp);

    void readFromChild();

    void createPipe();
    void cleanPipes();

}; // CGI

#endif
