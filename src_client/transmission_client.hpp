#ifndef TRANSMISSION_CLIENT_HPP
#define TRANSMISSION_CLIENT_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>

struct Paddles {
	int id;
	int positionPaddles[MAX_CONNECTIONS];
};

struct Data {
	int xAxis;
	int yAxis;
	int scoreTeam1;
	int scoreTeam2;
	struct Paddles paddles;
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
	RelevantData *data;

	//Strings to transmit serialized data
	char output_buffer[60];
	char input_buffer[50];

public:
	Transmission();
	void initTransmission();
	bool getSocketStatus();
	bool getTransmissionStatus();
	DataScreen getDataScreen();
	void serialize(char *input_buffer);
	void unserialize(char *output_buffer);
	Data getData();
	void setData();
	void stop();
};

#endif