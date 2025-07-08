#include <iostream>
#include <thread>
#include <string>
#include "../include/client.h"
#include "../include/other.h"
void recvMessage(Client sock)
{
	std::string buffer;
	while (1)
	{
		if (sock.recv(&buffer)) break;
		std::cout << buffer << std::endl;
	}
}

void sendMessage(Client sock)
{
	std::string buffer;
	while (1)
	{
		getline(std::cin, buffer);
		sock.send(buffer);
		if (buffer.compare("exit") == 0) break;
	}
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("usage: %s ip_address port\n", argv[0]);
		return 1;
	}
	Client client(AF_INET, std::string(argv[1]), char_to_int(argv[2]));
	if (client.connect(AF_INET, SOCK_STREAM, 0) == 1) return 1;
	std::thread sendThread(sendMessage, client);
	std::thread recvThread(recvMessage, client);
	recvThread.join();
	sendThread.join();
	client.close();
	return 0;
		
	
}
