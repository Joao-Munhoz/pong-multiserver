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

Transmission::Transmission(){

	data = new RelevantData((int)(SCREEN_HEIGHT/2),(int)(SCREEN_WIDTH/2), 0);
	socklen_t client_size = (socklen_t)sizeof(client);
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);  

	myself.sin_family = AF_INET;              
	myself.sin_port = htons(3001);
	inet_aton("127.0.0.1", &(myself.sin_addr));

	//Abre a porta
	if (bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
		socket_status = false;
		return;
	}

	//Recebe dados nessa porta
	listen(socket_fd, 2);                 
	connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
	socket_status = true;
	transmissionRunning = true;
	std::thread newthread(&Transmission::threadTransmission, this);
	kb_thread.swap(newthread);
}

bool Transmission::getSocketStatus(){
	return this->socket_status;
}

bool Transmission::getTransmissionStatus(){
	return this->transmissionRunning;
}

void Transmission::threadTransmission(){

	char * string = new char[sizeof(DataScreen)];

	while (1) {
		data->serialize(string);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		send(connection_fd, string, sizeof(DataScreen), 0);
	}

	this->transmissionRunning = false;
	return;
}

DataScreen Transmission::getDataScreen(){
	return (this->data)->getData();
}

void Transmission::update(Ball *ball, int displacement){
	data->setData(ball->get_xAxis(), ball->get_yAxis(), displacement);
}

void Transmission::stop(){
	(this->kb_thread).join();
	close(socket_fd);
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