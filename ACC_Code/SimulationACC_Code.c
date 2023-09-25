//Doxygen is still being implemented
// Simulation Code calling the ACC_code functions
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ACC_Code.h"
#include <unistd.h>


float Ego_ace = 0; /*!< ego car acceleration */
float Lead_ace = 0; /*!< lead car acceleration */

float Lead_pos = 100.0;  /*!< Initial lead car position (m) */
float Ego_pos  = 20.0;  /*!< Initial ego car position  (m) */

float Relative_distance_pres = 80; /*!<Relative distance between Ego and Lead Car. */
float Relative_distance_past = 80; /*!<Initial past value of relative distance. */

float Lead_velo = 50.0/3.6; /*!< Initial lead car position (m/s) */
float Ego_velo  = 70.0/3.6; /*!< Initial ego car position  (m/s)*/

float interval = 0.010; /*!< simulation clock, required by RStw-14*/
int counter = 1; /*!<Interactions limiter */

//Calibration Variables
const float Default_Time_Gap = 3; /*!< minimum time gap */
float Time_Gap = 3;                /*!< Time gap add(add to safe distance) */

float Acceleration       = 0;  /*!< Acceleration */
float Safe_distance      = 15; /*!< Safe distance  */




//Variables received from CAN FRAMES
bool ACC_input     = 1; /*!< User input variable, on or off  */ 
bool Rain_sensor   = 0; /*!<Variable that receives whether the rain sensor detected a wet road or not */ 
bool Gas_pedal     = 0; /*!<Variable that receives whether the gas pedal was pressed or not  */ 
bool Brake_pedal   = 0; /*!<Variable that receives whether the brake pedal was pressed or not  */ 
bool Fault_signal  = 0; /*!<Variable that checks whether the sensors are working correctly or not  */ 

float ACC_speed_set  = 100; /*!<start set spedd of ACC  */ 

bool ACC_enabled             = false; /*!< variable that defines whether ACC is on or not  */ 
bool aux  		               = 0; /*!< auxiliar variable   */  
float ACC_acceleration       = 0; /*!< ACC defined acceleration   */  
float ACC_brake_acceleration = 0; /*!< Return from acc in use of breaks   */ 

//--------------------------------Car Simulation--------------------------------/
int main ()
{
  char *filename = "SimulationACC_Code.txt";  
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
  Ego_velo = Ego_velo + interval*Ego_ace; // Required by RStw-12.
  Lead_velo = Lead_velo + interval*Lead_ace;
  
  //Calculation of Ego Car position
  Ego_pos += (Ego_velo*interval);

  //Calculation of Lead Car position, required by RStw–07
  Lead_pos += (Lead_velo*interval); 

  //Relative distance between Ego and Lead Car, required by RStw-01.
  Relative_distance_pres = Lead_pos - Ego_pos;
  

  // Sensor range, required by RStw-01.
  if(Relative_distance_pres > 200){
      Relative_distance_pres = 200;
  }else{
      Relative_distance_pres = Relative_distance_pres;
  }
  
  //ACC enable logic block
	struct ACCenable index = logicBlockAccEnable(aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal);
  ACC_enabled = index.ACC_enabled; 
  aux = index.aux;
  
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
  fprintf(fp,"RelativeDistance: %.2f, ", Relative_distance_pres);
  fprintf(fp,"EgoVelocity: %.1f, ", Ego_velo);
  fprintf(fp,"LeadVelocity: %.1f ", Lead_velo);
  

  sleep(interval); // Simulation's clock, required by RStw-14
  }

  fclose(fp);   // close the file
}

