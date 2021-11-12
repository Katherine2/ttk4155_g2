/*
 * motorbox.h
 *
 * Created: 03.11.2021 09:29:29
 *  Author: zahrajm
 */ 

#include "can_controller.h"

#ifndef MOTORBOX_H_
#define MOTORBOX_H_

void motorbox_init(void);
uint16_t receive_data(void);
void dac_init(void);
void move_motor(int joystick_position);


#endif /* MOTORBOX_H_ */