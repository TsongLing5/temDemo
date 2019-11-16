/*
 * IIC.H
 *
 *  Created on: 2019年10月28日
 *      Author: aria
 */

#ifndef _IIC_H_
#define _IIC_H_

void initialIIC(void);
unsigned int readData(unsigned char regAddress,unsigned int *data);
unsigned int IIC_WriteData(unsigned int regAddress,unsigned int Data);



#endif /* SCR_IIC_H_ */
