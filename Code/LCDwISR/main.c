/*
EE 444 Project Team 4
Reverse Bop-it
4/6/2023

LCDwISR.c
Coded by: Lisa Jacklin
Objective: learn to use the LCD screen on the MSP430 with a simple button interrupt to display a message

Operations:
- button press working correctly to light up an LED and LCD screen is initialized.

*/

#include <msp430.h>
#include <hal_lcd.h>

//for testing purposes, incrementing the Vcore could be performed....

int time = 0;

void main(void) {


//basics for the LCD screen
////////////////////LCD SCREEN SETUP//////////////////////////////

halLcdInit();
halLcdBackLightInit();

//we had previously discussed adjusting this to work 
halLcdSetBackLight(10);
//halLcdSetContrast(45);

//now to setup the lcd to be in standby so it can be used later in the ISR
halLcdStandby();

////////////////////BUTTON SETUP//////////////////////////////

 //BUTTON SETUP
    P2DIR &= ~BIT7;                           //sets to input direction
    P2IE |= BIT7;                             // interrupts are allowed for this button                  
    P2REN = BIT7;                             //input with pull up resistor 
    P2OUT = BIT7;                             //also required to enable pull up resistor.

 //FLASHING LED TO TEST THE ISR BEFORE/DURING LCD SCREEN WORK
  P1DIR |= BIT0;                             //pin set to output 
  P1SEL &= ~BIT0;                            //set pin to I/O mode


//THIS MUST STAY HERE AT THE END OF THE PROGRAM
 _EINT();
 LPM0;

}
// BUTTON INTERRUPT TO TEST THE LCD SCREEN WITH
void Interrupt_Button(void) __interrupt [PORT2_VECTOR] {

    if (P2IV == P2IV_P2IFG7){
      //toggling LED to show the LCD is now active
       P1OUT ^= BIT0;    
         halLcdActive();
       P1OUT &=~ BIT0;                         

       halLcdPrint("STOP POKING ME ", 'B'); //won't lie, no idea why the b but a style text char is needed so B it is

      __delay_cycles(600000);

       //and toggling LED to display the cleared screen 
       P1OUT ^= BIT0;    
             halLcdClearScreen();
       P1OUT &=~ BIT0;  
        // halLcdStandby();
    }   
}
