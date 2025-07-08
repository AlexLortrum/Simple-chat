#include <cstring>
#include <iostream>
#include <cstdint>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include "../include/server.h"

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
	fds.push_back(serversock.getsockfd());
	return 0;
}
int Server::accept()
{
	int clnt_sock = serversock.accept(NULL, NULL);
	if (clnt_sock < 0) return 1;
	fds.push_back(clnt_sock);
	return clnt_sock;
}

int Server::recv(int fd, std::string *message)
{
	char buffer[BUFFER_SIZE] = {0};
	int res = ::recv(fd, buffer, BUFFER_SIZE, 0);
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

int Server::send(int fd, std::string message)
{
	int res = ::send(fd, message.c_str(), message.size(), 0);
	if (res == -1)
	{
		std::cout << "Message send failed\nCode:" << errno << std::endl;
		return 1;
	}
	return 0;
}

void Server::sendAll(std::string message)
{
	for (int i = 1; i < fds.size(); i++)
	{
		send(fds[i], message);
	}
}

std::vector<int> Server::getfds()
{
	return fds;
}

int Server::getfd(int index)
{
	if (index > fds.size()) return -1;
	return fds[index];
}

int Server::close(int index)
{
	if (index > fds.size()) return -1;
	::close(fds[index]);
	fds.erase(fds.begin() + index);
	return 0;
}

void Server::closeAll()
{
	for (int i = fds.size()-1; i >= 0; i--)
	{
		::close(fds[i]);
	}
}
