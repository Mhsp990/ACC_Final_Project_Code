
#ifndef ACC_CODE_H
#define ACC_CODE_H
#include "ACC_Code.c"
#include <stdbool.h>

struct ACCenable logicBlockAccEnable(bool aux, bool ACC_input,
                                     bool Fault_signal, float Ego_velo,
                                     bool Gas_pedal, bool Brake_pedal);
struct ACCcontrol accelerationControl(bool ACC_enabled, float Ego_velo,
                                      float Time_Gap, float ACC_speed_set,
                                      float Relative_velo,
                                      float Relative_distance_pres);
float timeGap(bool Rain_sensor);
float speedSet(float ACC_speed_set);

#endif
