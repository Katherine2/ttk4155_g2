/*
 * PID.h
 *
 * Created: 15.11.2021 09:20:45
 *  Author: zahrajm
 */ 

#ifndef PID_H
#define PID_H

#include "stdint.h"

#define SCALING_FACTOR  128

/*! \brief PID Status
 *
 * Setpoints and data used by the PID control algorithm
 */
typedef struct PID_DATA{
  int id;
  //! Last process value, used to find derivative of process value.
  int16_t lastProcessValue;
  //! Summation of errors, used for integrate calculations
  int32_t sumError;
  //! The Proportional tuning constant, multiplied with SCALING_FACTOR
  int16_t P_Factor;
  //! The Integral tuning constant, multiplied with SCALING_FACTOR
  int16_t I_Factor;
  //! The Derivative tuning constant, multiplied with SCALING_FACTOR
  int16_t D_Factor;
  //! Maximum allowed error, avoid overflow
  int16_t maxError;
  //! Maximum allowed sumerror, avoid overflow
  int32_t maxSumError;
} pidData_t;

/*! \brief Maximum values
 *
 * Needed to avoid sign/overflow problems
 */
// Maximum value of variables
#define MAX_INT         32767
#define MAX_LONG        2147483647
#define MAX_I_TERM      (MAX_LONG / 2)

// Boolean values
#define FALSE           0
#define TRUE            1

void pid_Init(void);
int16_t pid_Controller(int16_t setPoint, int16_t processValue);
#endif
