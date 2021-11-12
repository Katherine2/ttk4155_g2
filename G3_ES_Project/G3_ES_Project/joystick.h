/*
 * joystick.h
 *
 * Created: 22.09.2021 15:45:33
 *  Author: kathersi
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

int calibrate_joystick_center(uint8_t channel, int samples);
int normalize_output_joystick(uint8_t value, int center);
void send_position_horizontal(int position);
void send_position_vertical(int position);
void send_button_status(int status);
int get_button_status(void);
void send_calibration(int position, int id);
void send_joystick_status(int h_pos, int v_pos, int b_stat, int h_center, int v_center);


#endif /* JOYSTICK_H_ */