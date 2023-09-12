#include <stdbool.h>
#include "Arduino.h"
#include "mcp_can.h"


float Ego_ace       = 0;
float Lead_ace      = 0;
float Relative_velo = 0;
float Lead_pos      = 50.0;                            //Initial lead car position (m)
float Ego_pos       = 10.0;                            //Initial ego car position  (m)
float Relative_distance_pres = Lead_pos - Ego_pos;     //Relative distance between Ego and Lead Car.
float Relative_distance_past = Relative_distance_pres; //Past value of relative distance.
float Lead_velo = 90.0/3.6;                            //Initial lead car position (m/s)
float Ego_velo  = 70.0/3.6;                            //Initial ego car position  (m/s)
float interval  = 0.001;
int   counter   = 0;

//--------------------------------Can Setup--------------------------------/
MCP_CAN CAN1(10);

#define Ego_Speed_ID			0xEC100007
#define Relative_Distance_ID	0xEC100008 
#define Ego_Acceleration_ID		0xACC00001
#define Relative_Speed_ID		0xEC100009 

#define DLC			8
#define CAN_EXTID   1

unsigned char Ego_Speed_data[8]         = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Relative_Distance_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char mSim3_data[8] 			= {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Relative_Speed_ID[8]      = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char mDATA[8];
unsigned char mDLC = 0;
long unsigned int mID = 0;
static byte ret = 0;

void setup(){
	Serial.begin(115200);
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {           
		delay(200);        
	}
	CAN1.setMode(MCP_NORMAL);
}


void loop ()
{
	
	Relative_distance_past = Relative_distance_pres; //Storage of the last value of Relative Distance

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
	if(!digitalRead(2)){
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		if((mID & Ego_Acceleration_ID) == Ego_Acceleration_ID){
			Ego_ace = mDATA[1];
		}else{
        Ego_ace = Ego_ace;
    }

	//Relative Distance CAN Message Sender
	Ego_Speed_data[1] = (char)Relative_distance_pres;
	
	CAN1.sendMsgBuf(Ego_Speed_ID,CAN_EXTID,DLC,Ego_Speed_data);

	delay(1);

	//Ego Speed CAN Message Sender
	Relative_Distance_data[1] = (char)Ego_velo;
	CAN1.sendMsgBuf(Relative_Distance_ID,CAN_EXTID,DLC,Relative_Distance_data);

	delay(1);

	//Relative Speed CAN Message Sender
	Relative_Speed_data[4] = Relative_velo;
	CAN1.sendMsgBuf(Relative_Speed_ID,CAN_EXTID,DLC,Relative_Speed_data);
	
	delay(100);

	if (counter == 50000){
		exit(0);
	}
}

