/*
EE 444 Embedded Systems
Reverse Bop-it

Compiled full program
Team Members: 
  Audrey Eikenberry
  Jessica Pennock
  Lisa Jacklin
*/

#include <msp430.h>
#include <hal_lcd.h> //LCD library header file

char z; //z value (8-bits unsigned) for the z-axis of the accelerometer

void main(void) {
///////////////////SMCLK SETUP////////////////////////////////
UCSCTL1 |= DCORSEL_2;
UCSCTL2 |= 61; //2 MHz 
UCSCTL3 |=SELREF_XT1CLK; //INTERNAL CRYSTAL NOT AS STABLE
UCSCTL4 |= SELM__DCOKL +SELS__DCOCLKDIV;
////////////////////LCD SCREEN SETUP//////////////////////////////
halLcdInit();
halLcdBackLightInit();
halLcdSetBackLight(10);

//now to setup the lcd to be in standby so it can be used later in the ISR
halLcdStandby();
////////////////////BUTTON SETUP//////////////////////////////

 //BUTTON SETUP
    P2DIR &= ~BIT7 + BIT6;                    //sets to input direction
    P2IE |= BIT7 + BIT6;                      // interrupts are allowed for this button                  
    P2REN = BIT7 + BIT6;                      //input with pull up resistor 
    P2OUT = BIT7 + BIT6;                      //also required to enable pull up resistor.
//////////////////////SDA AND SCL BITS FOR ACCEL/////////////////
P3SEL = (BIT1 + BIT2);
P3DIR |= BIT1; //TRANSMIT OR OUTPUT SDA PIN
P3DIR &=~ BIT2; //RECIEVE OR INPUT SCL PIN

/////////////////////LEDS AS CHECKING////////////////////////
//LED FOR ACCELEROMETER
P1DIR |=BIT1;
P1OUT &=~BIT1;
//FOR BUTTON INPUT
P1DIR |= BIT0;                           
P1SEL &= ~BIT0;                          
/////////////SETTING UP THE SLAVE/MASTER //////////////////
UCB0CTL1 |= UCSWRST; 

UCB0CTL0 |= UCMST +UCMODE_3 + UCSYNC; //MASTER, I2C MODE, SYNC
//NOTE 7-BIT ADDRESS DEFAULT
UCB0CTL1 |= UCSSEL_3; //SMCLK

//BAUD RATE SET TO 20KHz
UCB0BR0 |=1000;
UCB0BR1 |=0;

UCB0CTL1 &=~ UCSWRST;
////////////INTERRUPTS AND POWER MODE ENABLES/////////////

//UCB0IE = UCRXIE + UCTXIE; //INTERRUPTS FOR I2C

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

