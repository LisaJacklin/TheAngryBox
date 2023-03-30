/*
EE 444 Embedded Systems 
Group 4 Project: Reverse Bop-it

LCD_testing.c
Coded by: Lisa Jacklin

*/

#include <msp430.h>
#include <hal_lcd.h> //LCD library header file

void main(void)
{
  //first, you must initialize the LCD screen
  halLcdInit();
  //then you must initialize the backlight
  halLcdBackLightInit();

  //now to set backlight level and contast level
  halLcdSetBackLight();// this takes a unsigned char
  halLcdSetContrast(); //also takes an unsigned char

  /*
  //Since we are trying to save power, there are several function prototypes to attempt and use
  halLcdShutDown();
  halLcdShutDownBackLight();
  halLcdStandby();
  halLcdActive();

  //items that could help to clear anything to send another message (or like up for 5, then down till prompted)
  halLcdClearScreen();

  */

}
