#ifndef MOTOR_H
#define MOTOR_H
//#define DEBUG
#include <Arduino.h>

class Motor {
  public:
	  Motor  (const uint8_t step, 
	          const uint8_t dir ,
	          const uint8_t enb);
	  ~Motor ();
	  void enableOn	    (void);
	  void enableOff    (void);
	  void setForward   (void);
	  void setReverse   (void);
	  void stop	    (void); 
	  bool getEnable    (void);
	  bool getDirection (void);
	  void execute	    (void);
   
  private:
    void run		    (void);
	
	  uint8_t step_pin;
	  uint8_t dir_pin;
	  uint8_t enable_pin;
	   bool enable;
	   bool dir;
};

#endif MOTOR_H
