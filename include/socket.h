#ifndef SOCKET_H
#define SOCKET_H
#include <sys/types.h>
#include <string>

#define BUFFER_SIZE 128

class Socket
{
	public:
		int init(int domain, int type, int protocol);
		int bind(struct sockaddr *addr, socklen_t addrlen);
		int listen(int backlog);
		int accept(struct sockaddr *addr, socklen_t *addrlen);
		int connect(struct sockaddr *addr, socklen_t addrlen);
		int send(std::string message);
		int recv(std::string *message);
		int getsockfd();
		void close();		
	private:
		int socketfd;
};
#endif
