// Simulation Code calling the ACC_code functions
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ACC_Code.h"
#include <unistd.h>


float Ego_ace = 0;
float Lead_ace = 0;
float Relative_velo = 0;

float Lead_pos = 15.0;  // Initial lead car position (m)
float Ego_pos  = 10.0;  // Initial ego car position  (m)

float Relative_distance_pres = 5; //Relative distance between Ego and Lead Car.
float Relative_distance_past = 5; //Initial past value of relative distance.

float Lead_velo = 100.0/3.6; // Initial lead car position (m/s)
float Ego_velo  = 100.0/3.6; // Initial ego car position  (m/s)

float interval = 0.001; // Simulation's clock
int counter = 1; //Interactions limiter

//Calibration Variables
float Time_Gap = 3;


float Acceleration       = 0;
float Safe_distance      = 0;

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
  char *filename = "simulation_copy.txt";  
  FILE *fp = fopen(filename, "w");   // open the file for writing 
   if (fp == NULL)
    {
        printf("Error opening the file %s",filename);
        return -1;
    }

  //Number of times of looping
  while (counter < 50000)
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
	struct ACCenable index = logicBlockAccEnable(aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal);
  ACC_enabled = index.ACC_enabled;
  aux = index.aux;
  //----------------------Control block---------------------//

  //Detection of rain and change the Time_Gap
  Time_Gap = timeGap(Rain_sensor);

  //Limit of Ego_velo setup to don't exceed the safe velocity in ACC_enebled on
  ACC_speed_set = speedSet(ACC_speed_set);

  //ACC Model returning Acceleration
  struct ACCcontrol i = accelerationControl(ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval);
  Acceleration = i.Acceleration;
  Safe_distance = i.Safe_distance;

  counter++;



  // write to the text file
  fprintf(fp,"\nAcceleration: %.2f, ",Acceleration);
  fprintf(fp,"SafeDistance: %.2f, ", Safe_distance);
  fprintf(fp,"EgoVelocity: %.2f, ", Ego_velo);
  fprintf(fp,"Relative_D_past: %.5f, ", Relative_distance_past);
  fprintf(fp,"Relative_D_pres: %.5f, ", Relative_distance_pres);


  sleep(interval); // Simulation's clock
  }

  fclose(fp);   // close the file
}

