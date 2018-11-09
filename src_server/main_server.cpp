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

	Ball *ball = new Ball(BALL_VELOCITY, BALL_VELOCITY , (int)(SCREEN_HEIGHT/2), (int)(SCREEN_WIDTH/2));
	Physics *f = new Physics(ball);
	Transmission *transmission = new Transmission();
	if(transmission->getSocketStatus() == false){ 
		printf("Connection failed!\n");
		return 1;
	}

	transmission->waitConnections();
	transmission->init();
	
	uint64_t t0, t1, deltaT, T;
	uint64_t time0, time1;
	int i = 0;
	T = get_now_ms();
	t1 = T;
	int t2;

	while (1) {
		t0 = t1;
		t1 = get_now_ms();
		deltaT = t1-t0;
		f->update(deltaT);
		transmission->updateBall(ball->get_xAxis(), ball->get_yAxis());
		std::this_thread::sleep_for (std::chrono::milliseconds(SLEEP_TIME));
	}
	return 0;
}