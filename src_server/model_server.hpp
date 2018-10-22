#ifndef OO_MODEL_HPP
#define OO_MODEL_HPP

#include <thread>
#include <vector>

//Screen Size
#define SCREEN_WIDTH  70
#define SCREEN_HEIGHT 20
#define MARGIN 		  2

//Velocity and Size of Bar
#define HALF_BAR      5
#define VEL_BAR       4

//Initial and Increase Velocity
#define INI_BALL_VELOCITY      5
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
		int points;
		int seconds;

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
		void update(int nova_position);
		int get_position();
		char get_char();
};

class Paddle {
 private:
		std::vector<Particle*> *particles;

	public:
		Paddle();
		void hard_copy(Paddle *ldc);
		void update(int displacement);
		void init();
		std::vector<Particle*> *get_particles();
};

class Physics {
	private:
		Ball *ball;

	public:
		Physics(Ball *ball);
		void update(float deltaT);
		int get_position();	
};

#endif
