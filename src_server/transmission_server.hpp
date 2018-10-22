#ifndef TRASNMISSION_SERVER_HPP
#define TRASNMISSION_SERVER_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>

struct Paddles {
	Paddle *paddle;
	int id;
	int positionPaddles;
};

struct ServerToClient {
	int xAxis;
	int yAxis;
	struct Paddle * paddles;
};

struct ClientToServer {
	int id;
	int positionPaddle;
};

struct DataScreen {
	int xAxis;
	int yAxis;
	int displacementPaddle; 
};

class RelevantData {
	private:
		DataScreen data;

	public:
		RelevantData(int xAxis, int yAxis, int displacementPaddle);
		RelevantData(char *string);
		void serialize(char *string);
		void unserialize(char *string);
		DataScreen getData();
		void setData(int x, int y, int displacement);
};

class Transmission {
	private:
		int socket_fd, connection_fd;
		struct sockaddr_in myself, client;
		bool socket_status, transmissionRunning;
		RelevantData *data;
		std::thread kb_thread;
		char * string;

	public:
		Transmission();
		void threadTransmission();
		bool getSocketStatus();
		bool getTransmissionStatus();
		DataScreen getDataScreen();
		void update(Ball *ball, int displacement);
		void stop();
};

#endif