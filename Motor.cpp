#include "Motor.h"

Motor::Motor(const uint8_t step, 
             const uint8_t dir , 
             const uint8_t enb) : step_pin  (step),
                                  dir_pin   (dir) ,
                                  enable_pin(enb) {
	pinMode(step_pin,   OUTPUT);
	pinMode(dir_pin,    OUTPUT);
	pinMode(enable_pin, OUTPUT);
	enableOff();
}

Motor::~Motor() {}

void Motor::run() {
	TIMSK1 |= (1 << OCIE1A);
}

void Motor::stop() {
	enable = HIGH;
	TIMSK1 &= ~(1 << OCIE1A);
}

void Motor::execute() {
	digitalWrite(enable_pin, enable);
	if(!enable) {
		digitalWrite(dir_pin, dir);
		run();
	}else{
		stop();
	}
}

void Motor::enableOn() {
	enable = LOW;
}

void Motor::enableOff() {
	enable = HIGH;
}

void Motor::setForward() {
  dir = HIGH;
}

void Motor::setReverse() {
	dir = LOW;
}

bool Motor::getEnable() {
  return enable;
}

bool Motor::getDirection() {
	return dir;
}
