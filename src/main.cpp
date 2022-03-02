#include "defines.hpp"
#include "utils.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"

void close_serv(int sig)
{
	(void)sig;
	std::cout << YELLOW << "Closing Server...." << END << std::endl;
	exit(SIG_EXIT);
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
		ConfigParser config(config_path);

		for (int i = 0; i < config.server)
		config.getServersInfos();
		
		// Allume le sockets pour chaque serveur du config file pour receive request (port unique)
		// Parse Request
		// Exec le bon serveur avec la bonne method
			// Create Respond
		

	} else {
		std::cerr << RED << "Error: Too Many Arguments" << END << std::endl;
		exit(EXIT_FAILURE);
	}
}
