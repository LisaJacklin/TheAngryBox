#include <msp430.h>

//variable to store value from z-component of accelerometer
char z = 0x05; //address for reading MSB out Z value(8-bits unsigned)

void
main(void)
{
 

//set up smclk for 4 kHz

UCSCTL1 |= DCORSEL_1;
UCSCTL2 |= 2;                     // 4 kHz sorta
UCSCTL3 |= SELREF__XT1CLK + FLLREFDIV_16;     //  32768/16 and 
UCSCTL4 |= SELS_0;    //SMCLK SOURCED FROM XT1CLK

//setup accelerometer power



//select SCL and SDA bits

P3SEL |= BIT1 + BIT2;   //select SDA and SEL (both either used as inputs or outputs) set up as input
P3REN |= BIT1 + BIT2;     //set up pullup resistors for connecting to power supply (positive)
P3OUT |= BIT1 + BIT2; 

//Initializing the eUSCI_B module (pg. 1081 user manual)

UCB0CTL1 |= UCSWRST;    //ENABLE RESET

UCB0CTL0 |= UCMST + UCMODE_3 + UCSYNC;//set to master in I2C mode and synchronous, both adresses are 7 bits

UCB0CTL1 |= UCSSEL_3 + UCTR_0;  //smclk, reciever, acknowledge normally

//don't need UCB0BR0 or  since SMCLK is at intended value

UCB0I2CSA = 0x1D;      // device address is 0x1C or 0x1D depending SA0 value

UCB0CTL1 |= UCTXSTT; //start condition generated

UCB1CTL1 &= ~UCSWRST;    // disable reset

UCB0IE = UCRXIE;  //enable interrupt for reciever and I2C

}

void USCI_I2C_ISR(void)__interrupt[USCI_I2C_VECTOR] { //can I put vector or does it have to be UCRXIFG?

switch(UCB0IV) {

case 0: break;
case 2:
  while (!(UCB0IFG&UCTXIFG));

  z = UCB0RXBUF;

  UCB0TXBUF = x;
  break;

case 4: break;
default: break;
}
}


