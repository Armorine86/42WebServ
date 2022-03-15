#include "defines.hpp"
#include "utils.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"
#include "Socket.hpp"
#include <exception>

// typedef std::vector<Sockets> SocketsVector;

void close_serv(int sig)
{
	(void)sig;
	std::cout << YELLOW << logEvent("Closing Server....\n") << END << std::endl;
	throw std::runtime_error("Server Terminated by User");
}

int main(int argc, char** argv)
{
	std::string config_path;
	
	signal(SIGINT, close_serv);
	signal(SIGQUIT, close_serv);

	std::vector<Server> servers;
	
	if (argc <= 2){
		// No arguments = default file
		config_path = (argc == 2) ? argv[1] : DEFAULT_CONFIG_FILE;

		// for (int i = 0; i < config.server)
		// config.getServersInfos();
		
		// Allume le sockets pour chaque serveur du config file pour receive request (port unique)
		// Parse Request
		// Exec le bon serveur avec la bonne method
		// Create Respond
		try {
			ConfigParser config(config_path);
			SocketsVector sockvector;
		
			for (int i = 0; i < config.getServersSize(); i++) {
				server_info serv_info = config.getServersInfos(i);
				Sockets socket(serv_info);
				sockvector.push_back(socket);
			}

			Server server(sockvector);

			//server.run();
		} catch (std::exception &e) {
			std::cout << BRED << e.what() << END << std::endl;
			return (EXIT_FAILURE);
		}
	} 
	else {
		std::cerr << RED << "Error: Too Many Arguments" << END << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
