#include "stdafx.h"
#include "CMotor.h"
#include <pigpio.h>

#define HIGH 1
#define LOW 0

//GPIO RaspberryPi Pins 
#define AIN1 17
#define AIN2 27
#define PWMA 18

#define BIN1 22
#define BIN2 23
#define PWMB 12

#define STBY 24

CMotor::CMotor() {


	int status = gpioInitialise();
	if (status < 0) return;

	handle = spiOpen(0, 200000, 3);

	//Initialize Pins 
	gpioSetMode(AIN1, PI_OUTPUT);
	gpioSetMode(AIN2, PI_OUTPUT);
	gpioSetMode(PWMA, PI_OUTPUT);

	gpioSetMode(BIN1, PI_OUTPUT);
	gpioSetMode(BIN2, PI_OUTPUT);
	gpioSetMode(PWMB, PI_OUTPUT);

	gpioSetMode(STBY, PI_OUTPUT);
	gpioWrite(STBY, HIGH); 

	gpioWrite(AIN1, 0);
	gpioWrite(AIN2, 0);
	gpioPWM(PWMA, 0);

	gpioWrite(BIN1, 0);
	gpioWrite(BIN2, 0);
	gpioPWM(PWMB, 0);

}

CMotor::~CMotor() {

}

void CMotor::set_pwm_left(int val) {

	gpioWrite(AIN1, 1);
	gpioWrite(AIN2, 0);

	gpioPWM(PWMA, val);


}

void CMotor::set_pwm_right(int val) {

	gpioWrite(BIN1, 1);
	gpioWrite(BIN2, 0);

	gpioPWM(PWMB, val);

}

int CMotor::get_pwm_left() {

	return 0;

}

int CMotor::get_pwm_right() {

	return 0;

}

void CMotor::stop() {

	gpioPWM(PWMA, 0);
	gpioPWM(PWMB, 0);

}

void CMotor::forward(float time) {

	set_pwm_left(200);
	set_pwm_right(200);
	gpioDelay(time);
}

void CMotor::backward(float time) { //not yet

	set_pwm_left(200);
	set_pwm_right(200);
	gpioDelay(50000);

}

void CMotor::left(float time) {

	set_pwm_left(200);
	gpioDelay(50000);
}

void CMotor::right(float time) {

	set_pwm_right(200);
	gpioDelay(50000);
}