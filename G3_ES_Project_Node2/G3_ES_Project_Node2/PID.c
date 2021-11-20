/*
 * PID.c
 *
 * Created: 15.11.2021 09:20:24
 *  Author: zahrajm
 */ 

#include "pid.h"

#define K_P     1.5
#define K_I     0.25
#define K_D     0.1

//! Parameters for regulator
struct PID_DATA pid;

#define TIME_INTERVAL   157

void pid_Init(void)
// Set up PID controller parameters
{
	pid.id = 1;
	pid.sumError = 0;
	pid.lastProcessValue = 0;
	pid.P_Factor = K_P;
	pid.I_Factor = K_I;
	pid.D_Factor = K_D;
	// Limits to avoid overflow
	pid.maxError = MAX_INT / (pid.P_Factor + 1);
	pid.maxSumError = MAX_I_TERM / (pid.I_Factor + 1);
}

int16_t pid_Controller(int16_t setPoint, int16_t processValue)
{
	int16_t error, p_term, d_term;
	int32_t i_term, ret, temp;
	
	error = processValue - setPoint;
	
	// Calculate Pterm and limit error overflow
	if (error > pid.maxError){
		p_term = MAX_INT;
	}
	else if (error < -pid.maxError){
		p_term = -MAX_INT;
	}
	else{
		p_term = pid.P_Factor * error;
	}

	// Calculate Iterm and limit integral runaway
	temp = pid.sumError + (int32_t)error;
	if(temp > pid.maxSumError){
		i_term = MAX_I_TERM;
		pid.sumError = pid.maxSumError;
	}
	else if(temp < -pid.maxSumError){
		i_term = -MAX_I_TERM;
		pid.sumError = -pid.maxSumError;
	}
	else{
		pid.sumError = temp;
		i_term = pid.I_Factor * pid.sumError;
	}

	// Calculate Dterm
	d_term = pid.D_Factor * (pid.lastProcessValue - processValue);

	pid.lastProcessValue = processValue;
	ret = (p_term + i_term + d_term); // SCALING_FACTOR;
  
	if(ret > MAX_INT){
		ret = MAX_INT;
	}
	else if(ret < -MAX_INT){
		ret = -MAX_INT;
	}
	return((int16_t)ret);
}
