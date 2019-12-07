all: game server

game:
	g++ game.cpp -o game -pthread

server:
	g++ server.cpp -o server -pthread