#include <stdbool.h>
#include "Arduino.h"
#include "mcp_can.h"

//--------------------------------Can Setup--------------------------------/
MCP_CAN CAN1(10);
//Ego Velocity
#define mSim1_ID			0xEC100007
#define mSim1_DLC			8
//Relative Distance
#define mSim2_ID			0xEC100008
#define mSim2_DLC			8
//Ego Aceleration
#define mSim3_ID			0xACC00001
#define mSim3_DLC			8

#define CAN_EXTID           1

unsigned char mSim1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

unsigned char mSim2_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

unsigned char mDATA[8];
unsigned char mDLC = 0;

void setup()
{
	Serial.begin(115200);
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
    {         
        delay(200);
    }
	Serial.println("MCP2515 can_send inicializado com sucesso!");
	CAN1.setMode(MCP_NORMAL);
//	pinMode(ECU2_CAN1_INT, INPUT);
}

//--------------------------------Car Decla--------------------------------/

float Ego_pos, Lead_pos;
float Ego_velo, Lead_velo;
float Ego_ace = 0, Lead_ace = 0;

Lead_pos = 50.0;  // Initial lead car position (m)
Ego_pos  = 10.0;  // Initial ego car position  (m)

float Relative_distance_pres = Lead_pos - Ego_pos; //Relative distance between Ego and Lead Car.
float Relative_distance_past = Relative_distance_pres // Past value of relative distance.

Lead_vel = 90.0/3.6; // Initial lead car position (m/s)
Ego_vel  = 70.0/3.6; // Initial ego car position  (m/s)

float interval = 0.001;
int counter = 0;


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

Relative_distance_pres = Lead_pos - Ego_pos;

if(Relative_distance > 200){
    Relative_distance_pres = 200;
}else{
    Relative_distance_pres = Relative_distance_pres;
}

//Reading CAN network for Ego Acelaration
if(!digitalRead("ECU2_CAN1_INT")){
CAN1.readMsgBuf(&mID, &mDLC, mDATA);
if((mID & mSim3_ID) == mSim3_ID){
    Ego_ace = mDATA[5];
    }else{
        Ego_ace = Ego_ace;
    }
}
//Sending CAN message with Relative Distance
CAN1.sendMsgBuf(mSim2_ID,CAN_EXTID,mSim2_DLC,mSim2_data);

delay(10);

//Sending CAN message with Ego velocity
CAN1.sendMsgBuf(mSim1_ID,CAN_EXTID,mSim1_DLC,mSim1_data);

counter++;

while (counter < 50000);
}

