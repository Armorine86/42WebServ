#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include "config_fields.hpp"
#include "defines.hpp"

#define BACKLOG 10

// Builds Sockets based on the informations collected by the ConfigParser class
// This class lives within the Server Class
class Sockets {
  public:
    Sockets();
    Sockets(server_info& serv_info, bool& canBind);
    Sockets(const Sockets& src) { *this = src; }
    ~Sockets() {}

    void init_sockaddr(const server_info& serv_info);
    int getServFD() { return server_fd; }
    std::string getHostName(); // return IP, and port#
    sockaddr_in& getAddress() { return address; }
    server_info getServInfo() { return serv_info; }

  private:
    int server_fd;

    struct sockaddr_in address;
    bool canBind;
    server_info serv_info; // Contain server infos from Config file
};

typedef std::vector<Sockets> SocketsVector;

#endif // __SOCKET_H__
