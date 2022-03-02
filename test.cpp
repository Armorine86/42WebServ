#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <vector>
#include <iostream>
#include <fcntl.h>

#define PORT 8585

std::vector<struct pollfd> getServersSockets(std::vector<struct pollfd> pfds, int n_server)
{
	int yes = 1;
	std::vector<pollfd>::iterator it;

	struct sockaddr_in sa;

	for (int i = 0; i < n_server; i++)
	{
		pollfd pfd;

		bzero(&sa, sizeof(sa));
		sa.sin_family = AF_INET; //IPv4
		sa.sin_port = htons(PORT);
		sa.sin_addr.s_addr = inet_addr("127.0.0.1");

		if ((pfd.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			std::cout << "Invalid socket" << std::endl;
			exit(1);
		}

		fcntl(pfd.fd, F_SETFL, O_NONBLOCK);

		if (setsockopt(pfd.fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
			std::cout << "Setting option failed" << std::endl;
			exit(1);
		}

		if (i == 0)
		{
			if (bind(pfd.fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
				std::cout << "Could not bind" << std::endl;
				exit(1);
			}
		}

		if (listen(pfd.fd, 10) < 0) {
			std::cout << "Can't Listen" << std::endl;
			exit(1);
		}
		pfds.push_back(pfd);
	}
	return pfds;
}

int main(void)
{
	int n_server = 3;
	int fd_count = 0;

	std::vector<struct pollfd> pfds;
	std::vector<pollfd>::iterator it;
	
	it = pfds.begin();

	pfds = getServersSockets(pfds, n_server);
}
