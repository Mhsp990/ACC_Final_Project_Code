//////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE    //  
//////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"

// define para msg de Break, Gas, Rain e Fault
#define Brake_pedal_ID  0xEC100005
#define Gas_pedal_ID    0xEC100004
#define Time_gap_ID  	0xEC100003
#define Fault_signal_ID 0xEC100006

#define mEEC1_DLC		8
#define EXT_FRAME 		1

static byte M  = 0;
static byte M1 = 0;
static byte M2 = 0;
static byte M3 = 0;


//Variables sensors Break, Gas, Rain e Fault
unsigned int Time_gap_base = 3;
unsigned int Time_gap      = 0;
bool Break_pedal_sensor    = 0;
bool Gas_pedal_sensor      = 0;
bool Fault_signal          = 0;
bool Rain                  = 0;


//Variables for frame data CAN message
unsigned char Gas_pedal_data[8]    = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Break_pedal_data[8]  = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Fault_signal_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Time_gap_data[8]         = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

unsigned char mDATA[8];
unsigned char mDLC = 0;


//This pin (10) will be defined as the CS and put on output mode
MCP_CAN CAN1(10);

void setup()
{
	pinMode(2, INPUT); 
	//serial: baudrate = 115200
	Serial.begin(115200);
	
	//Inicializa o controlador can : baudrate = 250K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
    {         
        delay(200);        
    }
	//Serial.println("MCP2515 can_send inicializado com sucesso!");
	//Modifica para o modo normal de operação
	CAN1.setMode(MCP_NORMAL);
}


TASK(SenderTimeGap){
	
	if(Rain){
		Time_gap = (2*Time_gap_base);
	}else{
		Time_gap = Time_gap_base;
	}
	//Write CAN message.
	Time_gap_data[1] = (char)Time_gap;
	

	//Send CAN message with previous DATA and with the desired ID. Desired ID on "CAN_ID_M2" field.
	M = CAN1.sendMsgBuf(Time_gap_ID,EXT_FRAME,mEEC1_DLC,Time_gap_data);

	if (M == CAN_OK) //Check for sucess.
	{
		 Serial.print("CAN Rain SENT.");
		//Could also print the value that was sent.
	}
	TerminateTask();

}


TASK(SenderBreakSensor){

	Break_pedal_data[1] = (char)Break_pedal_sensor;
	
	M1 = CAN1.sendMsgBuf(Brake_pedal_ID,EXT_FRAME,mEEC1_DLC,Break_pedal_data);

	if (M1 == CAN_OK) {
		 Serial.print("CAN MSG Break pedal SENT:");
	}
	TerminateTask();
}


TASK(SenderGasSensor){
	
	Gas_pedal_data[1] = (char)Gas_pedal_sensor;

	M2 = CAN1.sendMsgBuf(Gas_pedal_ID,EXT_FRAME,mEEC1_DLC,Gas_pedal_data);

	if (M2==CAN_OK) {
		 Serial.print("CAN MSG Gas pedal SENT:");
	}
	TerminateTask();
}


TASK(SenderFault){
	
	Fault_signal_data[1]= (char)Fault_signal;
	
	M3 = CAN1.sendMsgBuf(Fault_signal_ID,EXT_FRAME,mEEC1_DLC,Fault_signal_data);

	if (M3 == CAN_OK)
	{
		 Serial.print("CAN MSG fault SENT:");
	}
	TerminateTask();
}


//Ego_speed
//Relative_speeed
//Relative_distance
