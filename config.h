#ifndef CONFIG_H
#define CONFIG_H

#define STEP_PIN 6
#define DIR_PIN  7
#define ENBL_PIN 8

#define DT  2         // S1
#define SLK 3         // S2
#define SW  4         // Key

#define SPEAKER_PIN         11
#define FREQUENCY_SP        1100
#define DURATION_SP         140

#define BUTTON_RIGHT_PIN    A0
#define BUTTON_LEFT_PIN     A1
#define BUTTON_RESET_PIN    10
#define TERM_SW_PIN_1       9
#define TERM_SW_PIN_2       12

#define OLED_RESET         -1
#define DISPLAY_I2C_ADDR    0x3C
#define DISPLAY_WIDTH       128
#define DISPLAY_HEIGHT      32

#define MIN_ 60   // Max Speed
#define MAX_ 3000 // Min Speed

#endif CONFIG_H
