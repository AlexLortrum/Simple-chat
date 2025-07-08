#include <iostream>
#include <vector>
#include <poll.h>
#include <errno.h>
#include "../include/server.h"
#include "../include/socket.h"
#include "../include/other.h"


int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("usage: %s ip_address port\n", argv[0]);
		return 1;
	}
	int res;
	std::vector<struct pollfd> pollfds;
	struct pollfd socket;
	socket.events = POLLIN;
	std::string buffer;

	Server test(AF_INET, std::string(argv[1]), char_to_int(argv[2]));
	if (test.init(AF_INET, SOCK_STREAM, 0) == 1) return 1;
	socket.fd = test.getfd(0);
	pollfds.push_back(socket);
	while (1)
	{
		res = poll(pollfds.data(), pollfds.size(), -1);
		if (res == -1)
		{
			std::cout << "Poll error\nCode:" << errno << std::endl;
			return 1;
		}
		else
		{
			for (int i = 0; i < pollfds.size(); i++)
			{
				if (pollfds[i].revents & POLLIN)
				{
					if (i == 0) // new user connection
					{
						socket.fd = test.accept();
						if (socket.fd > 0)
						{
							pollfds.push_back(socket);
						}
					}
					else
					{
						res = test.recv(pollfds[i].fd, &buffer);
						if (buffer.compare("exit") == 0 || res == 1) 
						{
							test.close(i);
							pollfds.erase(pollfds.begin() + i);
							if (pollfds.size() == 1)
							{
								test.closeAll();
								return 0;
							};
						}
						else test.sendAll(buffer);
						buffer = "";
					}
				}
			}
		}
	}
}
