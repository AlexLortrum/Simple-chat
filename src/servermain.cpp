#include <iostream>
#include <sstream>
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
	struct message buffer;
	std::stringstream textbuffer;
	buffer.id = 0;

	Server server(AF_INET, std::string(argv[1]), char_to_int(argv[2]));
	socket.fd = server.init(AF_INET, SOCK_STREAM, 0); // listener fd
	if (socket.fd == 1) return 1;
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
						socket.fd = server.accept();
						if (socket.fd > 0)
						{
							pollfds.push_back(socket);
						}
					}
					else
					{
						res = server.recv(pollfds[i].fd, &buffer);
						buffer.id = server.getuserid(pollfds[i].fd);
						if (strcmp(buffer.text, "exit") == 0 || res == 1) 
						{
							server.close(i);
							pollfds.erase(pollfds.begin() + i);
							if (pollfds.size() == 1)
							{
								server.closeAll();
								return 0;
							}
							
							textbuffer << buffer.username << "(" << buffer.id << "):" << "leaves the chat";
							strcpy(buffer.text, std::string(textbuffer.str()).c_str());
							buffer.id = 0;
							strcpy(buffer.username, "Server");
						}
						server.sendAll(buffer);
						memset(buffer.text, 0, BUFFER_SIZE);
					}
				}
			}
		}
	}
}
