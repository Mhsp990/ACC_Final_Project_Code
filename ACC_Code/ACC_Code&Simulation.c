#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Validation.h"

float Ego_ace = 0;
float Lead_ace = 0;
float Relative_velo = 0;

float Lead_pos = 30.0;  // Initial lead car position (m)
float Ego_pos  = 10.0;  // Initial ego car position  (m)

float Relative_distance_pres = 20; //Relative distance between Ego and Lead Car.
float Relative_distance_past = 20; //Initial past value of relative distance.

float Lead_velo = 70.0/3.6; // Initial lead car position (m/s)
float Ego_velo  = 70.0/3.6; // Initial ego car position  (m/s)

float interval = 0.001;
int counter = 1; //Interactions limiter

//Calibration Variables
const float D_default        = 10;
const float Default_Time_Gap = 3;
float Time_Gap = 3;
const float Kverr_gain       = 0.5;
const float Kxerr_gain       = 0.02;
const float Kvx_gain         = 0.04;

float Acceleration       = 0;
float Safe_distance      = 15;
float Control_v          = 0;
float Control_x          = 0;
float SafeD_relD		     = 0;
const float Ego_acceleration_min     = -5;
const float Ego_acceleration_max     = 1.47;

//Variables received from CAN FRAMES
bool ACC_input     = 1; //Trigger for ACC_input. 
bool Rain_sensor   = 0;
bool Gas_pedal     = 0;
bool Brake_pedal   = 0;
bool Fault_signal  = 0;

float ACC_speed_set  = 100;

bool ACC_enabled             = false;
bool aux  		               = 0;
float ACC_acceleration       = 0;
float ACC_brake_acceleration = 0;

//--------------------------------Car Simulation--------------------------------/
int main ()
{
  char *filename = "simulation.txt";  
  FILE *fp = fopen(filename, "w");   // open the file for writing 
   if (fp == NULL)
    {
        printf("Error opening the file %s",filename);
        return -1;
    }

  //Number of times of looping
  while (counter < 100000)
  {
  
  Ego_ace = Acceleration;
  //Storage of the last value of Relative Distance
  Relative_distance_past = Relative_distance_pres;

  //Simulation of vehicles behavior
  Ego_velo = Ego_velo + interval*Ego_ace;
  Lead_velo = Lead_velo + interval*Lead_ace;

  //Calculation of Ego Car position
  Ego_pos += (Ego_velo*interval);

  //Calculation of Lead Car position
  Lead_pos += (Lead_velo*interval);

  //Relative distance between Ego and Lead Car.
  Relative_distance_pres = Lead_pos - Ego_pos;
  //Value of relative distance.
  Relative_velo = Lead_velo - Ego_velo;

  // Sensor range
  if(Relative_distance_pres > 200){
      Relative_distance_pres = 200;
  }else{
      Relative_distance_pres = Relative_distance_pres;
  }

  //ACC enable logic block
	if(aux == 0 && ACC_input == 1 && Fault_signal == 0 && Ego_velo >= 11 && Gas_pedal == 0 && Brake_pedal == 0){
		aux = 1;
		ACC_enabled = 1;
	}else {
		if (ACC_input == 1 && aux == 1 && Fault_signal == 0 && Gas_pedal == 0 && Brake_pedal == 0){
			ACC_enabled = 1;
		}else {
			ACC_enabled = 0;
			aux = 0;
		}
	}

  //----------------------Control block---------------------//

  //Detection of rain and change the Time_Gap
  Time_Gap = Rain_sensor ? Default_Time_Gap*2 : Default_Time_Gap;
  //Limit of Ego_velo to dont exceed the safe velocity
  if(ACC_speed_set > 120) ACC_speed_set = 120;

	Safe_distance = (Ego_velo * Time_Gap) + D_default;
	SafeD_relD = Safe_distance - Relative_distance_pres;
	Control_x = (Relative_velo * Kvx_gain) - ((Safe_distance - Relative_distance_pres) * Kxerr_gain);
	Control_v = (ACC_speed_set - Ego_velo) * Kverr_gain;

	if (SafeD_relD > 0){
			
		Acceleration = (Relative_velo * Kvx_gain) - (SafeD_relD * Kxerr_gain);
			
	}else{
		Acceleration = (Control_x < Control_v) ? Control_x : Control_v;
	}
	
	Acceleration = (Acceleration < Ego_acceleration_min) ? Ego_acceleration_min : (Acceleration > Ego_acceleration_max) ? Ego_acceleration_max : Acceleration;

  counter++;

 
 // Data validation 
  bool checkOne = checkCollision(ACC_enabled, Relative_distance_pres);
  bool checkTwo = checkValidationSensors(ACC_enabled,  Fault_signal,  Gas_pedal,  Brake_pedal);
  bool checkThree = checkRainSafeDistance( ACC_enabled,  Rain_sensor,  D_default,  Ego_velo,  Safe_distance);
  if(checkOne || checkTwo || checkThree) counter=100000;


  // write to the text file
  fprintf(fp,"\nAcceleration: %.2f, ",Acceleration);
  fprintf(fp,"RelativeDistance: %.2f, ", Relative_distance_pres);
  fprintf(fp,"SafeDistance: %.2f, ", Safe_distance);
  fprintf(fp,"EgoVelocity: %.2f, ", Ego_velo);
  fprintf(fp,"LeadVelocity: %.2f, ", Lead_velo);
  }

  fclose(fp);   // close the file
}

