#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <string>

#include "model_server.hpp"
#include "transmission_server.hpp"

using namespace std::chrono;

Transmission::Transmission() {

	//Start variables
	connections.clientSize = (socklen_t)sizeof(connections.client);
	for(int i = 0; i < MAX_CONNECTIONS; i++) {
		connections.usedConnection[i] = 0;
	}
	connections.running = 1;

	//Star configuration on connection
	connections.socketFd = socket(AF_INET, SOCK_STREAM, 0);  
	connections.myself.sin_family = AF_INET;              
	connections.myself.sin_port = htons(3001);
	inet_aton("127.0.0.1", &(connections.myself.sin_addr));

	//Open door
	if (bind(connections.socketFd, (struct sockaddr*)&connections.myself, sizeof(connections.myself)) != 0) {
		socketStatus = false;
		return;
	}
	std::cout << "Connection Established!" << '\n';
	socketStatus = true;

	//Receive data in this door
	listen(connections.socketFd, 2);                 
	
	//Thread to listen connections
	std::thread newthread(&Transmission::waitConnections, this);
	kbThread.swap(newthread);
}

void *Transmission::waitConnections() {
	int connFD;
	int userID;

	std::cout << "Searching..." << '\n';

	//Waiting to add new connections
	while(connections.running) {
		connFD = accept(connections.socketFd, (struct sockaddr*)&connections.client, &connections.clientSize);
		userID = addConnection(connFD);
		if(userID != -1) {
			std::cout << "New user! ID = " << userID << '\n'; 
		}
	}
	return NULL;
}

int Transmission::addConnection(int newConnectionFD) {
	for(int i = 0; i < MAX_CONNECTIONS; i++) {
		if(connections.usedConnection[i] == 0) {
			connections.usedConnection[i] = 1;
			connections.connectionFd[i] = newConnectionFD;
			return 1;
		}
	}
	return -1;
}

int Transmission::removeConnection(int user) {
	if(connections.usedConnection[user] == 1) {
		connections.usedConnection[user] = 0;
		close(connections.connectionFd[user]);
		std::cout << "User removed!" << '\n';
	}
	return 1;
}

bool Transmission::getSocketStatus() {
	return this->socketStatus;
}









/*void Transmission::threadTransmission(){

	string = new char[sizeof(DataScreen)];

	while (1) {
		data->serialize(string);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		send(connection_fd, string, sizeof(DataScreen), 0);
	}
	return;
}
*/

DataScreen Transmission::getDataScreen(){
	return (this->data)->getData();
}

void Transmission::update(Ball *ball, int displacement){
	data->setData(ball->get_xAxis(), ball->get_yAxis(), displacement);
}

void Transmission::stop(){
	(this->kbThread).join();
	return;
}

RelevantData::RelevantData(int X, int Y, int B) {
	this->data.xAxis = X;
	this->data.yAxis = Y;
	this->data.displacementPaddle = B;
}

void RelevantData::serialize(char *string) {
	std::memcpy((void*)string, &(this->data), sizeof(DataScreen));
}

void RelevantData::unserialize(char *string) {
	std::memcpy(&(this->data), (void*)string, sizeof(DataScreen));
}

DataScreen RelevantData::getData() {
	return this->data;
}

void RelevantData::setData(int x, int y, int displacement) {
	data.xAxis = x;
	data.yAxis = y;
	data.displacementPaddle = displacement;
}