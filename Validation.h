#ifndef VALIDATION_H
#define VALIDATION_H

    float checkCollision(int ACC_enable, float relativeDistance);
    float checkValidationSensors(short int ACC_enable, int Fault_signal, int Gas_pedal, int Brake_pedal);
    float checkRainSafeDistance(int ACC_enable, int rain_signal, int D_distance, int V_ego, float safeDistance);
    float checkRelativeSafeDistance(int ACC_enable, float safeDistance, float relativeDistance);

#endif
