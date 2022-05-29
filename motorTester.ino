/* Motor Tester ver. 1.0
*   Stepper Motor: MITSUMI M42SP-7
*   Step Angle   : 7.5 degree
*
*  Driver: A4988
*    ENBL: LOW - On, HIGH - Off
*    DIR : HIGH - ClockWise, LOW - CounterClockWise 
*/

#include "setup.h"                                                                                                                     
#include <Adafruit_SSD1306.h>
#include <GyverEncoder.h>
#include "Motor.h"
#include "interrupt_timer1.h"

Motor motor(STEP_PIN, DIR_PIN, ENBL_PIN);
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);
Encoder encoder(SLK, DT, SW);

void setup() {
  //Serial.begin(9600);
  initInterrupt();
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN,  INPUT_PULLUP);
  pinMode(BUTTON_RESET_PIN, INPUT_PULLUP);
  pinMode(TERM_SW_PIN,      INPUT_PULLUP);

  encoder.setType(TYPE2);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDR)) {
    Serial.println(F("SSD1306 allocation failed."));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
}

uint8_t huge = 100;
uint8_t score = 0;
uint32_t tmr;

void loop() {
  encoder.tick();
  if(encoder.isRight()) {
    impulse += huge;
  }
  else if(encoder.isLeft()) {
    if(impulse < MIN_) impulse = MIN_;
    else impulse -= huge;
  }
  if(encoder.isClick()) {
    if(score == 2) {huge = 100; score = 0;}
    else {huge = huge / 10; score++;}
  }

  if(!digitalRead(BUTTON_RIGHT_PIN)) {
    motor.setForward();
    motor.enableOn();
  }else if(!digitalRead(BUTTON_LEFT_PIN)) {
    motor.setReverse();
    motor.enableOn();
  }else{
      motor.stop();
  }
  if(!digitalRead(BUTTON_RESET_PIN)) steps = 0;
  if(!digitalRead(TERM_SW_PIN)) motor.stop();
  
  if(millis() - tmr >= 100){
    draw();
    tmr = millis();
  }
  
  motor.execute();
}

void draw(void) {
  display.clearDisplay();
  display.setCursor(3, 10);
  display.print(F("Speed: "));
  display.setCursor(40, 10);
  display.print(impulse);

  display.setCursor(3, 20);
  display.print(F("Steps: "));
  display.setCursor(40, 20);
  display.print(steps);
  display.display();
}
