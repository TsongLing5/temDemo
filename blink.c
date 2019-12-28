//#include <msp430.h>
//
//
///**
// * blink.c
// */
//void main(void)
//{
//	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
//	P1DIR |= 0x01;					// configure P1.0 as output
//
//	volatile unsigned int i;		// volatile to prevent optimization
//
//	while(1)
//	{
//		P1OUT ^= 0x01;				// toggle P1.0
//		for(i=10000; i>0; i--);     // delay
//	}
//}

#include <msp430.h>
//#include"SSD1306.h"
//#include "IIC.H"
unsigned char MST_Data,SLV_Data;
unsigned int temp;
unsigned char tmp;
unsigned char data[4];
unsigned char BME280Data[9];
unsigned int tmpp,hum;
int tmpFloat;
unsigned char tmpDB[5],humDB[5];
unsigned char BME280ID;

#define Temp_LOW 0x00
#define Temp_HIGH 0x01
#define HUMI_LOW 0x02
#define HUMI_HIGH 0x03

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  P1DIR |= BIT0;
  P1OUT |= 0x02;                            // Set P1.0 for LED
                                            // Set P1.1 for slave reset
  P1DIR |= 0x03;                            // Set P1.0-2 to output direction
  P3SEL |= BIT3+BIT4;                       // P3.3,4 option select
  P2SEL |= BIT7;                            // P2.7 option select

  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 0x02;                           // /2
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  P1OUT &= ~0x02;                           // Now with SPI signals initialized,
  P1OUT |= 0x02;                            // reset slave

  for(i=50;i>0;i--);                        // Wait for slave to initialize

  MST_Data = 0x01;                          // Initialize data values
  SLV_Data = 0x00;


  LCD_Init();//
  initialIIC();
  LCD_P6x8Str(30,0,"2019-10-24");
  LCD_P6x8Str(30,1,"Temp:");
  LCD_P6x8Str(60,1,"??.??C");
  LCD_P6x8Str(30,2,"Humi:");
  LCD_P6x8Str(60,2,"??.??%");
  LCD_P6x8Str(30,6,"Hello World");
  P1OUT |=BIT0;
//
//  IIC_WriteData(0x0E,0x00);
//  IIC_WriteData(0x0F,0x00);
//  delay(100);delay(100);delay(100);delay(100);
//  IIC_WriteData(0x0F,0x01);
//  delay(100);delay(100);delay(100);delay(100);
////  readData(0x01,&tmp);
////  temp=tmp;
////  readData(Temp_LOW,&tmp);
//  readDataNByte(0x00,data,4);
//  temp=(temp<<8)|tmp;
//  tmpp=data[0]|(data[1]<<8);
//  tmpp=(tmpp/4)-4000;
//  hum=data[2]|(data[3]<<8);
//  tmpFloat=(int)(hum*1.0/6.56);
//
//  tmpDB[0]=tmpp/1000+'0';
//  tmpDB[1]=(tmpp/100)%10+'0';
//  tmpDB[2]='.';
//  tmpDB[3]=tmpp%100/10+'0';
//  tmpDB[4]=tmpp%10+'0';
//
//  humDB[0]=(tmpFloat/1000)+'0';
//  humDB[1]=(tmpFloat/100)%10+'0';
//  humDB[2]='.';
//  humDB[3]=tmpFloat%100/10+'0';
//  humDB[4]=tmpFloat%10+'0';
//  LCD_P6x8Str(60,1,tmpDB);
//  LCD_P6x8Str(60,2,humDB);
  while (1){               // USCI_A0 TX buffer ready?
//      FuncHDC2080();
      FuncBME280();
  }
//  UCA0TXBUF = MST_Data;                     // Transmit first character

//  __bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
}
//
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=USCI_A0_VECTOR
//__interrupt void USCI_A0_ISR(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//  volatile unsigned int i;
//
//  switch(__even_in_range(UCA0IV,4))
//  {
//    case 0: break;                          // Vector 0 - no interrupt
//    case 2:                                 // Vector 2 - RXIFG
//      while (!(UCA0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
//
//      if (UCA0RXBUF==SLV_Data)              // Test for correct character RX'd
//        P1OUT |= 0x01;                      // If correct, light LED
//      else
//        P1OUT &= ~0x01;                     // If incorrect, clear LED
//
//      MST_Data++;                           // Increment data
//      SLV_Data++;
////      UCA0TXBUF = MST_Data;                 // Send next value
//
//      for(i = 20; i>0; i--);                // Add time between transmissions to
//                                            // make sure slave can process information
//      break;
//    case 4: break;                          // Vector 4 - TXIFG
//    default: break;
//  }
//}


void FuncHDC2080(){
    setAddr(0x40);
    IIC_WriteData(0x0F,0x01);
            delay(100);delay(100);delay(100);delay(100);
            delay(100);delay(100);delay(100);delay(100);
            delay(100);delay(100);delay(100);delay(100);
            delay(100);delay(100);delay(100);delay(100);

          //  readData(0x01,&tmp);
          //  temp=tmp;
          //  readData(Temp_LOW,&tmp);
            readDataNByte(0x00,data,4);
            temp=(temp<<8)|tmp;
            tmpp=data[0]|(data[1]<<8);
            tmpp=(tmpp/4)-4000;
            hum=data[2]|(data[3]<<8);
            tmpFloat=(int)(hum*1.0/6.56);

            tmpDB[0]=tmpp/1000+'0';
            tmpDB[1]=(tmpp/100)%10+'0';
            tmpDB[2]='.';
            tmpDB[3]=tmpp%100/10+'0';
            tmpDB[4]=tmpp%10+'0';

            humDB[0]=(tmpFloat/1000)+'0';
            humDB[1]=(tmpFloat/100)%10+'0';
            humDB[2]='.';
            humDB[3]=tmpFloat%100/10+'0';
            humDB[4]=tmpFloat%10+'0';
            LCD_P6x8Str(60,1,tmpDB);
            LCD_P6x8Str(60,2,humDB);
}

void FuncBME280(){
    setAddr(0x76);  //0x77
    delay(100);
    IIC_WriteData(0xE0,0xB6 );
    delay(1000);
    IIC_WriteData(0xF2,0x01 );
    delay(100);
    IIC_WriteData(0xF4,0x57);
    delay(100);
    IIC_WriteData(0xF5,0x02);
    delay(100);
    readData(0xD0,&tmp);

    readData(0xF3,&tmp);
    while(tmp&0x04){
        readData(0xF3,&tmp);
    }
    delay(100);
    readDataNByte(0xF7,BME280Data,8);
    readData(0xF3,&tmp);
}
