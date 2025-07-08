#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <sys/types.h>
#include <arpa/inet.h>
#include "socket.h"

class Client
{
	public:
		Client(int domain, std::string ip, uint16_t port);
		int connect(int domain, int type, int protocol);
		int recv(std::string *message);
		int send(std::string message);
		void close();
	private:
		struct sockaddr_in serv_addr;
		Socket clientsock;
};

#endif
