/*
 * BME280.c
 *
 *  Created on: 2021年1月23日
 *      Author: aria
 */
#include "BME280.h"


uint8_t ReadTHid(void)  // Check BME280 sensor chip ID: 0x60
{
    uint8_t FLAG=0;

    if(1){
        FLAG=1;
    }else{
        FLAG=0;
    }
    return FLAG;
}
