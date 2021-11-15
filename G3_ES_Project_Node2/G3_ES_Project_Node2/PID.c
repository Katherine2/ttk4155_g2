/*
 * PID.c
 *
 * Created: 15.11.2021 09:20:24
 *  Author: zahrajm
 */ 

#include "pid.h"
//#include "stdint.h"


#define K_P     1
#define K_I     0
#define K_D     0

//! Parameters for regulator
struct PID_DATA pid;

#define TIME_INTERVAL   157

/*! \brief Initialisation of PID controller parameters.
 *
 *  Initialise the variables used by the PID algorithm.
 *
 *  \param p_factor  Proportional term.
 *  \param i_factor  Integral term.
 *  \param d_factor  Derivate term.
 *  \param pid  Struct with PID status.
 */
void pid_Init(void/*struct PID_DATA *pid*/)
// Set up PID controller parameters
{
  // Start values for PID controller
	pid.id = 1;
  pid.sumError = 0;
  pid.lastProcessValue = 0;
  // Tuning constants for PID loop
  pid.P_Factor = K_P;
  pid.I_Factor = K_I;
  pid.D_Factor = K_D;
  // Limits to avoid overflow
  pid.maxError = MAX_INT / (pid.P_Factor + 1);
  pid.maxSumError = MAX_I_TERM / (pid.I_Factor + 1);
}


/*! \brief PID control algorithm.
 *
 *  Calculates output from setpoint, process value and PID status.
 *
 *  \param setPoint  Desired value.
 *  \param processValue  Measured value.
 *  \param pid  PID status struct.
 */
int16_t pid_Controller(int16_t setPoint, int16_t processValue/*, struct PID_DATA *pid*/)
{
  //printf("id: %d\n\r", pid.id);
  int16_t error, p_term, d_term;
  int32_t i_term, ret, temp;
  
  if(processValue <= setPoint){
  error = setPoint - processValue;
  }

  if(processValue > setPoint){
  error = processValue - setPoint;
}


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
	//printf("p:%d, i:%d, d:%d\n\r", p_term, i_term, d_term);
  ret = (p_term + i_term + d_term); // SCALING_FACTOR;
  
if(ret > MAX_INT){
    ret = MAX_INT;
  }
  else if(ret < -MAX_INT){
    ret = -MAX_INT;
  }

//printf("ret:%d\n\r",ret);
  return((int16_t)ret);
}
