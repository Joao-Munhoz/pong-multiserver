#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>

#include "model_client.hpp"

using namespace std::chrono;

/*
class Keyboard {
	private:
		char ultima_captura;
		int rodando;

	public:
		Keyboard();
		~Keyboard();
		void stop();
		void init();
		char getchar();
}

*/

void threadfun (char *keybuffer, int *control)
{
	char c;
	while ((*control) == 1) {
		c = getch();
		if (c!=ERR) (*keybuffer) = c;
		else (*keybuffer) = 0;
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
	}
	return;
}

Keyboard::Keyboard() {
}

Keyboard::~Keyboard() {
}

void Keyboard::init() {
	
	// Inicializa ncurses
	raw();				          		/* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 			/* We get F1, F2 etc..		*/
	noecho();			            	/* Don't echo() while we do getch */
	curs_set(0);           		  /* Do not display cursor */
	nodelay(stdscr, TRUE);

	this->rodando = 1;
	std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
	(this->kb_thread).swap(newthread);
}

void Keyboard::stop() {
	this->rodando = 0;
	(this->kb_thread).join();
	endwin();
}

char Keyboard::getchar() {
	char c = this->ultima_captura;
	this->ultima_captura = 0;
	return c;
}