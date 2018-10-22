#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>
#include <stdlib.h>

#include "model_client.hpp"

using namespace std::chrono;

Ball::Ball(float velocityX, float velocityY, float xAxis, float yAxis) {
	this->velocityX = velocityX;
	this->velocityY = velocityY;
	this->xAxis = xAxis;
	this->yAxis = yAxis;
}

void Ball::update(float new_posX, float new_posY, float new_velX, float new_velY) { 
	this->xAxis = new_posX;
	this->yAxis = new_posY;
	this->velocityX = new_velX;
	this->velocityY = new_velY;
}

float Ball::get_velocityX() {
	return this->velocityX;
}
float Ball::get_velocityY() {
	return this->velocityY;
}

float Ball::get_xAxis() {
	return this->xAxis;
}

float Ball::get_yAxis() {
	return this->yAxis;
}

Particle::Particle(int position) {
	this->position = position;
	this->iconPaddle = '_';
}

int Particle::get_position(){
	return this->position;
}
void Particle::update(int position){
	this->position = position;
}

Paddle::Paddle() {
	this->particles = new std::vector<Particle *>(0);
}

void Paddle::init() {
	int position = (int)(SCREEN_WIDTH/2) - HALF_BAR;
	for (int i = -(HALF_BAR); i <= HALF_BAR; ++i){
		(this->particles)->push_back(new Particle(position));
		position++;
	}
}

void Paddle::update(int displacement) {
	for(int i = 0; i < particles->size(); i++){
		((*(this->particles))[i])->update(((*(this->particles))[i])->get_position() + displacement);
	}
}

std::vector<Particle*> *Paddle::get_particles() {
	return (this->particles);
}

void Paddle::hard_copy(Paddle *ldc) {
	std::vector<Particle *> *particles = ldc->get_particles();

	for (int k = 0; k < particles->size(); k++) {
		Particle *c = new Particle( (*particles)[k]->get_position());
		(this->particles)->push_back(c);
	}
}
