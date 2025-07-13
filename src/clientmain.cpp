#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include "../include/client.h"
#include "../include/other.h"
#include "../include/socket.h"

void recvMessage(Client sock)
{
	struct message buffer;
	while (1)
	{
		if (sock.recv(&buffer)) break;
		std::cout << "[" << buffer.id << ":" << buffer.username << "] " << buffer.text << std::endl;
	}
}

void sendMessage(Client sock, char username[NAME_SIZE])
{
	struct message message;
	message.id = 0;
	strcpy(message.username, username);
	std::string buffer;
	while (1)
	{
		getline(std::cin, buffer);
		strcpy(message.text, buffer.c_str());
		sock.send(message);
		if (buffer.compare("exit") == 0) break;
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("usage: %s ip_address port username\n", argv[0]);
		return 1;
	}
	Client client(AF_INET, std::string(argv[1]), char_to_int(argv[2]));
	if (client.connect(AF_INET, SOCK_STREAM, 0, argv[3]) == 1) return 1;
	std::thread sendThread(sendMessage, client, argv[3]);
	std::thread recvThread(recvMessage, client);
	recvThread.join();
	sendThread.join();
	client.close();
	return 0;	
}
