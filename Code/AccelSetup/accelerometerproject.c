#include <msp430.h>

//variable to store value from z-component of accelerometer
char z; // Z value(8-bits unsigned)

void
main(void)
{
 

//set up smclk for 4 kHz

UCSCTL1 |= DCORSEL_2;
UCSCTL2 |= 61;                     // 2 MHz sorta
UCSCTL3 |= SELREF__XT1CLK;     //  
UCSCTL4 |= SELM__DCOCLK + SELS__DCOCLKDIV;    //SMCLK SOURCED FROM XT1CLK

//FREQUENCY CHECKING
   P11DIR |= BIT1;                                 //SET TO BE ABLE TO CHECK THE FREQUENCY
   P11SEL |= BIT1;    

//current: depends on ODR (50 Hz) --> 24 uA = Idd

//output data rates (ODR): register 0x2A where DR[0:2]=100 (50 Hz or 20 ms) or bits 3 through 5

//select SCL and SDA bits

//new forma for SDA AND SCL bits
P3SEL = (BIT1 + BIT2);
P3DIR |=BIT1; //TRANSMIT OR OUTPUT SDA PIN
P3DIR &=~ BIT2; //RECIEVE OR INPUT SCL PIN

//P3SEL |= BIT1 + BIT2;     //select SDA and SCL (both either used as inputs or outputs) set up as input
//P3REN |= BIT1 + BIT2;     //set up pullup resistors for connecting to power supply (positive)

P11SEL |= BIT2 + BIT1;
P11DIR |= BIT2 + BIT1;

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
  UCB0I2CSA = 0x1D;      // write and SA0=1
  UCB0CTL1 |= UCTXSTT + UCTR; //start condition generated and I2C transmit 
  UCB0TXBUF = 0x06; //address of the Z_LSB Register

  while(!(UCB0IFG & UCTXIFG));  //wait for register address to be sent
  while(UCB0CTL1 & UCTXSTT);  //wait for start bit cleared

  //Recieve z-axis force//
  UCB0CTL1 |= UCTXSTT;  //generate start again
  UCB0CTL1 &= ~UCTR;  //recieve mode
  UCB0I2CSA = 0x1D;      // write and SA0=1
  while(UCB0CTL1 & UCTXSTT);  //wait for start bit cleared

  z = UCB0RXBUF;  //read z-axis byte

  while(!(UCB0IFG & UCRXIFG));  //wait to recieve value

  UCB0CTL1 |= UCTXSTP;  //generate stop

  while(UCB0CTL1 & UCTXSTT);  //wait for stop to be sent

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

