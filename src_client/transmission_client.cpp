#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <cstring>
#include <string>

#include "model_client.hpp"
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
	running = 1;
	socketStatus = true;
}

void Transmission::init(){
	//Thread to transmit data
	transmissionRunning = true;
	std::thread newthread(&Transmission::threadTransmission, this);
	kbThread.swap(newthread);
}

void Transmission::threadTransmission(){
	
	std::cout << "Communicating with server..." << '\n';
	int msglen;
	while(1){ 
		recv(socketFd, inputBuffer, 120, 0);
		unserialize(inputBuffer);
		std::cout << "xAxis: " << data.xAxis << "- yAxis: "<< data.yAxis << '\n';
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		serialize(outputBuffer);
		msglen = send(socketFd, outputBuffer, 120, 0);
		if(msglen < 0){
			std::cout << "Error!" << '\n';
			break;
		}
	}
	transmissionRunning = false;
	return;
}

bool Transmission::getSocketStatus(){
	return this->socketStatus;
}

Data Transmission::getData(){
	return this->data;
}

void Transmission::updatePaddle(int *positionPaddle){
	for (int i = 0; i < SIZE_PADDLE; ++i){
		data.positionPaddle[i] = positionPaddle[i];
	}
	return ;
}

void Transmission::serialize(char *inputBuffer) {
	std::memcpy((void*)inputBuffer, &(this->data), sizeof(Data));
}

void Transmission::unserialize(char *outputBuffer) {
	std::memcpy(&(this->data), (void*)outputBuffer, sizeof(Data));
}

void Transmission::stop(){
	data.running = 0;
	kbThread.join();
	close(socketFd);
	return;
}