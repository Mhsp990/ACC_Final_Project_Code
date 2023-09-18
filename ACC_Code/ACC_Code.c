//ACC_code functions 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct ACCenable{
    bool ACC_enabled;
    bool aux;
};

struct ACCcontrol{
    float Acceleration;
    float Safe_distance;
};

//----------------------ACC enable logic block------------//

    struct ACCenable logicBlockAccEnable(bool aux, bool ACC_input, bool Fault_signal, float Ego_velo, bool Gas_pedal, bool Brake_pedal){
        struct ACCenable index;
        
        if(aux == 0 && ACC_input == 1 && Fault_signal == 0 && Ego_velo >= 11 && Gas_pedal == 0 && Brake_pedal == 0){
            index.aux = 1;
            index.ACC_enabled =1;
            return index;

        }else {
            if (ACC_input == 1 && aux == 1 && Fault_signal == 0 && Gas_pedal == 0 && Brake_pedal == 0){
                index.ACC_enabled = 1;

                return index;
            }else {
                index.ACC_enabled = 0;
                index.aux = 0;
                return index;
            }
        }
    }

//----------------------Control block---------------------//

    float timeGap(bool Rain_sensor){ //Detection of rain and change the Time_Gap
        float Time_Gap = 3;
        const float Default_Time_Gap = 3;
        Time_Gap = Rain_sensor ? Default_Time_Gap*2 : Default_Time_Gap;
        return Time_Gap;
    }

    float speedSet(float ACC_speed_set){ //Limit of Ego_velo to dont exceed the safe velocity
        if(ACC_speed_set > 120) ACC_speed_set = 120;
        if(ACC_speed_set < 40) ACC_speed_set = 40;
        return ACC_speed_set;
    }

    struct ACCcontrol accelerationControl(bool ACC_enabled, float Ego_velo, float Time_Gap, float ACC_speed_set, float Relative_velo,float Relative_distance_pres){
        struct ACCcontrol i;

        float SafeD_relD                    =0;
        const float D_default               = 10;
        float Control_x                     = 0;
        float Control_v                     = 0;
        const float Kverr_gain              = 0.5;
        const float Kxerr_gain              = 0.02;
        const float Kvx_gain                = 0.04;
        const float Ego_acceleration_min    = -5;
        const float Ego_acceleration_max    = 1.47;

        if(ACC_enabled){
        i.Safe_distance = (Ego_velo * Time_Gap) + D_default;
        SafeD_relD = i.Safe_distance - Relative_distance_pres;
        Control_x = (Relative_velo * Kvx_gain) - ((i.Safe_distance - Relative_distance_pres) * Kxerr_gain);
        Control_v = (ACC_speed_set - Ego_velo) * Kverr_gain;

        if (SafeD_relD > 0){   
            i.Acceleration = (Relative_velo * Kvx_gain) - (SafeD_relD * Kxerr_gain);
                
        }else{
            i.Acceleration = (Control_x < Control_v) ? Control_x : Control_v;
        }
        i.Acceleration = (i.Acceleration < Ego_acceleration_min) ? Ego_acceleration_min : (i.Acceleration > Ego_acceleration_max) ? Ego_acceleration_max : i.Acceleration;
        return i;

        }else{
            i.Acceleration = 0;
            return i;
        }
    }
