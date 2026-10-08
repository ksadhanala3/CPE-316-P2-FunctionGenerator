/*
 * key.h
 *
 *  Created on: Oct 25, 2024
 *      Author: ksadh
 */

#include "main.h"
#ifndef SRC_KEY_H_
#define SRC_KEY_H_

#define NO_PRESS -1
#define STAR 10
#define HASH 15

void key_init(void);
int32_t positionToValue(uint8_t  column, uint8_t  row);
int32_t pollValue(void);

#endif /* SRC_KEY_H_ */
