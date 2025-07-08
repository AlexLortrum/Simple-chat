#include <cstring>
#include <sys/types.h>
#include <arpa/inet.h>
#include "../include/socket.h"
#include "../include/client.h"

Client::Client(int domain, std::string ip, uint16_t port)
{	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = domain;
	serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	serv_addr.sin_port = htons(port);
}
int Client::connect(int domain, int type, int protocol)
{
	if (clientsock.init(domain, type, protocol) == 1) return 1;
	if (clientsock.connect((struct sockaddr*) &serv_addr, sizeof(serv_addr)) == 1) return 1;
	return 0;
}
int Client::recv(std::string *message)
{
	if (clientsock.recv(message)) return 1;
	return 0;
}
int Client::send(std::string message)
{
	if (clientsock.send(message)) return 1;
	return 0;
}
void Client::close()
{
	clientsock.close();
}
