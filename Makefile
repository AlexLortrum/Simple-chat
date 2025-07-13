all: client server

client: src/client.cpp src/clientmain.cpp src/socket.cpp src/other.cpp
	g++ src/client.cpp src/clientmain.cpp src/socket.cpp src/other.cpp -o ./client -g

server: src/server.cpp src/servermain.cpp src/socket.cpp src/other.cpp
	g++ src/server.cpp src/servermain.cpp src/socket.cpp src/other.cpp -o ./server -g 
