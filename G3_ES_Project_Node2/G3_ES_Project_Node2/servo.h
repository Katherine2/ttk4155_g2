/*
 * servo.h
 *
 * Created: 27.10.2021 15:04:34
 *  Author: zahrajm
 */ 


#ifndef SERVO_H_
#define SERVO_H_

void pwm_init(void);
void move_servo(void);
void move_to(char pos);


#endif /* SERVO_H_ */