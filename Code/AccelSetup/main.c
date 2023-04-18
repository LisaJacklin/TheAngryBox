/* EE 444 Embedded Systems
Project: Reverse Bop-it
AccelBegin.c

Objective: get the accelerometer to operate correctly and as expected for the 

*/

#include <msp430.h>

void main(void)
{
////////////setting up clock rates///////////////////
UCSCTL1 = DCORSEL_3; //FREQUENCY RANGE TO INCLUDE 2MHZ WITHOUT VCORE
UCSCTL2 = 61; //DIVIDER VALUE
P7SEL |= (BIT0 + BIT1); //EXTERNAL XT1 IN USE
UCSCTL3 = 0; //USING THE EXTERNAL CCRYSTAL ASS THE REFERENCE CLOCK

UCSCTL4 = SELS_3 + SELM_3; //SET TO REFERENCE 

   do {
      UCSCTL7 &= ~(XT1LFOFFG + DCOFFG);                   //clear the XT1 and DCO flags
      SFRIFG1 &= ~OFIFG;                                  //clear the OSC fault flag
      } while ((SFRIFG1 & OFIFG));

   UCSCTL6 &= ~XT1OFF;                            //MAKES SURE THAT THIS EXTERNAL CLOCK IS ON

   //CHECK TO MAKE SURE THAT THE FREQUENCY IS CORRECT BEFORE CONTINUING ON


////////////////////////////////////////////////////
///////SINCE WE HAVE TO SET UP THE USCI/I2C MODULE TO WORK WITH THE ACCELERATOR//////////////
UCB1CTL0 |= UCSWRST;  //STARTS THE SET STATE

//////BEGINNING WITH THE MASTER/////////////
/////////PINS FOR THE TRANSMIT AND RETRIEVAL
UCB1CTLW0 |= UCMODE_3 + UCMST; //SET TO I2C MODE AND MASTER MODE

//NOW TO SETUP VALUES FOR DATA PROTOCOL

//BAUD RATE
UCB1BRW = ; //baud rate register.... probably have to go through this another way like in UART

UCB1CTLW1 = UCASTP_2; //STOP ASSERTION
UCB1TBCNT = 0X07; //SEVEN BYTES OF DATA TO TRANSMIT
UCB1I2CSA = 0X0012; //ADDRESS OF THE SLAVE (CHECK IN THE ACCELEROMETER DATASHEET)

P2EL |= 0X03; //CONFIGUURE I2C PINS (THIS IS DEVICE SPECIFIC SO CHECK THIS!)

UCB1CTL1 &=~UCSWRST; //NOW WE ARE IN THE RESET STATE.

////////////////////////////////////////////
_EINT();
LPM0;
}

/////////////ISR FOR THE BUTTON////////////////

