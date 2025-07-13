#include <iostream>
#include <cstring>
#include <cstdint>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../include/socket.h"

int Socket::init(int domain, int type, int protocol)
{
	socketfd = socket(domain, type, protocol);
	if (socketfd == -1)
	{
		std::cout << "Socket failed\nCode:" << errno << std::endl;
		return 1;
	}
	return 0;
}

int Socket::bind(struct sockaddr *addr, socklen_t addrlen)
{
	int res = ::bind(socketfd, addr, addrlen);
	if (res == -1)
	{
		std::cout << "Bind failed\nCode:" << errno << std::endl;
		return 1;
	}
	return 0;
}

int Socket::listen(int backlog)
{
	int res = ::listen(socketfd, backlog);
	if (res == -1)
	{
		std::cout << "Listen failed\nCode:" << errno << std::endl;
		return 1;
	}
	return 0;
}

int Socket::accept(struct sockaddr *addr, socklen_t *addrlen)
{
	int res = ::accept(socketfd, addr, addrlen);
	if (res == -1)
	{
		std::cout << "Accept failed\nCode:" << errno << std::endl;
		return 1;
	}
	return res;
}

int Socket::connect(struct sockaddr *addr, socklen_t addrlen)
{
	int res = ::connect(socketfd, addr, addrlen);
	if (res == -1)
	{
		std::cout << "Connect failed\nCode:" << errno << std::endl;
		return 1;
	}
	return 0;
}

int Socket::send(struct message message)
{
	int res = ::send(socketfd, &message, sizeof(message), 0);
	if (res == -1)
	{
		std::cout << "Message send failed\nCode:" << errno << std::endl;
		return 1;
	}
	return 0;
}

int Socket::recv(struct message *message)
{
	struct message buffer;
	int res = ::recv(socketfd, &buffer, sizeof(struct message), 0);
	if (res == -1)
	{
		std::cout << "Message recv failed\nCode:" << errno << std::endl;
		return 1;
	}
	else if (res == 0) return 1; // socket was closed
	*message = buffer;
	lseek(socketfd, 0, SEEK_END);
	return 0;
}

int Socket::getsockfd() 
{
	return socketfd;
}

void Socket::close()
{
	::close(socketfd);
}
