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
	int position;
};

struct Data {
	int xAxis;
	int yAxis;
	int scoreTeam1;
	int scoreTeam2;
	struct Paddles paddles[MAX_CONNECTIONS];
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

class Transmission {
	private:

		//Connection
		struct Connections connections;

		//Status of transmission
		bool socketStatus;

		//Threads
		std::thread kbThread;

	  //Data in struct format
	  RelevantData *data;

	  //Strings to transmit serialized data
	  char output_buffer[60];
	  char input_buffer[50];

	  //Structure data
	  Data data;

	  pthread_t esperar_conexoes;
	  int msglen;
	  int user_iterator;

	public:

		//Constructor
		Transmission();

		//Establish connection
		void initTransmission();
		void *waitConnections();
		int addConnection(int newConnectionFD);
		int removeConnection(int user);
		bool getSocketStatus();

		//Manipulate data	
		void serialize(char *inputBuffer);
		void unserialize(char *outputBuffer);
		Data getData();
		void updateData();

		//End connection
		void stop();

};

#endif