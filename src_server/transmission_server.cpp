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
	clientSize = (socklen_t)sizeof(client);
	for(int i = 0; i < MAX_CONNECTIONS; i++) {
		usedConnection[i] = 0;
	}
	running = 1;

	//Start connection's configuration
	socketFd = socket(AF_INET, SOCK_STREAM, 0);  
	myself.sin_family = AF_INET;              
	myself.sin_port = htons(3001);
	inet_aton("127.0.0.1", &(myself.sin_addr));

	//Open door
	if (bind(socketFd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
		socketStatus = false;
		return;
	}
	std::cout << "Connection Established!" << '\n';
	socketStatus = true;

	//Receive data in this door
	listen(socketFd, 2);
}

void Transmission::waitConnections() {
	int connFD;
	int userID;
	int numberConnections = 0;

	std::cout << "Searching..." << '\n';

	//Waiting to add new connections
	while(numberConnections != MAX_CONNECTIONS) {
		connFD = accept(socketFd, (struct sockaddr*)&(client), &(clientSize));
		userID = addConnection(connFD);
		if(userID != -1) {
			std::cout << "New user! ID = " << userID << '\n'; 
		}
		numberConnections++;
	}
	std::cout << "Players connected!" << '\n';
	return;
}

int Transmission::addConnection(int newConnectionFD) {
	for(int i = 0; i < MAX_CONNECTIONS; i++) {
		if(usedConnection[i] == 0) {
			usedConnection[i] = 1;
			connectionFd[i] = newConnectionFD;
			return i;
		}
	}
	return -1;
}

int Transmission::removeConnection(int user) {
	if(usedConnection[user] == 1) {
		usedConnection[user] = 0;
		close(connectionFd[user]);
		std::cout << "User removed!" << '\n';
	}
	return 1;
}

void Transmission::init(){
	std::thread newthread(&Transmission::threadTransmission, this);
	kbThread.swap(newthread);	
}

void Transmission::threadTransmission(){
	std::cout << "Communicating with clients..." << '\n';
	while (1) {	
		std:: cout << "xAxis: " << data.xAxis << " - yAxis: " << data.yAxis << '\n';
		std::cout << "Paddles positions: " << '\n';
		for(int i = 0; i < MAX_CONNECTIONS; i++){
			data.id = i;
			serialize(inputBuffer);
			send(connectionFd[i], inputBuffer, 300, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			msglen = recv(connectionFd[i], outputBuffer, 300, 0);
			if(msglen < 0){
				removeConnection(i);
				exchange(i, -1);
			}
			unserialize(outputBuffer);
			exchange(i);
			for (int j = 0; j < SIZE_PADDLE; ++j){
				std::cout << data.paddles[i].position[j] << ' ';
			}
			std::cout << '\n';
		}
	}
	return;
}

bool Transmission::getSocketStatus() {
	return this->socketStatus;
}

void Transmission::serialize(char *buffer) {
	std::memcpy((void*)buffer, &(this->data), sizeof(Data));
}

void Transmission::unserialize(char *buffer) {
	std::memcpy(&(this->newData), (void*)buffer, sizeof(Data));
}

void Transmission::exchange(int id){
	for (int i = 0; i < SIZE_PADDLE; ++i){
		data.paddles[id].position[i] = newData.paddles[id].position[i];
	}
}

void Transmission::exchange(int id, int off){
	for (int i = 0; i < SIZE_PADDLE; i++){
		data.paddles[id].position[i] = off;
	}
}

Data Transmission::getData(){
	return this->data;
}

void Transmission::updateBall(float xAxis, float yAxis){
	data.xAxis = xAxis;
	data.yAxis = yAxis;
	return;
}

void Transmission::stop(){
	for(int i = 0; i < MAX_CONNECTIONS; i++){
		this->removeConnection(i);
	}
	(this->kbThread).join();
	return;
}