#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <cstring>
#include "../include/client.h"
#include "../include/other.h"
#include "../include/socket.h"
#include "../include/terminal.h"

std::mutex mtx;

void recvMessage(Client sock)
{
	struct message buffer;
	int y = 1;
	int maxy;
	gety(&maxy);
	while (1)
	{
		if (sock.recv(&buffer)) break;
		mtx.lock();
		if (y == maxy)
		{
			y--;
			restore_cur();
			printf("\n");
		}
		mvcurs(y, 0);
		std::cout << "[" << buffer.id << ":" << buffer.username << "] " << buffer.text << std::endl;
		y++;
		if (y == maxy) save_cur();
		mtx.unlock();
	}
}

void sendMessage(Client sock, char username[NAME_SIZE])
{
	struct message message;
	message.id = 0;
	memset(message.text, 0, BUFFER_SIZE);
	strcpy(message.username, username);
	std::string buffer = "";
	char ch = 0;
	int y;
	int x = 1;
	gety(&y);
	while (1)
	{
		while (ch != '\n')
		{
			ch = getchar();
			if (ch == 127 && x > 1) // backspace
			{
				x--;
				buffer.pop_back();
				mtx.lock();
				mvcurs(y, x);
				printf(" ");
				mvcurs(y, x);
				mtx.unlock();
			}
			else if (x != BUFFER_SIZE-1 && ch != '\n')
			{
				buffer.push_back(ch);
				mtx.lock();
				mvcurs(y, x);
				printf("%c", ch);
				x++;
				mtx.unlock();
			}
		}
		strcpy(message.text, buffer.c_str());
		sock.send(message);
		if (buffer.compare("exit") == 0) break;
		buffer.erase();
		ch = 0;
		x = 1;
		mvcurs(y, 0);
		clear_line();
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
	terminit();
	std::thread sendThread(sendMessage, client, argv[3]);
	std::thread recvThread(recvMessage, client);
	recvThread.join();
	sendThread.join();
	client.close();
	termexit();
	return 0;	
}
