/*
Authors:                                                  Email for contact
        Matheus Henrique de Souza Passos            -     matheushenriquepassos45@gmail.com
        Leonardo Mello                              -     
        Gabriel Oliveira                            -     gabriel_lopeslomeu@hotmail.com
        Lucas Sobral                                -     lucasdecarvalhosobral@gmail.com
        Vitor Fassarano                             -

Adaptive Cruise Control in C language (on arduino) for Final Project of "Automotive Software Development" on UFPE (Brazil, Recife, PE).
In this repository lies the C Code for Adaptive Cruise Control, projected after the model we created on simulink/mathlab. 
Link for the simulink/mathlab ACC mode: https://github.com/leonardomello27/Projeto_Final
*/
#include <stdbool.h>

bool ACC_input, rain_signal,Fault_signal;
float V_set_Kmh;
float Distance, Ego_velocity_ms, Relative_Velocity_ms, Ego_acceleration_min, Ego_acceleration_max, Ego_acceleration, Time_gap;

//Sensors fault signals
bool Velocity_sensor, Rain_sensor, Distance_sensor, Brake_pedal_sensor, Gas_pedal_sensor;

//Calibration Variables
unsigned int D_default,rain_counter;
float Time_gap_base, m, b, Min_velocity_Activation, Max_velocity_Activation;

//Function Headers -- Verify later how many times each variable is used, to decide if they will be parameters or global variables.
bool ACC_signal(); //Function used to enable or disable the ACC system
unsigned short int checkSensorsFault(); //Function used to detect faulty sensors.
float calculateLeadVelocity(float previousDistance, float currentDistance, float time); //Calculate front car speed based on traveled distance. Requires two sequential measured distance values and time between those measurements.
float calculateEgoAcceleration(); //Calculate ACC acceleration.
float calculateTimeGap(); //Function to calculate timegap. Based on "Time_gap_base" and rain_signal (in this case, rain_counter)


//End Function Headers

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  

}


//Functions code bellow

bool ACC_signal()
{
    if(ACC_input && (Ego_velocity_ms > Min_velocity_Activation && (Ego_velocity_ms <= Max_velocity_Activation)))
    {
        if(!checkSensorsFault()) //Check if all sensors are working.
        {
            return true;
        }
    }
    else return false; //Must report to user when it is disabled or enabled and it`s reason

}// End ACC_signal function


unsigned short int checkSensorsFault() //Check if any sensor stopped working
{

    unsigned short int flags =0;
    //Each bit of this 8 bits variables represents an sensor condition. 0 is working, 1 is fault detected.
    //from right(Least Significant Bit) to left (Most Significant Bit):
    //bit 0 represents velocity sensor, bit 1 represents rain sensor, bit 2 represents distance sensor
    //bit 3 represents brake pedal sensor, bit 4 represents gas pedal sensor.

     if (Velocity_sensor) {
    flags |= (1 << 0);  // Defines LSB as high [0]
  }
  if (Rain_sensor) {
    flags |= (1 << 1);  // Defines second LSB number [1] as high
  }
  if (Distance_sensor) {
    flags |= (1 << 2);  // Defines bit number [2] as high
  }
  if (Brake_pedal_sensor) {
    flags |= (1 << 3);  // Defines bit number [3] as high
  }
  if (Gas_pedal_sensor) {
    flags |= (1 << 4);  // Defines bit number [4] as high
  }

    return flags;


} //End checkSensorsFault function

float Dy_Dx(float x, float y)
{
    return((x - y)/2);
}

float calculateLeadVelocity(float previousDistance, float currentDistance, float time_current)
{
  float runge_kutta_step;
  float K_1, K_2, K_3, K_4, K_5;
  float Relative_Velocity_ms_past;

  int n = (int)((currentDistance - previousDistance) / runge_kutta_step);

  // Iterate for number of iterations
  float Relative_Velocity_ms = Relative_Velocity_ms_past;
  for (int i=1; i<=n; i++)
  {
    K_1 = runge_kutta_step*Dy_Dx(previousDistance, Relative_Velocity_ms);
    K_2 = runge_kutta_step*Dy_Dx(previousDistance + 0.5*runge_kutta_step, Relative_Velocity_ms + 0.5*K_1);
    K_3 = runge_kutta_step*Dy_Dx(previousDistance + 0.5*runge_kutta_step, Relative_Velocity_ms + 0.5*K_2);
    K_4 = runge_kutta_step*Dy_Dx(previousDistance + runge_kutta_step, Relative_Velocity_ms + K_3);
    Relative_Velocity_ms = Relative_Velocity_ms + (1.0/6.0)*(K_1 + 2*K_2 + 2*K_3 + K_4);;
    previousDistance = previousDistance + runge_kutta_step;
  }
  //Must check about the first iteraction, as there will not be a "previousDistance".
  //For the first iteratction, should we consider "previousDistance" as zero? Or wait for second iteraction?
      //Sugestion 1: Wait for second interaction. Consider, on main function, using the first interaction AFTER
        //the ACC enable signal to generate the previousDistance and only starting by the second interaction the ACC
        //would start running.
      //Sugestion 2: Consider, for simulation purposes, an initial distance between them, regardless if the ACC
        //is enabled. That would generate the previous distance. Also, the system should simulate regardless if
        //the ACC is enabled or not (In my opinion)
      //Question for YOUR first suggestion: Would it not result in 0 divided by something = zero?
  //Problem for first suggestion: currentDistance/Elapsed time would give incredible speed!
  return Relative_Velocity_ms;
} //END calculateLeadVelocity Function


float calculateTimeGap() 
{
  //If the rain_signal is true, rain_counter will raise. When it reacher a certain value, the timeGap is changed based on Time_gap_base
  //If there is no rain detected (rain_counter < value), returns the Time_gap_base calibrated value
  if(rain_counter < 10)   return Time_gap_base*2;
  else  return Time_gap_base;

} //End calculateTimeGap


//End function code