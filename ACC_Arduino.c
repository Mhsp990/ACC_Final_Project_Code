
bool ACC_input,rain_signal ,Gas_pedal_sensor,Brake_pedal_sensor,Fault_signal;
float V_set_Kmh;
float Distance_sensor, Ego_velocity_ms, Lead_velocity_ms, Ego_acceleration_min, Ego_acceleration_max, Ego_acceleration, Time_gap;

//Calibration Variables
unsigned int D_default,rain_counter;
float Time_gap_base, m, b;

//Function Headers -- Verify later how many times each variable is used, to decide if they will be parameters or global variables.
bool ACC_signal(); //Function used to enable or disable the ACC system
unsigned short int checkSensors(); //Function used to detect faulty sensors.
float calculateLeadVelocity(float previousDistance, float currentDistance, float time); //Calculate front car speed based on traveled distance. Requires two sequential measured distance values and time between those measurements.
float calculateEgoAcceleration(); //Calculate ACC acceleration.

    //Function headers regarding sensor data capture
    float getCurrentVelocity();
    float getCurrentDistance();
    float getRainData();

//End Function Headers

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  

}
