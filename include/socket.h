#ifndef SOCKET_H
#define SOCKET_H
#include <sys/types.h>
#include <string>

#define BUFFER_SIZE 128
#define NAME_SIZE 16

struct message
{
	int id;
	char username[NAME_SIZE];
	char text[BUFFER_SIZE];
};

class Socket
{
	public:
		int init(int domain, int type, int protocol);
		int bind(struct sockaddr *addr, socklen_t addrlen);
		int listen(int backlog);
		int accept(struct sockaddr *addr, socklen_t *addrlen);
		int connect(struct sockaddr *addr, socklen_t addrlen);
		int send(struct message message);
		int recv(struct message *message);
		int getsockfd();
		void close();		
	private:
		int socketfd;
};
#endif
