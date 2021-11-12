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

#define GAME_OVER 5

int playing = 1, game_end= 0;
CAN_MESSAGE msg;

void start_game(){
	int goal = 0, adc;
	while(1) {
		if(playing && !game_end){
			adc = adc_read();
			printf("ir value: %d\n\r", adc);
			goal = is_goal(adc, goal);
			msg = get_positions();
			//uint16_t data = receive_data();
			//printf("data: %d\n\r", data);
			//delay_us(1000000000);
		}
		else{
			if(new_message_received()){
				msg = get_message();
				if(!msg.data[2]){
					playing = 1;
					//resume_game();
				}
			}
		}
	}
}

/*
void resume_game(){
	playing = 1;
}
*/
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