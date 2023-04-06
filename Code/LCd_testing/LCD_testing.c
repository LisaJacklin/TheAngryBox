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
  //once initiated, the message will be displayed
  halLcdPrint("Hello World",'A' );

  //first, you must initialize the LCD screen
  halLcdInit();
  //then you must initialize the backlight
  halLcdBackLightInit();

  //similar to shut down, this simply makes it so the screen isn't 100% down...
  halLcdStandby();


 // __delay_cycles(100000);

  //this function will shut the screen down entirely!
  //when completely done with the required data, this should be done!
  // halLcdShutDown();


  /*
  //Since we are trying to save power, there are several function prototypes to attempt and use
  halLcdShutDownBackLight();
  halLcdStandby();
  halLcdActive();

  //items that could help to clear anything to send another message (or like up for 5, then down till prompted)
  halLcdClearScreen();

  //now to set backlight level and contast level
  //looks like the backlight is simply what makes it so the string is something we can see or not...
  //halLcdSetBackLight(5);// this takes a unsigned char
  // halLcdSetContrast(3); //also takes an unsigned char


  */

}
