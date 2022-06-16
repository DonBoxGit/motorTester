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
  if(!((TIMSK1 >> OCIE1A) & 1)) {
	TIMSK1 |= (1 << OCIE1A);
  #ifdef DEBUG
  Serial.println("Motor TURN");
  #endif
  }
}

void Motor::stop() {
  if((TIMSK1 >> OCIE1A) & 1) {
	enable = HIGH;
	TIMSK1 &= ~(1 << OCIE1A);
  TCNT1 = 0x00;
  PORTD &= ~(1 << 6); // digitalWrite(step_pin, LOW);
  #ifdef DEBUG
  Serial.println("Motor STOP");
  #endif
  }
}

void Motor::execute() {
	if(!enable) {
		digitalWrite(dir_pin, dir);
		run();
	} else {
		stop();
	}
}

void Motor::enableOn() {
	enable = LOW;
  digitalWrite(enable_pin, enable);
}

void Motor::enableOff() {
	enable = HIGH;
  digitalWrite(enable_pin, enable);
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
