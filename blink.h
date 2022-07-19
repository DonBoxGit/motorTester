#ifndef BLINK_H
#define BLINK_H

class Blink {
  public:
    Blink(uint16_t period = 1000);
    ~Blink();
    bool ready(void);
    void reset(void);
    
  private:
    uint32_t _tmr = 0;
    uint16_t _period;
    bool _flag = false;
};

Blink::Blink(uint16_t period) : _period(period) {}

Blink::~Blink() {}

bool Blink::ready() {
  if(millis() - _tmr >= _period) {
    _tmr = millis();
    _flag = !_flag;
  }
  return _flag;
}

void Blink::reset() {
  _flag = false;
}

#endif BLINK_H
