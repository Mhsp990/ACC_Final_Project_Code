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
// EGO_SPEED 
int EGO_SPEED = 0;
// FAULT_SIGNAL
bool FAULT_SIGNAL = false;
//ACC FEEDBACK
bool ACC_FEEDBACK = false;
// ACC state
bool Acc_state = false;

static byte ret = 0;

long unsigned int 			mID;
unsigned char 				mDATA[8];
unsigned char 				mDLC  = 0;


#define CAN_ID_M1			0xEC300002
#define CAN_ID_M2			0xEC300001
#define CAN_ID_M3 			0xEC100007 
#define CAN_ID_M4 			0xEC100006 
#define CAN_ID_M5 			0xACC00003 

#define mEEC1_DLC			8
#define mEEC1_EXT_FRAME		1


// STORE FRAME_DATA
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


//Construct an MCP_CAN object and configure the selector chip for pin 10.
MCP_CAN CAN1(10);  //OUTPUT


// Macro para definicoes ID 
#define Fault_signal_ID 0xEC100006

long unsigned int 			mID;
unsigned char 				mDATA[8];
unsigned char 				mDLC  = 0;
bool Fault_signal  = 0;

//Variavel que armazena o FRAME_DATA
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};



//Constroi um objeto MCP_CAN e configura o chip selector para o pino 10.
MCP_CAN CAN1(10);  //A própria biblioteca já define este pino como output.


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


//______________ RECEIVE EGO  CAN EGO CURRENT SPEED________________________________________

TASK(ReceiveCANM3)
{
	if(!digitalRead(2))                        
	{
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		GetResource(res3);
		if((mID & CAN_ID_M3)==CAN_ID_M3) //Verify if the CAN message is the desired one. This is done by comparing the values.
		{
			EGO_SPEED = mDATA[3]; //Store the read data from the CAN message to local variable.
		}		
	ReleaseResource(res3);
	}
	TerminateTask();
}

//______________ RECEIVE Fault_signal________________________________________

TASK(ReceiveCANM4)
{
	if(!digitalRead(2))                        
	{
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		GetResource(res4);
		if((mID & CAN_ID_M4)==CAN_ID_M4) //Verify if the CAN message is the desired one. This is done by comparing the values.
		{
			FAULT_SIGNAL = mDATA[3]; //Store the read data from the CAN message to local variable.
		}		
	ReleaseResource(res4);
	}
	TerminateTask();
}

//______________ RECEIVE ACC FEEDBACK________________________________________

TASK(ReceiveCANM5)
{
	if(!digitalRead(2))                        
	{
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		GetResource(res5);
		if((mID & CAN_ID_M5)==CAN_ID_M5) //Verify if the CAN message is the desired one. This is done by comparing the values.
		{
			ACC_FEEDBACK = mDATA[3]; //Store the read data from the CAN message to local variable.
		}		
	ReleaseResource(res5);
	}
	TerminateTask();
}

TASK(Can_Receive)
{
	if(!digitalRead(2)){  

		GetResource(res1);
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		if((mID & Fault_signal_ID) == Fault_signal_ID) {
			Fault_signal = mDATA[4]; 
			TerminateTask();
		}
	}
}

