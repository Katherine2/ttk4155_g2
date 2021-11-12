/*
 * joystick.h
 *
 * Created: 27.10.2021 14:42:04
 *  Author: zahrajm
 */ 

#include "can_controller.h"

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

CAN_MESSAGE get_positions();
void button_pressed(char d);
int normalize_position(char d, int center);
int calibrate_center(char d);

#endif /* JOYSTICK_H_ */