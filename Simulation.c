#include <stdio.h>


//Calibration Variables
float step = 0.001;
float D_time_gap = 3;
float D_distance = 10;
float V_set = 70;
int rain_signal = 0;


float V_lead = 80;
float Pos_lead = 100;
float Acceleration_lead = 0;
float Amplitude_move_lead = 0.1;

float Pos_ego = 0;
float V_ego = 50;
float ACC_acceleration = 0;

float relativeDistance = 15;
float relativeSpeed = 10;
float safeDistance = 15;

int main(){

    //Generating Lead car data for this interaction. Based on previous values
    Acceleration_lead = generateLeadAcceleration(Amplitude_move_lead);
    V_lead = calculateLeadSpeed(V_lead, Acceleration_lead, step);
    Pos_lead =calculateEgoPosition(Pos_lead,V_lead,step,Acceleration_lead);

    safeDistance = D_distance + V_ego*(setTimeGap());
    relativeDistance = Pos_lead - Pos_ego;
    relativeSpeed = V_lead - V_ego;
    


    //Generating Ego car data for this interaction. Based on previous values


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
    return 0; //Use a Sin function with amplitude a
}

//EGO RELATED FUNCTIONS----------------------

float calculateEgoSpeed(float Vo, float Acc, float dT )
{
    return Vo+(Acc*dT);
}

float calculateEgoPosition(float So, float Vo, float dT, float Acc)
{
    return So+(Vo*dT)+((Acc*dT)^2)/2;
}

float ACC_function()
{

}


float setTimeGap()
{
    if(rain_signal == 0) return D_time_gap;
    else return D_time_gap*2;
}
