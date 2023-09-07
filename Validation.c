#include <stdio.h>

float checkCollision(int ACC_enable, double relativeDistance) {
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

int main() {
    int ACC_enable = 1;
    double relativeDistance = -5.0;
    int Fault_signal = 1; 
    int Gas_pedal = 1; 
    int Brake_pedal = 1; 
    int rain_signal = 1;

    checkCollision(ACC_enable, relativeDistance);
    checkValidationSensors(ACC_enable, Fault_signal, Gas_pedal, Brake_pedal)

    return 0;
}


