#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <cstring>
#include <string>

#include "transmission_client.hpp"

using namespace std::chrono;

Transmission::Transmission(){

	//Start connection's configuration
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	target.sin_family = AF_INET;
	target.sin_port = htons(3001);
	inet_aton("127.0.0.1", &(target.sin_addr));

	//Open door
	if (connect(socketFd, (struct sockaddr*)&target, sizeof(target)) != 0) {         
		socketStatus = false;
		return;
	}
	std::cout << "Connection Established!" << '\n';
	running =1;
	socketStatus = true;

	//Thread to transmit data
	transmissionRunning = true;
	std::thread newthread(&Transmission::initTransmission, this);
	kb_thread.swap(newthread);
}

void Transmission::initTransmission(){

	string = new char[sizeof(DataScreen)];
	
	while(1){ 
		recv(socketFd, string, sizeof(DataScreen), 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		data->unserialize(string);
	}
	this->transmissionRunning = false;
	return;
}

bool Transmission::getSocketStatus(){
	return this->socketStatus;
}

bool Transmission::getTransmissionStatus(){
	return this->transmissionRunning;
}

DataScreen Transmission::getDataScreen(){
	return (this->data)->getData();
}

void Transmission::stop(){
	kb_thread.join();
	close(socketFd);
	return;
}

RelevantData::RelevantData(int X, int Y, int displacement) {
	this->data.xAxis = X;
	this->data.yAxis = Y;
	this->data.displacementPaddle = displacement;
}

void RelevantData::serialize(char *string) {
	std::memcpy((void*)string, &(this->data), sizeof(DataScreen));
}

void RelevantData::unserialize(char *string) {
	std::memcpy(&(this->data), (void*)string, sizeof(DataScreen));
}

DataScreen RelevantData::getData(){
	return this->data;
}