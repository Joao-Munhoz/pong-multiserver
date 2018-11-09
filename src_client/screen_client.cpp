#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>

#include "model_client.hpp"

using namespace std::chrono;

Screen::Screen(Ball *ball, Paddle *paddle, int maxI, int maxJ, float maxX, float maxY) {
	this->actualBall = ball;
	this->lastBall = new Ball(ball->get_velocityX(),\
		ball->get_velocityY(),\
		ball->get_xAxis(),\
		ball->get_yAxis());
	this->actualPaddle = new Paddle[6];
	this->lastPaddle = new Paddle[6]; 
	for (int i = 0; i < MAX_CONNECTIONS; ++i){
		this->actualPaddle[i] = paddle[i];
		this->lastPaddle[i].hard_copy(this->actualPaddle[i])
	}
	this->maxI = maxI;
	this->maxJ = maxJ;
	this->maxX = maxX;
	this->maxY = maxY;
}

void Screen::init() {
  initscr();			      	 /* Start curses mode 		*/
  raw();				           /* Line buffering disabled	*/
  curs_set(0);           	 /* Do not display cursor */
  erase();
}

//Function to draw static interface
void Screen::draw() {

	for(int i = 0; i < this->maxI; i++){
		for(int j = 0; j < this->maxJ; j++){
			if(i == 0 || i == this->maxI - 1 || i == (this->maxI)/2){
				move(i, j);
				echochar('_');
			} else if(j == 0 || j == this->maxJ - 1 || j == (this->maxJ)/2){
				move(i, j);
				echochar('|');
			}
		}
	}
}

void Screen::update() {

   //Apaga a barra na tela
	for(int j = 0; j < MAX_CONNECTIONS; j++){
		for (int l = 0; l < (*(lastPaddle[j]->get_particles())).size(); ++l){
			if(idPaddle == 0 || idPaddle == 3)
			move( SCREEN_HEIGHT  , (*(lastPaddle[j]->get_particles()))[l]->get_position());
			echochar(' ');
		}
	}

    //Desenha barra na tela
	for (int k = 0; k < (*(actualPaddle[k]->get_particles())).size(); ++k){
		move( SCREEN_HEIGHT  , (*(actualPaddle[k]->get_particles()))[k]->get_position());
		echochar('_');

		(*(lastPaddle->get_particles()))[k]->update((*(actualPaddle->get_particles()))[k]->get_position());
	}

	int i, j;

    //Apaga ball da tela
	i = (int) lastBall->get_xAxis() * \
	(this->maxI / this->maxX);
	j = (int) lastBall->get_yAxis() * \
	(this->maxJ / this->maxY);


	if(i > 0 && i <= this->maxX){
		if(j > 0 && j < this->maxY - 1){
    		move(i, j);                 /* Move cursor to position */
      	 addch(' ');              /* Prints character, advances a position */
		}
	}

    // Desenha ball na tela
	i = (int) actualBall->get_xAxis() * \
	(this->maxI / this->maxX);

	j = (int) actualBall->get_yAxis() * \
	(this->maxJ / this->maxY);

	if(i > 0 && i <= this->maxX){
		if(j > 0 && j < this->maxY - 1){
   		move(i, j);   /* Move cursor to position */
         addch('o');  /* Prints character, advances a position */
		}
	}

    // Atualiza corpos antigos
	lastBall->update(actualBall->get_xAxis(), actualBall->get_yAxis(), actualBall->get_velocityX(), actualBall->get_velocityY());

	move(2,5);
	//printw("Score:%d", actualBall->get_points());

  // Atualiza tela
	refresh();
}

//void Screen::updatePaddles();

void Screen::end_game(std::string &s){
	clear();
	move((int)(SCREEN_HEIGHT/2), (int)(SCREEN_WIDTH/2) - (int)s.length()/2);
	printw("%s", s.c_str());
	refresh();  	
}

void Screen::stop() {
	endwin();
}

Screen::~Screen() {
	this->stop();;
}