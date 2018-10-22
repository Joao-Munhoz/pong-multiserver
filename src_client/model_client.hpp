#ifndef MODEL_CLIENT_HPP
#define MODEL_CLIENT_HPP

#include <thread>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

//Screen Size
#define SCREEN_WIDTH  70
#define SCREEN_HEIGHT 20
#define MARGIN 		  2

//Velocity and Size of Bar
#define HALF_BAR      5
#define VEL_BAR       4

//Initial and Increase Velocity
#define INI_BALL_VELOCITY      0
#define INCREASE_BALL_VELOCITY 1.005

#define POINTS_WIN  40

//Times
#define SLEEP_TIME	50
#define WAIT_TIME 	1500

class Ball {
private:
	float velocityX;
	float velocityY;
	float xAxis;
	float yAxis;

public:
	Ball(float velocityX, float velocityY, float xAxis, float yAxis);
	void update(float new_posX, float new_posY, float new_velX, float new_velY);
	float get_velocityX();
	float get_velocityY();
	float get_xAxis();
	float get_yAxis();
};

class Particle {
private:
	int position;
	char iconPaddle;

public:
	Particle(int position);
	void update(int displacement);
	int get_position();
	char get_char();
};

class Paddle {
private:
	std::vector<Particle*> *particles;

public:
	Paddle();
	void init();
	void hard_copy(Paddle *ldc);
	void update(int displacement);
	std::vector<Particle*> *get_particles();
};

class Screen {
private:
	Ball *actualBall, *lastBall;
	Paddle *actualPaddle;
	Paddle *lastPaddle;
	int maxI, maxJ;
	float maxX, maxY;

public:
	Screen(Ball *ball, Paddle *paddle, int maxI, int maxJ, float maxX, float maxY);
	~Screen();
	void stop();
	void init();
	void draw();
	void update();
	void end_game(std::string &s);
};

void threadfun (char *keybuffer, int *control);

class Teclado {
private:
	char ultima_captura;
	int rodando;

	std::thread kb_thread;

public:
	Teclado();
	~Teclado();
	void stop();
	void init();
	char getchar();
};

struct DataScreen {
	int xAxis;
	int yAxis;
	int displacementPaddle;
};

class RelevantData {
private:
	DataScreen data;

public:
	RelevantData(int xAxis, int yAxis, int displacementPaddle);
	RelevantData(char *string);
	void serialize(char *string);
	void unserialize(char *string);
	DataScreen getData();
};

class Transmission {
private:
	int socket_fd;
	struct sockaddr_in target;
	bool socket_status, transmissionRunning;
	RelevantData *data;
	std::thread kb_thread;
	char * string;

public:
	Transmission();
	void threadTransmission();
	bool getSocketStatus();
	bool getTransmissionStatus();
	DataScreen getDataScreen();
	void stop();
};

#endif
