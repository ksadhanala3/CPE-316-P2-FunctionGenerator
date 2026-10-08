/*
 * dac.h
 *
 *  Created on: Oct 29, 2024
 *      Author: ksadh
 */

#ifndef SRC_DAC_H_
#define SRC_DAC_H_

#define DAC_CTRL 0x3000
#define V_REF 3300
#define STEPS 4095

void dac_init(void);
void dac_write(uint16_t data);
uint16_t dac_volt_conv(uint16_t value);



#endif /* SRC_DAC_H_ */
