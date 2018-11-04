#ifndef OO_MODEL_HPP
#define OO_MODEL_HPP

#include <thread>
#include <vector>

//Screen Size
#define SCREEN_WIDTH  70
#define SCREEN_HEIGHT 20
#define MARGIN 		  2

//Velocity and Size of Paddle
#define SIZE_PADDLE			10
#define VELOCITY_PADDLE		4

//Initial and Increase Velocity
#define BALL_VELOCITY		5
#define INCREASE_VELOCITY	1.005

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

class Physics {
	private:
		Ball *ball;

	public:
		Physics(Ball *ball);
		void update(float deltaT);
		int get_position();	
};

#endif
