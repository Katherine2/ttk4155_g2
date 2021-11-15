/*
 * motorbox.h
 *
 * Created: 03.11.2021 09:29:29
 *  Author: zahrajm
 */ 

#include "can_controller.h"
#include "pid.h"

#ifndef MOTORBOX_H_
#define MOTORBOX_H_

volatile struct PID_DATA pid_data;

void motorbox_init(void);
int16_t receive_data(void);
void dac_init(void);
void move_motor(int joystick_position, int center);
int calibrate_motor(int16_t position);

#endif /* MOTORBOX_H_ */