#include <msp430.h>

//variable to store value from z-component of accelerometer
char z; // Z value(8-bits unsigned)

void
main(void)
{
UCSCTL1 |= DCORSEL_2;
UCSCTL2 |= 61;                     // 2 MHz sorta
UCSCTL3 |= SELREF__XT1CLK;     //  
UCSCTL4 |= SELM__DCOCLK + SELS__DCOCLKDIV;    //SMCLK SOURCED FROM XT1CLK

//current: depends on ODR (50 Hz) --> 24 uA = Idd

//output data rates (ODR): register 0x2A where DR[0:2]=100 (50 Hz or 20 ms) or bits 3 through 5

//select SCL and SDA bits

//new forma for SDA AND SCL bits
P3SEL = (BIT1 + BIT2);
P3DIR |=BIT1; //TRANSMIT OR OUTPUT SDA PIN
P3DIR &=~ BIT2; //RECIEVE OR INPUT SCL PIN

//P3SEL |= BIT1 + BIT2;     //select SDA and SCL (both either used as inputs or outputs) set up as input
//P3REN |= BIT1 + BIT2;     //set up pullup resistors for connecting to power supply (positive)

//Initializing the eUSCI_B module (pg. 1081 user manual)

UCB0CTL1 |= UCSWRST;    //ENABLE RESET

UCB0CTL0 |= UCMST + UCMODE_3 + UCSYNC;//set to master in I2C mode and synchronous, both adresses are 7 bits

UCB0CTL1 |= UCSSEL_3;  //smclk, reciever, acknowledge normally

UCB0BR0 |= 40; //baud rate = 50 kHz from 2 MHz
UCB0BR1 |= 0;

UCB0CTL1 &= ~UCSWRST;    // disable reset

//UCB0IE = UCRXIE + UCTXIE;  //enable interrupt for reciever and I2C

//set up accelerometer registers

  // sysmod wake mode

//////////  Message:

//transmit adresses//

//SINCE SA0 ON THE BOARD IS SET TO 1, THE ADDRESS IS 3B FOR THE SLAVE
UCB0CTL1 |= UCTR;//start condition generated and I2C transmit 
UCB0I2CSA = 0x1D;      // write and SA0=1
UCB0CTL1 |= UCTXSTT;
UCB0TXBUF = 0x06; //address of the Z_LSB Register

while(!(UCB0IFG & UCTXIFG));  //wait for register address to be sent
while(UCB0CTL1 & UCTXSTT);  //wait for start bit cleared

//Recieve z-axis force//

UCB0CTL1 &= ~UCTR; //recieve mode
UCB0CTL1 |= UCTXSTT;  //generate start again

//UCB0I2CSA = 0x1D;      // write and SA0=1
while(UCB0CTL1 & UCTXSTT);  //wait for start bit cleared

z = UCB0RXBUF;  //read z-axis byte

while(!(UCB0IFG & UCRXIFG));  //wait to recieve value

UCB0CTL1 |= UCTXSTP;  //generate stop

while(UCB0CTL1 & UCTXSTT);  //wait for stop to be sent

while(1) {
  __delay_cycles(1);
}

}

//void USCI_I2C_ISR(void)__interrupt[USCI_I2C_VECTOR] { //can I put vector or does it have to be UCRXIFG?


//switch(UCB0IV) {      // find cases corresponding to the IFs pg 1023

//case 0x16:  //UCRXIFG0 or data recieved




//z = UCB0TXBUF;
  
 // break;

//case 0x18: // UCTXIFG0 or transmit buffer is empty

 

//break;
//default: break;
//}
//}

