#ifndef SERVER_H
#define SERVER_H
#include <cstdint>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>
#include "socket.h"

struct user 
{
	int fd;
	int id;
	char username[NAME_SIZE];
};

class Server
{
	public:
		Server(int domain, std::string ip, uint16_t port);
		int init(int domain, int type, int protocol);
		int accept();
		int recv(int fd, struct message *message);
		int send(int fd, struct message message);
		void sendAll(struct message message);
		int getuserid(int fd);
		int close(int index);
		void closeAll();
	private:
		struct sockaddr_in serv_addr;
		Socket serversock;
		std::vector<user> users;
};
#endif
