#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include "model_client.hpp"

using namespace std::chrono;

Transmission::Transmission(){

	data = new RelevantData((int)(SCREEN_HEIGHT/2),(int)(SCREEN_WIDTH/2), 0);
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	target.sin_family = AF_INET;
	target.sin_port = htons(3001);
	inet_aton("127.0.0.1", &(target.sin_addr));
	if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {         
		socket_status = false;
	} else {
		socket_status = true;
		transmissionRunning = true;
		std::thread newthread(&Transmission::threadTransmission, this);
		kb_thread.swap(newthread);
	}
}

bool Transmission::getSocketStatus(){
	return this->socket_status;
}

bool Transmission::getTransmissionStatus(){
	return this->transmissionRunning;
}

void Transmission::threadTransmission(){

	string = new char[sizeof(DataScreen)];
	
	while(1){ 
		recv(socket_fd, string, sizeof(DataScreen), 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		data->unserialize(string);
	}
	this->transmissionRunning = false;
	return;
}

DataScreen Transmission::getDataScreen(){
	return (this->data)->getData();
}

void Transmission::stop(){
	kb_thread.join();
	close(socket_fd);
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