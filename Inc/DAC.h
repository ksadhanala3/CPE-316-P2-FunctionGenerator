/*
 * DAC.h
 *
 *  Created on: Oct 24, 2025
 *      Author: sean1
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include <stdint.h>

void DAC_init(void);
uint16_t DAC_volt_conv(uint16_t voltage);
void DAC_write(uint16_t to_write);


#endif /* INC_DAC_H_ */
