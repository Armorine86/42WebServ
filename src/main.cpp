#include "defines.hpp"
#include "utils.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"

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

		// for (int i = 0; i < config.server)
		// config.getServersInfos();
		
		// Allume le sockets pour chaque serveur du config file pour receive request (port unique)
		// Parse Request
		// Exec le bon serveur avec la bonne method
			// Create Respond
		Server server(config.getServersInfos(0));
		//server.run();
	} else {
		std::cerr << RED << "Error: Too Many Arguments" << END << std::endl;
		exit(EXIT_FAILURE);
	}
}

/* int main()
{
	std::string requeststr = "GET /fr/aire-perimetre/triangle-rectangle.html HTTP/1.1\n\
Host: www.calculat.org\n\
User-Agent: Mozilla/5.0 X11; Ubuntu; Linux x86_64; rv:97.0 Gecko/20100101 Firefox/97.0\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,/;q=0.8\n\
Accept-Language: en-CA,en-US;q=0.7,en;q=0.3\n\
Accept-Encoding: gzip, deflate, br\n\
Connection: keep-alive\n\
Cookie: _ga=GA1.2.1907237419.1646320397; _gid=GA1.2.1462519239.1646320397; _gat=1\n\
Upgrade-Insecure-Requests: 1\n\
Sec-Fetch-Dest: document\n\
Sec-Fetch-Mode: navigate\n\
Sec-Fetch-Site: none\n\
Sec-Fetch-User: ?1";

	RequestParser request(requeststr);
	while(1);
   
} */
