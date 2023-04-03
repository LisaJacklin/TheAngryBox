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
//we might need to use IncrementVcore.c again in this, but first, test!

  //first, you must initialize the LCD screen
  halLcdInit();
  //then you must initialize the backlight
  halLcdBackLightInit();

  //now to set backlight level and contast level
  //looks like the backlight is simply what makes it so the string is something we can see or not...
  halLcdSetBackLight(3);// this takes a unsigned char
  halLcdSetContrast(3); //also takes an unsigned char


  halLcdPrint("Hello World",'A' );

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
