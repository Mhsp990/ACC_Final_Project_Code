#ifndef VALIDATION_H
#define VALIDATION_H
#include "Validation.c"

    bool checkCollision(bool ACC_enable, float relativeDistance);
    bool checkValidationSensors(bool ACC_enable, bool Fault_signal, bool Gas_pedal, bool Brake_pedal);
    bool checkRainSafeDistance(bool ACC_enable, bool rain_signal, const float D_distance, float V_ego, float safeDistance);
    //bool checkRelativeSafeDistance(bool ACC_enable, float safeDistance, float relativeDistance);

#endif
