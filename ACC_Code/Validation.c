#include <stdio.h>
#include<stdbool.h>

bool checkCollision(bool ACC_enable/**<  Checks if ACC is Enabled */,
                    float relativeDistance/**<  Relative Distance Between Ego and Lead Car */) {

    if(ACC_enable == true && relativeDistance <= 0){
        printf("Collision detected. Take the necessary actions...\n");
        return true;
    }else{
        return false;
    }   
}

bool checkValidationSensors(bool ACC_enable/**<  Checks if ACC is Enabled */,
                            bool Fault_signal/**<  Checks if all sensors are working */, 
                            bool Gas_pedal/**<  Checks if gas pedal was pressed */, 
                            bool Brake_pedal/**<  Checks if break pedal was pressed */) {
    short int flag = 0;

    if(ACC_enable){
        if(Fault_signal){
            printf("ACC could not have triggered with a Fault signal on\n");
            flag++;
        }
        if(Gas_pedal){
            printf("ACC could not have triggered with a Gas pedal signal on\n");
            flag++;
        }
         if(Brake_pedal){
            printf("ACC could not have triggered with a Break pedal signal on\n");
            flag++;
        }
        if(flag){
            return 1;
        }
    }else{
        return 0;
    }
    return 0;
}

bool checkRainSafeDistance(bool ACC_enable/**<  Checks if ACC is Enabled */, 
                           bool rain_signal/**<  checks the presence of rain on the track */, 
                           const float D_distance/**<  default distance */, 
                           float V_ego/**<  velocity of ego car */, 
                           float safeDistance/**<  Safe distance */){
                            
    if(ACC_enable && rain_signal){
        if(safeDistance != (D_distance + V_ego*6)){
            printf("Safe distance is not correct for a wet street\n");
            return 1;
        }
    }
    return 0;
}

/*bool checkRelativeSafeDistance(bool ACC_enable, float safeDistance, float relativeDistance){
    if(ACC_enable){
        if(relativeDistance < safeDistance){
            printf("The vehicle is closer than the limit\n"); //This is not a error, is a mode change
            //This change from speed control to distance control mode
            return 1;
        }
    }
    return 0;
}*/




//---------------------- simulating calling the check functions------------------
/*
int main() {
    int ACC_enable = 1;
    float relativeDistance = 0;
    int Fault_signal = 1; 
    int Gas_pedal = 1; 
    int Brake_pedal = 1;

    int rain_signal = 1;
    int D_distance = 10;
    int V_ego = 17;
    float safeDistance = 112; //distance for a wet street 

    checkCollision(ACC_enable, relativeDistance);
    checkValidationSensors(ACC_enable, Fault_signal, Gas_pedal, Brake_pedal);
    checkRainSafeDistance(ACC_enable ,rain_signal, D_distance, V_ego, safeDistance);
    checkRelativeSafeDistance(ACC_enable, safeDistance, relativeDistance);
    return 0;
}

*/
