/*
EE 444 Project Team 4
Reverse Bop-it
4/6/2023

LCDw2Buttons.c
Coded by: Lisa Jacklin
Objective: setting up interrupts to display different messages based on two different buttons that are being pressed

Operations:
- button press working correctly to light up an LED and LCD screen is initialized.
-

*/

#include <msp430.h>
#include <hal_lcd.h>

void main(void) {

////////////////////LCD SCREEN SETUP//////////////////////////////

halLcdInit();
halLcdBackLightInit();

//we had previously discussed adjusting this to work 
halLcdSetBackLight(10);

//now to setup the lcd to be in standby so it can be used later in the ISR
halLcdStandby();

////////////////////BUTTON SETUP//////////////////////////////

 //BUTTON SETUP
    P2DIR &= ~BIT7 + BIT6;                    //sets to input direction
    P2IE |= BIT7 + BIT6;                      // interrupts are allowed for this button                  
    P2REN = BIT7 + BIT6;                      //input with pull up resistor 
    P2OUT = BIT7 + BIT6;                      //also required to enable pull up resistor.

 //FLASHING LED TO TEST THE ISR BEFORE/DURING LCD SCREEN WORK
  P1DIR |= BIT0;                             //pin set to output 
  P1SEL &= ~BIT0;                            //set pin to I/O mode


//THIS MUST STAY HERE AT THE END OF THE PROGRAM
 _EINT();
 LPM0;

}
// BUTTON INTERRUPT TO TEST THE LCD SCREEN WITH
void Interrupt_Button(void) __interrupt [PORT2_VECTOR] {
  //button 2.7

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
      P2IFG &= ~BIT7; // Clear interrupt flag for P2.7
    }  

  /*
      //button 2.6
    if (P2IV == P2IV_P2IFG6) {
     //toggling LED to show the LCD is now active
       P1OUT ^= BIT0;    
         halLcdActive();
       P1OUT &=~ BIT0;                         

       halLcdPrint("LEAVE ME BE ", 'B'); //won't lie, no idea why the b but a style text char is needed so B it is

      __delay_cycles(600000);

       //and toggling LED to display the cleared screen 
       P1OUT ^= BIT0;    
             halLcdClearScreen();
       P1OUT &=~ BIT0;  
        // halLcdStandby();
        P2IFG &= ~BIT6; // Clear interrupt flag for P2.6
    }

    */

     
}
