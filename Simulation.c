#include <stdio.h>
#include <math.h>


//Calibration Variables
float step = 0.001;
float D_time_gap = 3;
float D_distance = 10;
float V_set = 21;
int rain_signal = 0;
short int ACC_enable = 0;
const float Kverr_gain = 0.5;
const float Kxerr_gain = 0.0;
const float Kvx_gain   = 0.04;
const float Ego_acceleration_min = -5.0;
const float Ego_acceleration_max = 1.47;

//Logic control variables
int ACC_input = 0;
int Fault_signal = 0;
int Gas_pedal = 0;
int Brake_pedal = 0;

int counter = 0;
const int maxCounter = 100;


float V_lead = 25;
float Pos_lead = 100;
float Acceleration_lead = 0;
float Amplitude_move_lead = 0.1;

float Pos_ego = 0;
float V_ego = 17;
//float V_ego_INITIAL = 17; //Velocity when the ACC is pressed
float ACC_acceleration = 0;

float relativeDistance = 15;
float relativeSpeed = 10;
float safeDistance = 15;

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
		Control_x = (relativeDistance * Kvx_gain) - ((safeDistance - relativeDistance) * Kxerr_gain);
		Control_v = (V_set - V_ego) * Kverr_gain;

		if (safeDistance <= relativeDistance){
			
			Acceleration = (relativeDistance * Kvx_gain) - ((safeDistance - relativeDistance) * Kxerr_gain);
			
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