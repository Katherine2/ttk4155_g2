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
void move_servo(void);
void move_to(char pos);
void set_duty_cycle(float dutyCycle);

#endif /* SERVO_H_ */