#include <stdio.h>
#include <math.h>


//Calibration Variables
float step = 0.001; //Time between iteractions
float D_time_gap = 3; //Base time gap between the ACC car (Ego) and front car (Lead).
float D_distance = 10; //Base distance between cars, regardless of timegap. 
float V_set = 21; //User defined velocity for ACC.
int rain_signal = 0; //Signal from rain sensor. Indicates rain surface, affecting required time gap.
short int ACC_enable = 0; //Represents the system input when the ACC is enabled.
const float Kverr_gain = 0.5;  //Gains
const float Kxerr_gain = 0.0;
const float Kvx_gain   = 0.04;
const float Ego_acceleration_min = -5.0;  //Max brake value
const float Ego_acceleration_max = 1.47;  //Max acceleration value - More than this and will be unconfortable

//Logic control variables
int ACC_input = 0;   //User input to enable ACC. Does not mean the acc is enabled (yet).
int Fault_signal = 0;  //System input indicating fault in at least one sensor. If true, ACC must be disabled.
int Gas_pedal = 0;  //User input indicating the Gas pedal has been pressed and ACC must be disabled.
int Brake_pedal = 0; //User input indicating the Brake pedal has been pressed and ACC must be disabled.

int counter = 0; // Counter of iteractions for this simulation
const int maxCounter = 10000; //Max number of iteractions for this simulation

//SIMULATION VALUES - Used to simulate sensors inputs for the ACC system
float V_lead = 25; //Initial front car speed in meters per second 
float Pos_lead = 100; //Initial front car position (meters)
float Acceleration_lead = 0; //Base front car acceleration - Causes variation in front car speed
float Amplitude_move_lead = 0.1; //Determines how much the front car acceleration should vary

float Pos_ego = 0; //Ego car initial position (meters)
float V_ego = 17; //Ego car initial speed (meters per second)
//float V_ego_INITIAL = 17; //Velocity when the ACC_input is pressed
float ACC_acceleration = 0; //Ego car initial acceleration. Changed by ACC output during runtime.

float relativeDistance = 15; // relativeDistance = Pos_lead - Pos_ego
float relativeSpeed = 10;   // relativeSpeed = V_lead - V_ego
float safeDistance = 15;   //  safeDistance = (D_time_Gap*2) + D_distance

int main(){

    do
    {
     //Generating Lead car data for this interaction. Based on previous values
    Acceleration_lead = generateLeadAcceleration(Amplitude_move_lead);
    V_lead = calculateLeadSpeed(V_lead, Acceleration_lead, step);
    Pos_lead =calculateEgoPosition(Pos_lead,V_lead,step,Acceleration_lead);

    safeDistance = D_distance + V_ego*(setTimeGap()); //Calculating safe distance
    relativeDistance = Pos_lead - Pos_ego; //Calculating relative distance
    relativeSpeed = V_lead - V_ego; //Calculating relative speed
    
    //if(ACC_input) V_ego_INITIAL = V_ego;

    //Using the ACC system with generated data from simulation
    ACC_enable = control(); //Check if ACC can be enabled
    ACC_acceleration = ACC_FUNCTION(); //Calculate ACC. If ACC is disable, returns 0;

    //Generating Ego car data for this interaction. Based on previous values
    V_ego = calculateEgoSpeed(Pos_ego, V_ego, step); //Calculating new speed
    Pos_lead = calculateEgoPosition(Pos_ego,V_ego,step,ACC_acceleration); //Calculating new pos

    //Run test for this interaction, such as check if there is colision

    counter++;
    } while (counter<=maxCounter);

    printf("SIMULATION ENDED");
    
    
}


//Functions

float calculateLeadSpeed(float Vo, float Acc, float dT )
{
    return Vo+(Acc*dT);
}

float calculateLeadPosition(float So, float Vo, float dT, float Acc)
{
    return So+(Vo*dT)+((Acc*dT)^2)/2;
}

float generateLeadAcceleration(float Amplitude_move_lead)
{
    return 0; //Use a Sin function with amplitude to variate the lead car speed
}

//EGO RELATED FUNCTIONS----------------------

float calculateEgoSpeed(float Vo, float Acc, float dT )
{
    return Vo+(Acc*dT);
}

float calculateEgoPosition(float So, float Vo, float dT, float Acc)
{
    return So+(Vo*dT)+ (pow(Acc*dT,2))/2;
}

float setTimeGap()
{
    if(rain_signal == 0) return D_time_gap;
    else return D_time_gap*2;
}


//----------------------------------
float control()
{
	if(ACC_input == 1 && Fault_signal == 0 && (V_ego >= 11 || V_ego <= 33) && Gas_pedal == 0 && Brake_pedal == 0){
		return 1;
	}
	else {
		return 0;
	}
	
}


float ACC_FUNCTION()
{
	if (ACC_enable){
		float Control_x =0, Control_v=0;
        	float Acceleration = 0;
		Control_x = (relativeSpeed * Kvx_gain) - ((safeDistance - relativeDistance) * Kxerr_gain);
		Control_v = (V_set - V_ego) * Kverr_gain;

		if (safeDistance <= relativeDistance){
			
			Acceleration = (relativeSpeed * Kvx_gain) - ((safeDistance - relativeDistance) * Kxerr_gain);
			
		}else{
			if (Control_x < Control_v){
				Acceleration = Control_x;
			}else{
				Acceleration = Control_v;
			}
		}
		if (Acceleration < Ego_acceleration_min){
			
			Acceleration = Ego_acceleration_min;
			
		}else if (Acceleration > Ego_acceleration_max) {
			
			Acceleration = Ego_acceleration_max;
			
		}
        return Acceleration;
		}
        return 0;
}		
