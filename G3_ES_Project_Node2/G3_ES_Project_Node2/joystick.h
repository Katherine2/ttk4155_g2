/*
 * joystick.h
 *
 * Created: 27.10.2021 14:42:04
 *  Author: zahrajm
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

CAN_MESSAGE get_positions();
void button_pressed(CAN_MESSAGE message);



#endif /* JOYSTICK_H_ */