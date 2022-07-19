#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
  private:
    uint32_t _tmr = 0;
    uint16_t _period;
    
  public:
    Timer(int period = 1000) : _period(period) {}
    bool ready(void) {                                // Возвращаем true когда период достиг своего значения
      if (millis() - _tmr >= _period) {
        _tmr = millis();
        return true;
      }
      return false;
    }
};

#endif TIMER_H
