/*
 * ir.h
 *
 * Created: 27.10.2021 15:13:35
 *  Author: zahrajm
 */ 


#ifndef IR_H_
#define IR_H_

void adc_init(void);
int adc_read(void);
int is_goal(int value, int score);



#endif /* IR_H_ */