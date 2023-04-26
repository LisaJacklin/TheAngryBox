#include <msp430.h>
#include <hal_lcd.h>

extern int IncrementVcore(void);
extern int DecrementVcore(void);

//variable to store value from z-component of accelerometer
char z; // Z value(8-bits unsigned)
int a; int b; int c; int i; int angle; int n; int m; float z1;
       	//01234567890
char msg[]={"Angle:   	"};
char msg2[]={"Angle:  	"};
long loop = 0;

//unsigned char msg[] = "000\r\n";  //the z-axis value and new line

void
main(void)
{

P5DIR |= BIT0;

UCSCTL1 |= DCORSEL_4;
UCSCTL2 |= 122;                 	// 4 MHz sorta
UCSCTL3 |= SELREF__XT1CLK; 	//  
UCSCTL4 |= SELM__DCOCLK + SELS__DCOCLKDIV;	//SMCLK SOURCED FROM XT1CLK

//current: depends on ODR (50 Hz) --> 24 uA = Idd

IncrementVcore();
IncrementVcore();
IncrementVcore();
IncrementVcore();

//output data rates (ODR): register 0x2A where DR[0:2]=100 (50 Hz or 20 ms) or bits 3 through 5

//select SCL and SDA bits

//new forma for SDA AND SCL bits
P3SEL = (BIT1 + BIT2);
P3DIR |=BIT1; //TRANSMIT OR OUTPUT SDA PIN
P3DIR &=~ BIT2; //RECIEVE OR INPUT SCL PIN

/////////// LED!!!!
P1DIR |= BIT1;	// set up LED
P1OUT &= ~BIT1;

/////////set up timer A1

TA1CTL |= TASSEL__SMCLK + TACLR; // timer clear, SMCLK source, clock on for Timer A1,
//TA1CCTL0 |= CCIE; //capture compare interrupt enabled
TA1CCR0 |= 8000; // count to 2 ms but is acting like 40 ms?

//Initializing the eUSCI_B module (pg. 1081 user manual)

UCB0CTL1 |= UCSWRST;	//ENABLE RESET

UCB0CTL0 |= UCMST + UCMODE_3 + UCSYNC;//set to master in I2C mode and synchronous, both adresses are 7 bits

UCB0CTL1 |= UCSSEL_3;  //smclk, reciever, acknowledge normally

UCB0BR0 |= 2000; //baud rate = 20 kHz from 4 MHz
UCB0BR1 |= 0;

UCB0CTL1 &= ~UCSWRST;	// disable reset

 ///////// Active Mode:

while((UCB0STAT & UCBBUSY));	// wait for bus to stop being busy

UCB0CTL1 |= UCTR;//start condition generated and I2C transmit
UCB0I2CSA = 0x1D;  	// slave address and SA0=1
UCB0CTL1 |= UCTXSTT;
UCB0TXBUF = 0x2A; //address of CTRL_REG1 Register

while(UCB0CTL1 & UCTXSTT);  //wait for start bit cleared
while(!(UCB0IFG & UCTXIFG));  //wait for register address to be sent

UCB0TXBUF = 0x01; //active mode of CTRL_REG1 Register

while(!(UCB0IFG & UCTXIFG));  //wait to write value

UCB0CTL1 |= UCTXSTP;  //generate stop

while(UCB0CTL1 & UCTXSTT);  //wait for stop to be sent

//////////////LCD SETUP////////////////////////
halLcdInit();   	//LCD light settings
halLcdClearScreen();
halLcdBackLightInit();
halLcdSetContrast(90);
halLcdSetBackLight(5);
////////////////////BUTTON SETUP//////////////////////////////
 //BUTTON SETUP
    P2DIR &= ~BIT7;                           //sets to input direction
    P2IE |= BIT7;                             // interrupts are allowed for this button                  
    P2REN = BIT7;                             //input with pull up resistor 
    P2OUT = BIT7;                             //also required to enable pull up resistor.
TA1CTL |= MC__UP + TAIE;  //count up to 1 ms
_EINT();
LPM0;
}
//////////// BUTTON INTERRUPT /////////////////////////////
void Interrupt_Button(void) __interrupt [PORT2_VECTOR] {
  //button 2.7
    if (P2IV == P2IV_P2IFG7){ 
        halLcdPrintLine("STOP POKING ME", 3, OVERWRITE_TEXT);  
      P2IFG &= ~BIT7; // Clear interrupt flag for P2.7
    }       
}
void timerA(void)__interrupt[TIMER1_A1_VECTOR] {
    
  P5OUT |= BIT0; // check timer A reset
  TA1CTL &= ~TAIFG; //reset flag

  //////////  Message ////////////
  //transmit adresses//
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
  while(!(UCB0IFG & UCRXIFG));  //wait to recieve value
 
  z = UCB0RXBUF;  //read z-axis byte
  UCB0CTL1 |= UCTXSTP;  //generate stop

  while(UCB0CTL1 & UCTXSTT);  //wait for stop to be sent
  z1 = (float)z;
  m = (int)((1.385)*z1);
  n = (m)%5;
  angle = 90 - (m - n);

  msg[6] = '0' + z/100;
  msg[7] = '0' + (z%100)/10;
  msg[8] = '0' + z%10;
  halLcdPrintLine(msg, 5, OVERWRITE_TEXT);

  msg2[8]= '0' + angle/100;
  msg2[9] = '0' + (angle%100)/10;
  msg2[10] = '0' + angle%10;
  halLcdPrintLine(msg2, 1, OVERWRITE_TEXT);

  if (z > 120) {
    halLcdPrintLine("STOP SHAKING ME", 3, OVERWRITE_TEXT);  
  } else if ((z < 50) && (z > 0)) {
    halLcdPrintLine("PUT ME DOWN", 3, OVERWRITE_TEXT);  
  }

  else {
  halLcdPrintLine("                 	", 3, OVERWRITE_TEXT);
  }

P5OUT &= ~BIT0;	//checking how long ISR takes
}

