
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

//! ACC enable logic block
/*! This function has the role of checking whether or not it is possible to activate the ACC */

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



    //! Time Gap
    /*! This function focuses on calculating the time gap between the car and the lead car */
    float timeGap(bool Rain_sensor){ 
        float Time_Gap = 3; 
        const float Default_Time_Gap = 3;
        Time_Gap = Rain_sensor ? Default_Time_Gap*2 : Default_Time_Gap;
        return Time_Gap;
    }
    //! Speed Set
    /*! This function has the role of checking whether or not it is possible to activate the ACC */
    /*! Limit of Ego_velo to dont exceed the safe velocity */
    float speedSet(float ACC_speed_set){ 
        if(ACC_speed_set > 120) ACC_speed_set = 120;
        if(ACC_speed_set < 40) ACC_speed_set = 40;
        return ACC_speed_set;
    }

    struct ACCcontrol accelerationControl(bool ACC_enabled, float Ego_velo, float Time_Gap, float ACC_speed_set, float Relative_velo,float Relative_distance_pres){
        struct ACCcontrol i;
        //! Acceleration Control
        /*!
        
        Function responsible for calculating and defining the acceleration to reach the setspeed or reduce the speed to meet the safety distance
        */
        float SafeD_relD                    =0;   // Difference between safe and relative distance   
        const float D_default               = 10; // Standard distance
        float Control_x                     = 0;  //  Intermediate variable to calculate relative distance 
        float Control_v                     = 0;  //  Intermediate variable to calculate relative distance 
        const float Kverr_gain              = 0.5; //   Model gain
        const float Kxerr_gain              = 0.02; //  Model gain
        const float Kvx_gain                = 0.04; //  Model gain
        const float Ego_acceleration_min    = -5; //   Minimum ego car acceleration
        const float Ego_acceleration_max    = 1.47; //  Max ego car acceleration

        if(ACC_enabled){ // Test if acc is ON/OFF
        i.Safe_distance = (Ego_velo * Time_Gap) + D_default; 
        SafeD_relD = i.Safe_distance - Relative_distance_pres;
        Control_x = (Relative_velo * Kvx_gain) - ((i.Safe_distance - Relative_distance_pres) * Kxerr_gain);
        Control_v = (ACC_speed_set - Ego_velo) * Kverr_gain;
        // Test whether it is possible to use ACC safely
        if (SafeD_relD > 0){   
            i.Acceleration = (Relative_velo * Kvx_gain) - (SafeD_relD * Kxerr_gain);
                
        }else{
            i.Acceleration = (Control_x < Control_v) ? Control_x : Control_v;
        }
        i.Acceleration = (i.Acceleration < Ego_acceleration_min) ? Ego_acceleration_min : (i.Acceleration > Ego_acceleration_max) ? Ego_acceleration_max : i.Acceleration;
        return i;
        // If the safety conditions are not accepted, do not accelerate the car
        }else{
            i.Acceleration = 0;
            return i;
        }
    }