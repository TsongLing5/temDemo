
#include <msp430.h>
#include "iic.h"
//#define unsigned char uint8_t;
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
      UCB0I2CSA = 0x40;                         // Slave Address  //HDC2080 according to PIN ADDR ADDR=GND?1000000:1000001 //default(not connect) 80
      UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
//      UCB0IE |= UCTXIE;                         // Enable TX interrupt

            // Load TX byte counter

//        UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition



}

uint8_t IIC_WriteData(uint8_t regAddress,uint8_t Data){
//    while( UCB0CTL1& UCTXSTP );
//
//     UCB0CTL1 |= UCTR;                // 写模式
//
//     UCB0CTL1 |= UCTXSTT;             // 发送启动位
//
//
//
//     UCB0TXBUF = regAddress;           // 发送字节地址
//
//     // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可
//
//     while(!(UCB0IFG& UCTXIFG))
//
//     {
//
//       if( UCB0IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1
//       {
//         return 1;
//       }
//     }
//
//     UCB0TXBUF = Data;          // 发送字节内容
//
//     while(!(UCB0IFG& UCTXIFG));     // 等待UCTXIFG=1
//
//     UCB0CTL1 |= UCTXSTP;
//
//     while(UCB0CTL1& UCTXSTP);       // 等待发送完成
//
//     return 0;

    while( UCB0CTL1& UCTXSTP );
      UCB0CTL1 |= UCTR;                // 写模式
      UCB0CTL1 |= UCTXSTT;             // 发送启动位

      UCB0TXBUF = regAddress;           // 发送字节地址
      // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可
      while(!(UCB0IFG& UCTXIFG))
      {
        if( UCB0IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1
        {
          return 1;
        }
      }

      UCB0TXBUF = Data;          // 发送字节内容
      while(!(UCB0IFG& UCTXIFG));     // 等待UCTXIFG=1

      UCB0CTL1 |= UCTXSTP;
      while(UCB0CTL1& UCTXSTP);       // 等待发送完成

      return 0;


}

uint8_t readData(uint8_t regAddress,uint8_t *data){
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

    while(UCB0CTL1 & UCTXSTT);
    //Read byte
    UCB0CTL1 |= UCTXSTP;
    while(!(UCB0IFG& UCRXIFG));     // 读取字节内容
    *data=UCB0RXBUF;
//
//    UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
//    UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
//        //
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    return 0;

}

uint8_t readDataNByte( uint8_t word_addr,  uint8_t *pword_buf,  uint8_t len )
{
  uint8_t i;
  while( UCB0CTL1& UCTXSTP );
  UCB0CTL1 |= UCTR;                // 写模式
  UCB0CTL1 |= UCTXSTT;             // 发送启动位和写控制字节

  UCB0TXBUF = word_addr;           // 发送字节地址
  // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可
  while(!(UCB0IFG& UCTXIFG))
  {
    if( UCB0IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1
    {
      return 1;
    }
  }

  UCB0CTL1 &= ~UCTR;               // 读模式
  UCB0CTL1 |= UCTXSTT;             // 发送启动位和读控制字节

  while(UCB0CTL1& UCTXSTT);       // 等待UCTXSTT=0
  // 若无应答 UCNACKIFG = 1

  for( i= 0; i< len -1 ; i++)
  {
    while(!(UCB0IFG& UCRXIFG));   // 读取字节内容，不包括最后一个字节内容
    *pword_buf++= UCB0RXBUF;
  }

  UCB0CTL1 |= UCTXSTP;             // 在接收最后一个字节之前发送停止位

  while(!(UCB0IFG& UCRXIFG));     // 读取最后一个字节内容
  *pword_buf = UCB0RXBUF;

  while( UCB0CTL1& UCTXSTP );

  return 0;
}
