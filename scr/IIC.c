
#include <msp430.h>

void initialIIC(void){

    unsigned int i;
    P3SEL &= ~BIT1;
    P3DIR |= BIT1;
    P3OUT |= BIT1;
    for(i=0;i<9;i++){
        P3OUT |=BIT1;
        __delay_cycles(8000);
        P3OUT &= BIT2;
    }

      P3SEL |= 0x03;                            // Assign I2C pins to USCI_B0
      UCB0CTL1 |= UCSWRST;                      // Enable SW reset
      UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
      UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
      UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
      UCB0BR1 = 0;
      UCB0CTL0 &= ~UCSLA10;
      UCB0I2CSA = 0x80;                         // Slave Address  //HDC2080 according to PIN ADDR ADDR=GND?1000000:1000001 //default(not connect) 80
      UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
//      UCB0IE |= UCTXIE;                         // Enable TX interrupt

            // Load TX byte counter

//        UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition



}

unsigned int IIC_WriteData(unsigned int regAddress,unsigned int Data){
    // Start
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, write mode and start condition
    // address
    UCB0TXBUF = regAddress;
    // ACK
    while(!(UCB0IFG& UCTXIFG)){
      if( UCB0IFG& UCNACKIFG ){      // 若无应答 UCNACKIFG=1
        return 1;
      }
    }
    // Data
    UCB0TXBUF = Data;
    // Stop
    UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
    UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
    //
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    return 0;

}

unsigned int readData(unsigned char regAddress,unsigned int *data){
//    while (UCB0CTL1 & UCTXSTP);
    //Start IIC
    UCB0CTL1 |= UCTR ;   //write mode write register address
//    UCB0TXBUF = regAddress;
    UCB0CTL1 |= UCTXSTT;
    //Write address
    UCB0TXBUF = regAddress;
    //ACK
    while(!(UCB0IFG& UCTXIFG)){
      if( UCB0IFG& UCNACKIFG ){      // 若无应答 UCNACKIFG=1
        return 1;
      }
    }


    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;

    while(UCB0CTL1&UCTXSTT);
    //Read byte
    UCB0CTL1 |= UCTXSTP;
    while(!(UCB0IFG& UCRXIFG));     // 读取字节内容
    *data=UCB0RXBUF;

    UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
    UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
        //
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    return 0;

}
