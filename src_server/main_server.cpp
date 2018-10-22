/* 
 * Projeto Jogo Pong-Server-Client Version: 2.03x
 * Utima atualizacao: 21/10/2018 - 18:53
 *
 * Leonardo Rodrigues Marques - 178610
 * Joao Victor da Conceicao Munhoz - 160174
 *
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string.h>

#include "model_server.hpp"
#include "transmission_server.hpp"

using namespace std::chrono;

uint64_t get_now_ms() {
	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main() {

	Ball *ball = new Ball(INI_BALL_VELOCITY , INI_BALL_VELOCITY , (int)(SCREEN_HEIGHT/2), (int)(SCREEN_WIDTH/2));
	Physics *f = new Physics(ball);
	Transmission *transmission = new Transmission();
	if(transmission->getSocketStatus() == false){ 
		printf("Connection failed!\n");
		return 1;
	}
	
	uint64_t t0, t1, deltaT, T;
	uint64_t time0, time1;
	int i = 0;
	T = get_now_ms();
	t1 = T;
	int t2;

	while (1) {
		
		// Atualiza timers
		t0 = t1;
		t1 = get_now_ms();
		deltaT = t1-t0;
		f->update(deltaT);

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

		std::this_thread::sleep_for (std::chrono::milliseconds(SLEEP_TIME));
		i++;
	}
	return 0;
}