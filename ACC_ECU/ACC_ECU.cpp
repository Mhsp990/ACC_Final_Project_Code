//////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE	//	  
// Residência Tecnológica em Software Automotivo//      			  
// Final Project                                //
//////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
#include <stdlib.h>
#include <stdio.h>

//Id CAN Receive
#define ID_Rain_sensor			0xEC100003 //Data dictionary 
#define ID_Gas_pedal			0xEC100004 //Data dictionary
#define ID_Brake_pedal			0xEC100005 //Data dictionary 
#define ID_Fault_signal			0xEC100006 //Data dictionary 
#define ID_Ego_speed			0xEC100007 //Data dictionary
#define ID_Relative_distance	0xEC100008 //Data dictionary 
#define ID_Relative_speed		0xEC100009 //Data dictionary
#define ID_ACC_speed_set		0xEC300001 //Data dictionary
#define ID_ACC_input			0xEC300002 //Data dictionary

//Id mensagens CAN sand
#define ID_ACC_Acceleration			0xACC00001 //Data dictionary
#define ID_ACC_Brake_Acceleration	0xACC00002 //Data dictionary
#define ID_ACC_Enabled				0xACC00003 //Data dictionary
#define ID_ACC_Disabled				0xACC00004 //Data dictionary
 
//Variables received
long unsigned int 			mID;
unsigned char 				mDATA[8];
unsigned char 				mDLC  = 0;

//Variables received from CAN FRAMES
bool ACC_input     = 0; //Trigger for ACC_input. 
bool Rain_sensor   = 0;
bool Gas_pedal     = 0;
bool Brake_pedal   = 0;
bool Fault_signal  = 0;
float Ego_speed    = 0;
float Relative_distance = 0;
float Relative_speed    = 0;
float ACC_speed_set     = 0;
bool ACC_Enabled        = false;

bool ACC_enabled  = false;
bool aux  		  = 0;
float ACC_acceleration       = 0;
float ACC_brake_acceleration = 0;

#define BUFF_MAX 	10
#define BUFF_MIN 	00
volatile int		buffer = BUFF_MAX;

//Calibration Variables
const float D_default  = 10;
const float Default_Time_Gap = 3;
const float Kverr_gain = 0.5;
const float Kxerr_gain = 0.0;
const float Kvx_gain   = 0.04;

float Acceleration       = 0;
float Safe_distance      = 15;
float Control_v          = 0;
float Control_x          = 0;
float SafeD_relD		 = 0;
const float Ego_acceleration_min     = -5;
const float Ego_acceleration_max     = 1.47;

//Macros send
#define DLC_ACC					8
#define mEEC1_EXT_FRAME			1
static 							byte M  = 0;
static 							byte M1 = 0;

//CAN FRAME_DATA START 
unsigned char Data_ACC_Acceleration[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Data_ACC_Enabled[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//Is two of them necessary?

//MPC_CAN Object with Chip selector as digital pin 10
MCP_CAN CAN1(10);  

void setup() {
	//serial: baudrate = 115200
	Serial.begin(115200);
	
	//Start CAN controller : baudrate = 250K, clock=8MHz
	while (CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
        delay(200);        
    }
	Serial.println("MCP2515 can_send Started!");
	//Defines operation mode
	CAN1.setMode(MCP_NORMAL);
	pinMode(2, INPUT);
}

TASK(Can_Receive)
{
	if(!digitalRead(2)){  
		GetResource(res1);
		//Read can frame: mID = Identifier, mDLC = Data lenght, mDATA = data frame
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		if((mID & ID_ACC_input) == ID_ACC_input) {
			ACC_input = mDATA[4]; //Check data dictionary 
			TerminateTask();
		}
		if((mID & ID_Rain_sensor) == ID_Rain_sensor) {
			Rain_sensor = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
		if((mID & ID_Gas_pedal) == ID_Gas_pedal) {
			Gas_pedal = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
		if((mID & ID_Brake_pedal) == ID_Brake_pedal) {
			Brake_pedal = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
		if((mID & ID_Fault_signal) == ID_Fault_signal) {
			Fault_signal = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
		if((mID & ID_Ego_speed) == ID_Ego_speed) {
			Ego_speed = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
		if((mID & ID_Relative_distance) == ID_Relative_distance) {
			Relative_distance = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
		if((mID & ID_Relative_speed) == ID_Relative_speed) {
			Relative_speed = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
		if((mID & ID_ACC_speed_set) == ID_ACC_speed_set) {
			Relative_distance = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
		if((mID & ID_ACC_speed_set) == ID_ACC_speed_set) {
			ACC_speed_set = mDATA[4]; //Check data dictionary
			TerminateTask();
		}
	}
	ReleaseResource(res1);
	TerminateTask();
}

TASK(Logic_block)
{
	GetResource(res1);

	if(aux == 0 && ACC_input == 1 && Fault_signal == 0 && Ego_speed >= 11 && Gas_pedal == 0 && Brake_pedal == 0){
		aux = 1;
		ACC_enabled = 1;
	}else {
		if (ACC_input == 1 && aux == 1 && Fault_signal == 0 && Gas_pedal == 0 && Brake_pedal == 0){
			ACC_enabled = 1;
		}else {
			ACC_enabled = 0;
			aux = 0;
		}
	}
	Data_ACC_Enabled [0] = ACC_enabled;
	
	M = CAN1.sendMsgBuf(ID_ACC_Enabled, CAN_EXTID, DLC_ACC, Data_ACC_Enabled);
	
	if (M == CAN_OK) {
		Serial.println("can_send: SENT!");
		Serial.println("");
	} else if (M == CAN_SENDMSGTIMEOUT) {
		Serial.println("can_send: Message timeout!");      
	} else {    
		Serial.println("can_send: Error to send!");      
	}
	ReleaseResource(res1);
	TerminateTask();
}


TASK(Calculate_ACC_Acceleration) 
{
	Safe_distance = (Ego_speed * Default_Time_Gap) + D_default;
	SafeD_relD = Safe_distance - Relative_distance;
	Control_x = (Relative_speed * Kvx_gain) - ((Safe_distance - Relative_distance) * Kxerr_gain);
	Control_v = (ACC_speed_set - Ego_speed) * Kverr_gain;

	if (SafeD_relD > 0){
			
		Acceleration = (Relative_speed * Kvx_gain) - (SafeD_relD * Kxerr_gain);
			
	}else{
		Acceleration = (Control_x < Control_v) ? Control_x : Control_v;
	}
	
	Acceleration = (Acceleration < Ego_acceleration_min) ? Ego_acceleration_min : (Acceleration > Ego_acceleration_max) ? Ego_acceleration_max : Acceleration;
	
	Data_ACC_Acceleration[0] = Acceleration;
	
	ReleaseResource(res1);
	
	M1 = CAN1.sendMsgBuf(ID_ACC_Acceleration, CAN_EXTID, DLC_ACC, Data_ACC_Acceleration);
	
	if (M1 == CAN_OK) {
		Serial.println("can_send: mensagem transmitida com sucesso");
		Serial.println("");
	} else if (M1 == CAN_SENDMSGTIMEOUT) {
		Serial.println("can_send: Message timeout!");      
	} else {    
		Serial.println("can_send: Error to send!");      
	}
	TerminateTask();
}		

