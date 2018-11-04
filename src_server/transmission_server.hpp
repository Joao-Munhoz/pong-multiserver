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
	int id;
	int position[SIZE_PADDLE];
};

struct Data {
	float xAxis;
	float yAxis;
	int positionPaddle[SIZE_PADDLE];
	struct Paddles paddles[MAX_CONNECTIONS];
	int running;
};

class Transmission {
	private:

		//Connection
		struct sockaddr_in client, myself;
		int socketFd, running;
		int connectionFd[MAX_CONNECTIONS];
		int usedConnection[MAX_CONNECTIONS];
		socklen_t clientSize;

		//Status of transmission
		bool socketStatus;

		//Threads
		std::thread kbThread;

		//Data in struct format
		Data data;

		//Strings to transmit serialized data
		char outputBuffer[120];
		char inputBuffer[120];

		pthread_t esperar_conexoes;
		int msglen;
		int user_iterator;

	public:

		//Constructor
		Transmission();

		//Establish connection
		void init();
		void threadTransmission();
		void waitConnections();
		int addConnection(int newConnectionFD);
		int removeConnection(int user);
		bool getSocketStatus();

		//Manipulate data	
		void serialize(char *inputBuffer);
		void unserialize(char *outputBuffer);
		Data getData();
		void updateBall(float xAxis, float yAxis);
		void updatePaddle(int id, int *positionPaddle);
		void updatePaddle(int id);

		//End connection
		void stop();

};

#endif