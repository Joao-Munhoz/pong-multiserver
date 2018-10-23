#ifndef TRASNMISSION_SERVER_HPP
#define TRASNMISSION_SERVER_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>

#define MAX_CONNECTIONS 6

struct Paddles {
	Paddle *paddle;
	int id;
	int positionPaddles;
};

struct ServerToClient {
	int xAxis;
	int yAxis;
	struct Paddle paddles[MAX_CONNECTIONS];
};

struct ClientToServer {
	int id;
	int positionPaddle;
};

struct Connections {
	struct sockaddr_in client, myself;
	struct ServerToClient dataServer;
	struct ClientToServer dataClient;
	int socketFd, running;
	int connectionFd[MAX_CONNECTIONS];
	int usedConnection[MAX_CONNECTIONS];
	socklen_t clientSize;
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
		struct Connections connections;

		//Status of transmission
		bool socketStatus;

		//Threads
		std::thread kbThread;

		pthread_t esperar_conexoes;
	  int msglen;
	  int user_iterator;

	  //Data in struct format
	  RelevantData *data;

	  //Strings to transmit serialized data
	  char output_buffer[60];
	  char input_buffer[50];


	public:
		Transmission();
		//void threadTransmission();
		DataScreen getDataScreen();
		void update(Ball *ball, int displacement);
		void stop();

		void *waitConnections();
		int addConnection(int newConnectionFD);
		int removeConnection(int user);
		bool getSocketStatus();

};

#endif