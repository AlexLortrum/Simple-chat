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
		int connect(int domain, int type, int protocol, char username[NAME_SIZE]);
		int recv(struct message *message);
		int send(struct message message);
		void close();
	private:
		struct sockaddr_in serv_addr;
		Socket clientsock;
};

#endif
