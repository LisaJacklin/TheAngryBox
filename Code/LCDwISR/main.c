/*
EE 444 Project Team 4
Reverse Bop-it
4/6/2023

LCDwISR.c
Coded by: Lisa Jacklin
Objective: learn to use the LCD screen on the MSP430 with a simple button interrupt to display a message

*/

#include <msp430.h>
#include <hal_lcd.h>  //lcd header file
//for testing purposes, incrementing the Vcore could be performed....

int ButtonPressed = 0;
int time = 0;

void main(void) {

//basics for the LCD screen
////////////////////LCD SCREEN SETUP//////////////////////////////

halLcdInit();
halLcdBackLightInit();

//now, to setup a button and frequency for the button using a past lab program
////////////////////BUTTON SETUP//////////////////////////////
//FREQUENCY
 UCSCTL1 = DCORSEL_6;                 //THIS SETS THE FREQUENCY RANGE
   UCSCTL2 = 762;                       //THIS SETS THE DIVISOR FOR FREQUENCY

   UCSCTL3 |= SELREF_3;                 //USING REFOCLK AS THE REFERENCED CLOCK
   UCSCTL4 = SELM_3 ;                  //SELM 2 SETS THE ACLK SOURCE TO REFOCLK 

   //TESTER BITS INCASE REQUESTED
   P11DIR |= BIT1;
   P11SEL |= BIT1;                      // this should set the functionality to periperial

   //SETTING UP EXTERNAL CRYSTAL AND STABALIZATION
   P7SEL |= 0X03;                       //XT1 CLOCK PINS
   UCSCTL6 &= ~(XT1OFF);                //CHECKING TO SEE IF THE CLOCK IS OFF
   UCSCTL6 |= XCAP_3;                   //LOAD CAPACITOR SPECIFYER

    do {
    UCSCTL7 &= ~(XT1LFOFFG + DCOFFG); //clear the XT1 and DCO flags
    SFRIFG1 &= ~OFIFG; //clear the OSC fault flag
    } while ((SFRIFG1 & OFIFG));

   //TIMER SETUP
   //sets timer A to depend on the ACLK and to be continuous 
   TA0CTL = TASSEL__SMCLK + MC__CONTINUOUS;

 //BUTTON SETUP
    P2DIR &= ~BIT7;                    //sets to input direction
    P2IE |= BIT7;                      // interrupts are allowed for this button                  
    P2REN = BIT7;                      //input with pull up resistor 
    P2OUT = BIT7;                      //also required to enable pull up resistor.

 //FLASHING LED TO TEST THE ISR BEFORE/DURING LCD SCREEN WORK
  P1DIR |= BIT0;                      //pin set to output 
  P1SEL &= ~BIT0;                      //set pin to I/O mode


//THIS MUST STAY HERE AT THE END OF THE PROGRAM
 _EINT();
 LPM0;

}
// BUTTON INTERRUPT TO TEST THE LCD SCREEN WITH
void Interrupt_Button(void) __interrupt [PORT2_VECTOR] {
    //push button is P2.7
    if (P2IV == P2IV_P2IFG7){
     P1OUT ^= BIT0;                   //this should toggle the LED in use
     }   

}