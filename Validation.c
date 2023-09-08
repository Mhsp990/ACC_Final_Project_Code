#include <stdio.h>

float checkCollision(int ACC_enable, float relativeDistance) {
    if(ACC_enable == 1 && relativeDistance <= 0){
        printf("Collision detected. Take the necessary actions...\n");
        return 1;
    }
    return 0;        
}

float checkValidationSensors(short int ACC_enable, int Fault_signal, int Gas_pedal, int Brake_pedal) {
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
}

float checkRainSafeDistance(int ACC_enable, int rain_signal, int D_distance, int V_ego, float safeDistance){
    if(ACC_enable && rain_signal){
        if(safeDistance != (D_distance + V_ego*6)){
            printf("Safe distance is not correct for a wet street\n");
            return 1;
        }
    }
    return 0;
}

float checkRelativeSafeDistance(int ACC_enable, float safeDistance, float relativeDistance){
    if(ACC_enable){
    if(relativeDistance < safeDistance){
        printf("The vehicle is closer than the limit\n"); //This is not a error, is a mode change
        //This change from speed control to distance control mode
        return 1;
    }
}
}



//---------------------- simulating calling the check functions------------------
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


