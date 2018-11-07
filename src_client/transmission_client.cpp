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
		recv(socketFd, inputBuffer, 300, 0);
		unserialize(inputBuffer);
		std::cout << "xAxis: " << data.xAxis << "- yAxis: "<< data.yAxis << '\n';
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << "Paddle values: " << data.id << '\n';
		for (int i = 0; i < SIZE_PADDLE; i++){
			std::cout << newData.paddles[data.id].position[i] << ' ';
		}
		std::cout << '\n';	
		serialize(outputBuffer);
		msglen = send(socketFd, outputBuffer, 300, 0);
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
		newData.paddles[data.id].position[i] = positionPaddle[i];
	}
	return ;
}

void Transmission::serialize(char *buffer) {
	std::memcpy((void*)buffer, &(this->newData), sizeof(Data));
}

void Transmission::unserialize(char *buffer) {
	std::memcpy(&(this->data), (void*)buffer, sizeof(Data));
}

void Transmission::stop(){
	data.running = 0;
	kbThread.join();
	close(socketFd);
	return;
}