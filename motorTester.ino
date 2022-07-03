/* Motor Tester ver. 1.2
     Stepper Motor: MITSUMI M42SP-7
     Step Angle   : 7.5 degree

   Driver: A4988
      ENBL: LOW - On, HIGH - Off
      DIR : HIGH - ClockWise, LOW - CounterClockWise
      
   Add Timer Class timer.h
   Improve Motor Class
*/

#include "config.h"
#include <Adafruit_SSD1306.h>
#include <EncButton.h>
#include "Motor.h"
#include "interrupt_timer_1.h"
#include "timer.h"


Timer drawTimer(50);
Timer rStatusTimer(500);

Motor motor(STEP_PIN, DIR_PIN, ENBL_PIN);

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);

EncButton<EB_TICK, DT, SLK, SW> encoder;
EncButton<EB_TICK, BUTTON_RIGHT_PIN> right_btn(INPUT_PULLUP);
EncButton<EB_TICK, BUTTON_LEFT_PIN > left_btn (INPUT_PULLUP);
EncButton<EB_TICK, BUTTON_RESET_PIN> reset_btn(INPUT_PULLUP);

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  initInterrupt();
  //pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  //pinMode(BUTTON_LEFT_PIN,  INPUT_PULLUP);
  //pinMode(BUTTON_RESET_PIN, INPUT_PULLUP);
  pinMode(TERM_SW_PIN_1, INPUT_PULLUP);
  pinMode(TERM_SW_PIN_2, INPUT_PULLUP);
  pinMode(SPEAKER_PIN,   OUTPUT);

  encoder.setEncType(0);			      // Full Step Type Ecnoder
  attachInterrupt(0, isr, CHANGE);  // SLK
  attachInterrupt(1, isr, CHANGE);  // DT

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDR)) {
    Serial.println(F("SSD1306 allocation failed."));
    for (;;);
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);
  startMenu();
}

void isr() {
  encoder.tickISR();  // в прерывании вызываем тик ISR
}

uint8_t  subtrahend = 100;
int8_t   score      = 0;
bool blinkFlag = true;

void loop() {
  encoder.tick();
  
  if (encoder.left()) {
    score--;
    if (score < 0) score = 1;
    startMenu();
  }
  
  if (encoder.right()) {
    score++;
    if (score > 1) score = 0;
    startMenu();
  }

  if (encoder.click()) {
    while (true) {
      encoder.tick();
      right_btn.tick();
      left_btn.tick();
      reset_btn.tick();

      if (encoder.right()) {
        impulse += subtrahend;
      }
      
      else if (encoder.left()) {
        if (impulse < MIN_) impulse = MIN_;
        else impulse -= subtrahend;
      }
      
      if (encoder.click()) {
        if (score == 2) { subtrahend = 100; score = 0; }
        else            { subtrahend /= 10; score++;   }
      }

      if (right_btn.press()) {
        motor.setForward();
        motor.enableOn();
      } else if (right_btn.release()) {
        motor.enableOff();
      }

      if (left_btn.press()) {
        motor.setReverse();
        motor.enableOn();
      } else if (left_btn.release()) {
        motor.enableOff();
      }

      if (reset_btn.click()) steps = 0;

      if (!digitalRead(TERM_SW_PIN_1) || !digitalRead(TERM_SW_PIN_2)) motor.stop();

      if (drawTimer.ready()) mainMenu();

      motor.execute();
    }
  }
}

void startMenu(void) {
  display.clearDisplay();

    //display.drawRoundRect(0, 0, 128, 32, 0, WHITE);
  display.setCursor(26, 5);
  display.print(F("SELECT DRIVER"));

  switch (score) {
    case 0:
      display.fillRoundRect(25, 16, 35, 11, 3, WHITE);
      display.setCursor(28, 18);
      display.setTextColor(BLACK);
      display.print(F("A4988"));
      display.setTextColor(WHITE);
      
      display.setCursor(68, 18);
      display.print(F("TB6600"));
    break;

    case 1:
      display.setCursor(28, 18);
      display.print(F("A4988"));
      display.fillRoundRect(64, 16, 42, 11, 3, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(68, 18);
      display.print(F("TB6600"));
      display.setTextColor(WHITE);
    break;
  }
  display.display();
}


void mainMenu(void) {
  display.clearDisplay();

  display.setCursor(3, 10);
  display.print(F("Speed: "));
  display.setCursor(40, 10);
  display.print(impulse);
  display.setCursor(68, 10);
  display.print(F("-"));

  switch (score) {
    case 0:
      display.fillRoundRect(77, 8, 20, 11, 3, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(78, 10);
      display.print("100");
      display.setTextColor(WHITE);
      display.setCursor(100, 10);
      display.print("10");
      display.setCursor(118, 10);
      display.print(F("1"));
      break;

    case 1:
      display.setCursor(78, 10);
      display.print("100");
      display.fillRoundRect(98, 8, 16, 11, 3, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(100, 10);
      display.print("10");
      display.setTextColor(WHITE);
      display.setCursor(118, 10);
      display.print(F("1"));
      break;

    case 2:
      display.setCursor(78, 10);
      display.print("100");
      display.setCursor(100, 10);
      display.print("10");
      display.fillRoundRect(115, 8, 11, 11, 3, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(118, 10);
      display.print(F("1"));
      display.setTextColor(WHITE);
      break;
  }
  display.setCursor(55, 25);
  display.print(F("Steps: "));
  display.setCursor(92, 25);
  display.print(steps);
  
  if (!motor.getEnable()) {
    if (rStatusTimer.ready()) blinkFlag = !blinkFlag;
    if (motor.getDirection()) {
      if (blinkFlag) {
        tone(SPEAKER_PIN, FREQUENCY_SP, DURATION_SP);
        display.fillRoundRect(1, 21, 46, 11, 3, WHITE);
        display.setTextColor(BLACK);
        display.setCursor(4, 23);
        display.print("FORWARD");
      }
    } else {
      if (blinkFlag) {
        tone(SPEAKER_PIN, FREQUENCY_SP, DURATION_SP);
        display.fillRoundRect(1, 21, 46, 11, 3, WHITE);
        display.setTextColor(BLACK);
        display.setCursor(4, 23);
        display.print("REVESRE");
      }
    }
  } else {
    if (!digitalRead(TERM_SW_PIN_1)) {
      display.fillRoundRect(1, 21, 41, 11, 3, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(4, 23);
      display.print("TERM-1");
    } else if (!digitalRead(TERM_SW_PIN_2)) {
      display.fillRoundRect(1, 21, 41, 11, 3, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(4, 23);
      display.print("TERM-2");
    } else {
      display.fillRoundRect(5, 21, 31, 11, 3, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(9, 23);
      display.print("STOP");
    }
  }
  display.setTextColor(WHITE);
  display.display();
}
