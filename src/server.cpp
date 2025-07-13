#include <cstring>
#include <iostream>
#include <cstdint>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include "../include/server.h"
#include "../include/other.h"

Server::Server(int domain, std::string ip, uint16_t port)
{
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = domain;
	serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	serv_addr.sin_port = htons(port);
}

int Server::init(int domain, int type, int protocol)
{
	if (serversock.init(domain, type, protocol) == 1) return 1;
	if (serversock.bind((struct sockaddr*) &serv_addr, sizeof(serv_addr)) == 1) return 1;
	if (serversock.listen(5) == 1) return 1;
	struct user listener = {0};
	listener.fd = serversock.getsockfd();
	listener.id = 0;
	users.push_back(listener);
	return listener.fd;
}

int Server::accept()
{
	int clnt_sock = serversock.accept(NULL, NULL);
	if (clnt_sock < 0) return 1;
	struct message userdata;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(clnt_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	if (recv(clnt_sock, &userdata) == 1) 
	{
		::close(clnt_sock);
		return -1;
	}
	tv.tv_sec = 0;
	setsockopt(clnt_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	struct user user;
	user.fd = clnt_sock;
	user.id = ::hash(userdata.username, clnt_sock);
	strcpy(user.username, userdata.username);
	users.push_back(user);
	return clnt_sock;
}

int Server::recv(int fd, struct message *message)
{
	struct message buffer;
	int res = ::recv(fd, &buffer, sizeof(struct message), 0);
	*message = buffer;
	lseek(fd, 0, SEEK_END);
	if (res == -1)
	{
		std::cout << "Message recv failed\nCode:" << errno << std::endl;
		return 1;
	}
	else if (res == 0)
	{
		std::cout << "Socket was disconnected" << std::endl;
		return 1;
	}
	return 0;
}

int Server::send(int fd, struct message message)
{
	int res = ::send(fd, &message, sizeof(struct message), 0);
	if (res == -1)
	{
		std::cout << "Message send failed\nCode:" << errno << std::endl;
		return 1;
	}
	return 0;
}

void Server::sendAll(struct message message)
{
	for (int i = 1; i < users.size(); i++)
	{
		send(users[i].fd, message);
	}
}

int Server::getuserid(int fd)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].fd == fd) return users[i].id;
	}
	return -1;
}

int Server::close(int index)
{
	::close(users[index].fd);
	users.erase(users.begin() + index);
	return 0;
}

void Server::closeAll()
{
	for (int i = users.size()-1; i >= 0; i--)
	{
		::close(users[i].fd);
	}
}
