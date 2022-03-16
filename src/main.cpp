#include "defines.hpp"
#include "utils.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"
#include "Socket.hpp"
#include <exception>

bool duplicateBind(SocketsVector& sockvector, int size, server_info& info)
{
    if (sockvector.size() == 0)
        return false;
    for (int i = 0; i < size; i++) {
        if (sockvector[i].getServInfo().listen_port == info.listen_port)
            return true;
    }
    return false;
}

void close_serv(int sig)
{
	(void)sig;
	std::cout << YELLOW << logEvent("Closing Server....\n") << END << std::endl;
	throw std::runtime_error("Server Terminated by User");
}

int main(int argc, char** argv)
{
	signal(SIGINT, close_serv);
	signal(SIGQUIT, close_serv);
	
	if (argc <= 2){

		// No arguments = default file
		std::string config_path = (argc == 2) ? argv[1] : DEFAULT_CONFIG_FILE;

		try {
			ConfigParser config(config_path);

			SocketsVector sockvector;
		
			for (int i = 0; i < config.getServersSize(); i++) {
                bool canBind = true;
                server_info serv_info = config.getServersInfos(i);
                int size = sockvector.size();
                if (duplicateBind(sockvector, size, serv_info))
                    canBind = false;
                Sockets socket(serv_info, canBind);
                sockvector.push_back(socket);
            }

			Server server(sockvector);

		} catch (std::exception &e) {
			std::cerr << BRED << e.what() << END << std::endl;
			return (EXIT_FAILURE);
		}
	} 
	else {
		std::cerr << RED << "Error: Too Many Arguments" << END << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
