
bool ACC_input,rain_signal ,Gas_pedal_sensor,Brake_pedal_sensor,Fault_signal;
float V_set_Kmh;
float Distance_sensor, Ego_velocity_ms, Relative_Velocity_ms, Ego_acceleration_min, Ego_acceleration_max, Ego_acceleration, Time_gap;

//Calibration Variables
unsigned int D_default,rain_counter;
float Time_gap_base, m, b, Min_velocity_Activation, Max_velocity_Activation;

//Function Headers -- Verify later how many times each variable is used, to decide if they will be parameters or global variables.
bool ACC_signal(); //Function used to enable or disable the ACC system
unsigned short int checkSensorsFault(); //Function used to detect faulty sensors.
float calculateLeadVelocity(float previousDistance, float currentDistance, float time); //Calculate front car speed based on traveled distance. Requires two sequential measured distance values and time between those measurements.
float calculateEgoAcceleration(); //Calculate ACC acceleration.



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




//End function code