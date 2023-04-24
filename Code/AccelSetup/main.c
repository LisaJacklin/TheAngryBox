#include <msp430.h>

//variable to store value from z-component of accelerometer
char z; // Z value(8-bits unsigned)

void
main(void)
{
UCSCTL1 |= DCORSEL_2;
UCSCTL2 |= 61;                 	// 2 MHz sorta
UCSCTL3 |= SELREF__XT1CLK; 	//  
UCSCTL4 |= SELM__DCOCLK + SELS__DCOCLKDIV;	//SMCLK SOURCED FROM XT1CLK

//current: depends on ODR (50 Hz) --> 24 uA = Idd

//output data rates (ODR): register 0x2A where DR[0:2]=100 (50 Hz or 20 ms) or bits 3 through 5

//select SCL and SDA bits

//new forma for SDA AND SCL bits
P3SEL = (BIT1 + BIT2);
P3DIR |=BIT1; //TRANSMIT OR OUTPUT SDA PIN
P3DIR &=~ BIT2; //RECIEVE OR INPUT SCL PIN

/////////// LED!!!!
P1DIR |= BIT1;	// set up LED
P1OUT &= ~BIT1;

//Initializing the eUSCI_B module (pg. 1081 user manual)

UCB0CTL1 |= UCSWRST;	//ENABLE RESET

UCB0CTL0 |= UCMST + UCMODE_3 + UCSYNC;//set to master in I2C mode and synchronous, both adresses are 7 bits

UCB0CTL1 |= UCSSEL_3;  //smclk, reciever, acknowledge normally

UCB0BR0 |= 1000; //baud rate = 20 kHz from 2 MHz
UCB0BR1 |= 0;

UCB0CTL1 &= ~UCSWRST;	// disable reset

//UCB0IE = UCRXIE + UCTXIE;  //enable interrupt for reciever and I2C

//set up accelerometer registers

  // sysmod wake mode

 ///////// Active Mode:

UCB0CTL1 |= UCTR;//start condition generated and I2C transmit
UCB0I2CSA = 0x1D;  	// slave address and SA0=1
UCB0CTL1 |= UCTXSTT;
UCB0TXBUF = 0x2A; //address of CTRL_REG1 Register

while(UCB0CTL1 & UCTXSTT);  //wait for start bit cleared
while(!(UCB0IFG & UCTXIFG));  //wait for register address to be sent
 

//still in transmit mode
// don't need to restart

UCB0TXBUF = 0x01; //active mode of CTRL_REG1 Register

while(!(UCB0IFG & UCTXIFG));  //wait to write value

UCB0CTL1 |= UCTXSTP;  //generate stop

while(UCB0CTL1 & UCTXSTT);  //wait for stop to be sent

//////////  Message:

//transmit adresses//

while(1) {

while((UCB0STAT & UCBBUSY));	// wait for bus to stop being busy

//SINCE SA0 ON THE BOARD IS SET TO 1, THE ADDRESS IS 3B FOR THE SLAVE
UCB0CTL1 |= UCTR;//start condition generated and I2C transmit
UCB0I2CSA = 0x1D;  	// write and SA0=1
UCB0CTL1 |= UCTXSTT;
UCB0TXBUF = 0x05; //address of the Z_LSB Register

while(UCB0CTL1 & UCTXSTT);  //wait for start bit cleared
while(!(UCB0IFG & UCTXIFG));  //wait for register address to be sent
//Recieve z-axis force//

//while((UCB0STAT & UCBBUSY));	// wait for bus to stop being busy

UCB0CTL1 &= ~UCTR; //recieve mode
UCB0CTL1 |= UCTXSTT;  //generate start again

while(UCB0CTL1 & UCTXSTT);  //wait for start bit cleared  //also when RXBUF is set

//while(!(UCB0IFG & UCRXIFG));  //wait to recieve value

z = UCB0RXBUF;  //read z-axis byte

UCB0CTL1 |= UCTXSTP;  //generate stop

while(UCB0CTL1 & UCTXSTT);  //wait for stop to be sent

if ((z < 40) & (z > 0)) {

P1OUT |= BIT1; 	//LED

}
else {
P1OUT &= ~BIT1;
}
}



}







