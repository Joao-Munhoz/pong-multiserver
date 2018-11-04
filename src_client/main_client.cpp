/* 
 * Projeto Jogo Pong-Solo Version: 1.03x
 * Utima atualizacao: 07/09/2018 - 18:16
 *
 * Leonardo Rodrigues Marques - 178610
 * Joao Victor da Conceicao Munhoz - 160174
 *
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "model_client.hpp"
#include "transmission_client.hpp"

using namespace std::chrono;

uint64_t get_now_ms() 
{
	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main(){ 

	Ball *ball = new Ball(BALL_VELOCITY , BALL_VELOCITY , (int)(SCREEN_HEIGHT/2), (int)(SCREEN_WIDTH/2));
	Paddle *paddle = new Paddle();
	paddle->init();
	Screen *screen = new Screen(ball, paddle, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
	Keyboard *keyboard = new Keyboard();
	Transmission *transmission = new Transmission();
	if(transmission->getSocketStatus() == false){
		return 1;
	}
	//transmission->init();

	while(1){}

	Data newData = transmission->getData();
	ball->update(newData.xAxis, newData.yAxis);
  	/*screen->init();
	screen->draw();

	while(transmission->getTransmissionStatus()){
		keyboard->getchar()
		DataScreen data = transmission->getDataScreen();
		ball->update(data.xAxis, data.yAxis, 0, 0);
		paddle->update(data.displacementPaddle);
		screen->update();
		data.displacementPaddle = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

		/*if (key == 'a') {
			if((*(barra->get_particulas()))[0]->get_posicao() > VEL_BAR){
					transmission->update(ball, -1*VEL_BAR);
			}
		} else if(key == 'd'){
			if((*(barra->get_particulas()))[0]->get_posicao() + 2*HALF_BAR < SCREEN_WIDTH - VEL_BAR){ 
				transmission->update(ball, VEL_BAR);
			}
		} else if(key == 'q'){
			break;
		} else {
			transmission->update(ball, 0);
		}
		*/

	transmission->stop();
	screen->stop();

	return 1;
}