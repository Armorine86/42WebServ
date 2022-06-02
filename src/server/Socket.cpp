#include "Socket.hpp"

// Creates a socket for the server with the infos gathered in the parser.
// Makes the sockets Non-Blocking with fcntl(), and binds it to the port & IP.
//
// If an error occurs, the program exits with appropriate code.
Sockets::Sockets(server_info& serv_info, bool& canBind)
    : server_fd(0), canBind(canBind), serv_info(serv_info) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        std::cerr << logEvent("[SOCKET] Invalid fd: ") << strerror(errno) << "\n"
                  << END << std::endl;
        throw std::runtime_error("Invalid Socket");
    }

    // Sets socket to non-blocking
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    int yes = 1;

    // Makes the port reusable if a socket lingers in the Kernel
    // Gets rid of the "Address already in use" message
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {

        std::cerr << logEvent("[SOCKET] Could not set Options: ") << strerror(errno) << "\n"
                  << END << std::endl;
        throw std::runtime_error("Cannot set socket option");
    }

    init_sockaddr(serv_info);

    // in case we have multiple servers listening on the same socket
    // you cannot bind a socket twice on the same Port/IP
    if (canBind == true)
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {

            std::cerr << logEvent("[SOCKET] Binding Error: ") << strerror(errno) << "\n"
                      << END << std::endl;
            throw std::runtime_error("Cannot bind socket");
        }

    if (listen(server_fd, BACKLOG) < 0) {

        std::cerr << logEvent("[SOCKET] Could not listen on port: ") << strerror(errno) << "\n"
                  << END << std::endl;
        throw std::runtime_error("Cannot listen on socket");
    }
}

// Inits sockaddr_in struct. Struct has been bzero()ed before hand
//
// sin_family = AF_INET (IPv4)
// sin_port = htons(listen_port) 127.0.0.1 | Localhost
// sin_addr.s_addr = inet_addr(host)
//
// according to beej's tutorial, sin_zero doesn't need to be touched at all as it changes nothing
void Sockets::init_sockaddr(const server_info& serv_info) {
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET; // IPv4
    address.sin_port = htons(serv_info.listen_port);
    address.sin_addr.s_addr = inet_addr(serv_info.host.c_str());
}

std::string Sockets::getHostName() {
    std::string s;
    std::stringstream out;
    out << serv_info.listen_port;
    s = out.str();
    return serv_info.host + ":" + s;
}
