#ifndef SERVER_H
#define SERVER_H
#include <cstdint>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>
#include "socket.h"

class Server
{
	public:
		Server(int domain, std::string ip, uint16_t port);
		int init(int domain, int type, int protocol);
		int accept();
		int recv(int fd, std::string *message);
		int send(int fd, std::string message);
		void sendAll(std::string message);
		std::vector<int> getfds();
		int getfd(int index);
		int close(int index);
		void closeAll();
	private:
		struct sockaddr_in serv_addr;
		Socket serversock;
		std::vector<int> fds;
};
#endif
