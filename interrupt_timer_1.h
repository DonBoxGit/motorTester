#ifndef INTERRUPT_TIMER_1_H
#define INTERRUPT_TIMER_1_H

volatile int16_t  impulse = 3000;
volatile uint16_t steps   = 0;

void initInterrupt() {
  cli();                                    // Запрет прерываний
  TCCR1A = 0;                               // Обнуляем регистры управления
  TCCR1B = 0;                               // Режим No Clock Source. (Timer/Counter Stopped)
  OCR1A = impulse;                          // Число для сравнения (timer_counts)
  TCCR1B |= (1 << WGM12);                   // Устанавливаем режим CTC (Сброс по совпадению с TCNTn)
  TCCR1B |= (1 << CS12);                    // Устанавливаем пределитель на 256
  TCNT1 = 0;                                // Cброс Счетного Регистра
  sei();                                    // Разрешить прерывания   
}

ISR (TIMER1_COMPA_vect) {
  OCR1A = impulse;
  digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
  if(digitalRead(STEP_PIN)) steps++;
}

#endif INTERRUPT_TIMER_1_H
