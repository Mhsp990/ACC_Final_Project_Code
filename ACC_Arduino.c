
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


}


//End function code