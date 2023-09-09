#include <stdbool.h>
#include "Arduino.h"
#include "mcp_can.h"

float Ego_pos, Lead_pos;
float Ego_velo, Lead_velo;
float Ego_ace = 0, Lead_ace = 0;
float Relative_velo = 0;

Lead_pos = 50.0;  // Initial lead car position (m)
Ego_pos  = 10.0;  // Initial ego car position  (m)

float Relative_distance_pres = Lead_pos - Ego_pos; //Relative distance between Ego and Lead Car.
float Relative_distance_past = Relative_distance_pres // Past value of relative distance.

Lead_vel = 90.0/3.6; // Initial lead car position (m/s)
Ego_vel  = 70.0/3.6; // Initial ego car position  (m/s)

float interval = 0.001;
int counter = 0;
//--------------------------------Can Setup--------------------------------/
MCP_CAN CAN1(10);
//Ego Speed
#define mSim1_ID			0xEC100007
#define mSim1_DLC			8
//Relative Distance
#define mSim2_ID			0xEC100008
#define mSim2_DLC			8
//Ego Acceleration
#define mSim3_ID			0xACC00001
#define mSim3_DLC			8
//Relative Speed
#define mSim4_ID			0xEC100009
#define mSim4_DLC			8

#define CAN_EXTID           1

unsigned char mSim1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char mSim2_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char mSim3_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};//Remove, maybe.
unsigned char mSim4_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char mDATA[8];
unsigned char mDLC = 0;

void setup{
Serial.begin(115200);
while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
{         
    delay(200);        
}
CAN1.setMode(MCP_NORMAL);
}
//--------------------------------Car Simul--------------------------------/
void loop ()
{
//Storage of the last value of Relative Distance
Relative_distance_past = Relative_distance_pres;

//Simulation of vehicles behavior
Ego_velo = Ego_velo + interval*Ego_ace;
Lead_velo = Lead_velo + interval*Lead_ace;

//Calculation of Ego Car position
Ego_pos += (Ego_velo*interval);

//Calculation of Lead Car position
Lead_pos += (Lead_velo*interval);

counter++;

Relative_distance_pres = Lead_pos - Ego_pos;
Relative_velo = Lead_velo - Ego_velo;

if(Relative_distance_pres > 200){
    Relative_distance_pres = 200;
}else{
    Relative_distance_pres = Relative_distance_pres;
}

CAN1.readMsgBuf(&mID, &mDLC, mDATA);

if((mID & mSim3_ID) == mSim3_ID){
    Ego_ace = mDATA[4];
    }else{
        Ego_ace = Ego_ace;
    }

//Relative Distance CAN Message Sender
mSim2_data[4] = Relative_distance_pres;
CAN1.sendMsgBuf(mSim2_ID,CAN_EXTID,mSim2_DLC,mSim2_data);

delay(20);

//Ego Speed CAN Message Sender
mSim1_data[4] = Ego_velo;
CAN1.sendMsgBuf(mSim1_ID,CAN_EXTID,mSim1_DLC,mSim1_data);

delay(20);

//Relative Speed CAN Message Sender
mSim4_data[4] = Relative_velo;
CAN1.sendMsgBuf(mSim4_ID,CAN_EXTID,mSim4_DLC,mSim4_data);

while (counter < 50000);
}

