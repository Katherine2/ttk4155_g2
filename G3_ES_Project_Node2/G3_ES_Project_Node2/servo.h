/*
 * servo.h
 *
 * Created: 27.10.2021 15:04:34
 *  Author: zahrajm
 */ 

#include <stdio.h>

#ifndef SERVO_H_
#define SERVO_H_

void pwm_init(void);
//void move_servo(int joystick_position);
void move_servo(int pos);
void set_duty_cycle(int dty);

#endif /* SERVO_H_ */