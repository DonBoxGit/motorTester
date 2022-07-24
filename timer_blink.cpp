#include "timer_blink.h"

/****************************Class Timer*****************************/
Timer::Timer(uint16_t period)  : _period(period) {}

Timer::~Timer() {}

bool Timer::ready() {
  if (millis() - _tmr >= _period) {
    _tmr = millis();
    return true;
  }
      return false;
}

void Timer::setPeriod(uint16_t period) {
  _period = period;
}

/****************************Class Blink*****************************/
Blink::Blink(uint16_t period) {
  blink.setPeriod(period);
  }
  
Blink::~Blink() {}

bool Blink::toggle() {
  if(blink.ready()) _flag = !_flag;
  return _flag;
}

void Blink::reset() {
  _flag = false;
}
