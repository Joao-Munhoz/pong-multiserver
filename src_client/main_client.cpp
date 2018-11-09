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

	int start = -1;
	Ball *ball = new Ball(BALL_VELOCITY , BALL_VELOCITY , (int)(SCREEN_HEIGHT/2), (int)(SCREEN_WIDTH/2));
	Paddle *paddle = new Paddle[6];;
	for(int i = 0; i < 6; i++){
		paddle[i].init(i);
	}
	Screen *screen = new Screen(ball, paddle, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
	Keyboard *keyboard = new Keyboard();
	Transmission *transmission = new Transmission();
	if(transmission->getSocketStatus() == false){
		return 1;
	}

	transmission->init();
	while(data.id == -1){
		Data data = transmission->getData();
	}
	screen->init();
	screen->draw();


	while(transmission->getTransmissionStatus()){
		keyboard->getchar(); 
		if (key == 'a') {
			paddle->update(VEL_BAR, data.id);			
		} else if(key == 'd'){
			paddle->update(-VEL_BAR, data.id);
			
		} else if(key == 'q'){
			break;
		} 
/*if((*(barra->get_particulas()))[0]->get_posicao() > VEL_BAR){
					transmission->update(ball, -1*VEL_BAR);
			}
if((*(barra->get_particulas()))[0]->get_posicao() + 2*HALF_BAR < SCREEN_WIDTH - VEL_BAR){ 
				transmission->update(ball, VEL_BAR);
			}
*/

		ball->update(data.xAxis, data.yAxis, 0, 0);
		screen->update();
		transmission->update();
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));		
		
}
	transmission->stop();
	screen->stop();

	return 1;
}