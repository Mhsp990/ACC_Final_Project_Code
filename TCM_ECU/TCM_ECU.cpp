//////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE		  
// Residência Tecnológica em Software Automotivo      			  
// PROJETO FINAL 
//////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
//#include "board.h"

// setspeed 
int setsp = 80;
// ACC state
bool Acc_state = false;

#define CAN_ID_M1			0xEC300002
#define CAN_ID_M2			0xEC300001

#define mEEC1_DLC			8
#define mEEC1_EXT_FRAME		1

// STORE FRAME_DATA
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


//Construct an MCP_CAN object and configure the selector chip for pin 10.
MCP_CAN CAN1(10);  //OUTPUT

void setup()
{
	// Initialize the serial interface: baudrate = 115200
	Serial.begin(115200);
	
	//Initialize the can controller: baudrate = 250K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
    {         
        delay(200);        
    }
	Serial.println("MCP2515 can_send initialized successfully!");
	//Changes to normal operating mode
	CAN1.setMode(MCP_NORMAL);
}

// SEND MESSAGE 1

TASK(SendCANM1)
{
	
	GetResource(res1);
	mEEC1_data[3]= 1; //WRITES TO CAN MESSAGE DATA FIELD
	ReleaseResource(res1);

	ret=CAN1.sendMsgBuf(CAN_ID_M1, CAN_EXTID, mEEC1_DLC, mEEC1_data);
	if (ret==CAN_OK)
	{
		Serial.println("can_send M1 OK"); 
	}
	else if (ret == CAN_SENDMSGTIMEOUT)
	{
		Serial.println("can_send M1: Message timeout!");      
	}
	else 
	{    
      Serial.println("can_send M1: Error to send!");      
	}	   
	
	TerminateTask();
}

//TASK SET SPEED
TASK(ReceiveSetSpeed)
{
	GetResource(res1);

	if (Serial.available() > 0) //Check serial buffer
	{
		setsp = Serial.parseInt(); //Read serial data and convert it to int.

		ReleaseResource(res1);
		Serial.read(); //CLEAR serial buffer (must do it, otherwise system executes it again with "zero" value

	}
	TerminateTask();
  
}


// SEND MESSAGE 2

TASK(SendCANM2)
{
	
	GetResource(res2);
	mEEC1_data[3]= 1; //WRITES TO CAN MESSAGE DATA FIELD
	ReleaseResource(res2);

	ret=CAN1.sendMsgBuf(CAN_ID_M2, CAN_EXTID, mEEC1_DLC, mEEC1_data);
	if (ret==CAN_OK)
	{
		Serial.println("can_send M2 OK"); 
	}
	else if (ret == CAN_SENDMSGTIMEOUT)
	{
		Serial.println("can_send M2: Message timeout!");      
	}
	else 
	{    
      Serial.println("can_send M2: Error to send!");      
	}	   
	
	TerminateTask();
}

//TASK ACC ACTIVATION 
TASK(AccOnOff)
{
	GetResource(res2);
	Acc_state = false;
	ReleaseResource(res2);
	TerminateTask();
  
}

