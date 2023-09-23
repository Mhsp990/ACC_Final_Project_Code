// ACC_code functions
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct ACCenable {
  bool ACC_enabled;
  bool aux;
};

struct ACCcontrol {
  float Acceleration;
  float Safe_distance;
};

//! ACC enable logic block
/*! This function has the role of checking whether or not it is possible to activate the ACC */

struct ACCenable logicBlockAccEnable(bool aux/**<  Auxiliary variable */, 
                                     bool ACC_input/**<  ACC input is the variable that defines whether or not the user-defined ACC works */,
                                     bool Fault_signal/**< Fault signal is the variable that checks whether the sensors are working correctly */, 
                                     float Ego_velo/**< Ego velocity is the current speed of the vehicle*/,
                                     bool Gas_pedal/**< It is the variable that identifies whether the accelerator pedal is pressed*/, 
                                     bool Brake_pedal/**< It is the variable that identifies whether the brake pedal is pressed*/
                                     ) {
  struct ACCenable index;

  if (aux == 0 && ACC_input == 1 && Fault_signal == 0 && Ego_velo >= 11 && Gas_pedal == 0 && Brake_pedal == 0) {
    index.aux = 1;
    index.ACC_enabled = 1;
    return index;

  } else {
    if (ACC_input == 1 && aux == 1 && Fault_signal == 0 && Gas_pedal == 0 && Brake_pedal == 0) {
      index.ACC_enabled = 1;

      return index;
    } else {
      index.ACC_enabled = 0;
      index.aux = 0;
      return index;
    }
  }
}

//! Time Gap
/*! This function focuses on calculating the time gap between the car and the lead car */
float timeGap(bool Rain_sensor/**<  Variable that receives whether there is rain or not */) { 
  float Time_Gap = 3;
  const float Default_Time_Gap = 3;
  Time_Gap = Rain_sensor ? Default_Time_Gap * 2 : Default_Time_Gap;
  return Time_Gap;
}
//! Speed Set
/*! This variable's function is to receive the desired speed value from the user and check whether it is possible to turn on the ACC by checking the speed range defined in the requirements.  */

float speedSet(float ACC_speed_set/**<  Setspeed defined by user */) { 
  float ACC_speed_set_max = 120.0;
  float ACC_speed_set_min = 40.0;
  ACC_speed_set = (ACC_speed_set < ACC_speed_set_min) ?
  ACC_speed_set_min : (ACC_speed_set > ACC_speed_set_max) ? 
  ACC_speed_set_max : ACC_speed_set;
  return ACC_speed_set/3.6;
}

struct ACCcontrol accelerationControl(bool ACC_enabled/**<  Variable that defines whether ACC is on or off */, 
                                      float Ego_velo/**<  Car speed */,
                                      float Time_Gap/**<  Time gap between ego car and lead car */, 
                                      float ACC_speed_set/**<  Setspeed defined by user */,
                                      float Relative_distance_past/**<  Relative speed between ego and lead car */, 
                                      float Relative_distance_pres/**<  Relative distance between ego car and lead car */, float interval) {
  struct ACCcontrol i;
  //! Acceleration Control
  /*!

  Function responsible for calculating and defining the acceleration to reach the setspeed or reduce the speed to meet the safety distance
  */
  float SafeD_relD                 = 0;   // Difference between safe and relative distance
  const float D_default            = 10;  // Standard distance
  float Control_x                  = 0;   // Intermediate variable to calculate relative distance 
  float Control_v                  = 0;   // Intermediate variable to calculate relative distance
  const float Kverr_gain           = 0.5; // Model gain
  const float Kxerr_gain           = 0.2;// Model gain
  const float Kvx_gain             = 0.04;// Model gain
  const float Ego_acceleration_min = -5;  // Minimum ego car acceleration
  const float Ego_acceleration_max = 1.47;// Max ego car acceleration
  float Relative_velo              = 0;   // Relative velocity of both cars

  if (ACC_enabled) {// Test if acc is ON/OFF
    Relative_velo = (Relative_distance_pres - Relative_distance_past)/interval;
    i.Safe_distance = (Ego_velo * Time_Gap) + D_default;
    SafeD_relD = i.Safe_distance - Relative_distance_pres;
    Control_x = (Relative_velo * Kvx_gain) - ((i.Safe_distance - Relative_distance_pres) * Kxerr_gain);
    Control_v = (ACC_speed_set - Ego_velo) * Kverr_gain;


  // Test whether it is possible to use ACC safely
    if (SafeD_relD > 0) {
      i.Acceleration = (Relative_velo * Kvx_gain) - (SafeD_relD * Kxerr_gain);

    } else {
      i.Acceleration = (Control_v < Control_x) ? Control_x : Control_v;
    }
    i.Acceleration = (i.Acceleration < Ego_acceleration_min) ?
    Ego_acceleration_min : (i.Acceleration > Ego_acceleration_max) ? 
    Ego_acceleration_max : i.Acceleration;
    return i;
    // If the safety conditions are not accepted, do not accelerate the car
  } else {
    i.Acceleration = 0;
    return i;
  }
}
