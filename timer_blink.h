#ifndef TIMER_BLINK_H
#define TIMER_BLINK_H

#include <Arduino.h>

/****************************Class Timer*****************************/
class Timer {
  public:
    Timer(uint16_t period = 1000);
    ~Timer();
    bool ready(void);
    void setPeriod(uint16_t period);

  private:
    uint32_t _tmr = 0;
    uint16_t _period;
};

/****************************Class Blink*****************************/
class Blink : public Timer {
  public:
    Blink(uint16_t period = 1000);
    ~Blink();
    bool toggle(void);
    void reset (void);
    
  private:
    bool _flag = false;
    Timer blink;
};

#endif TIMER_BLINK_H
