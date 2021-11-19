/*
 * game.c
 *
 * Created: 12.11.2021 14:03:43
 *  Author: kathersi
 */ 

#include "game.h"
#include "ir.h"
#include "joystick.h"
#include "can_controller.h"
#include "sam.h"

#define GAME_OVER 50

int playing = 1, game_end= 0;
CAN_MESSAGE msg;

void start_game(){
	int goal = 0, adc;
	while(1) {
		if(playing && !game_end){
			adc = adc_read();
			goal = is_goal(adc, goal);
			msg = get_positions();
			delay_us(100000);
		}
		else{
			if(new_message_received()){
				msg = get_message();
				if(!msg.data[2]){
					playing = 1;
					PIOA -> PIO_PER = PIO_PA19;		//enables input/output function
					PIOA -> PIO_OER = PIO_PA19;		//sets pin PA19 (pin 42) as output
					PIOA -> PIO_PUDR = PIO_PA19;	//disables pull-ups
					PIOA -> PIO_CODR = PIO_PA19;	//clear output data register
				}
			}
		}
	}
}

void pause_game(int score){
	playing = 0;
	if(score >= GAME_OVER){
		game_over();
	}
}

void game_over(){
	playing = 0;
	game_end = 1;
	printf("game over\n\r");
}