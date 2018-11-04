#ifndef TRANSMISSION_CLIENT_HPP
#define TRANSMISSION_CLIENT_HPP

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
	int scoreTeam1;
	int scoreTeam2;
	int positionPaddle[SIZE_PADDLE];
	struct Paddles paddles[MAX_CONNECTIONS];
	int running;
};

class Transmission {
private:

	//Connection
	int socketFd, running;
	struct sockaddr_in target;

	//Status of transmission
	bool socketStatus, transmissionRunning;

	//Threads
	std::thread kbThread;

	//Data in struct format
	Data data;

	//Strings to transmit serialized data
	char outputBuffer[120];
	char inputBuffer[120];

public:

	//Constructor
	Transmission();

	//Establish connection
	void init();
	void threadTransmission();
	bool getSocketStatus();

	//Manipulate Data
	Data getData();
	void serialize(char *input_buffer);
	void unserialize(char *output_buffer);
	void updatePaddle(int *position);

	//End connection
	void stop();
};

#endif