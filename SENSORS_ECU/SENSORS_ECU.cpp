//////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE		  
//////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
//#include "board.h"


//Macro para definicoes
#define CAN_ID_M1			0x18F00503  //Change for the CAN msg id
#define CAN_ID_M2 			0x0CF00400
#define CAN_ID_M3 			0x18FEF100  

#define mEEC1_ID			0x0CF00400
#define mEEC1_DLC			8
#define mEEC1_EXT_FRAME		1

// define para msg de Break, Gas, Rain e Fault
#define Brake_pedal_ID 0xEC100005
#define Gas_pedal_ID 0xEC100004
#define Rain_sensor_ID 0xEC100003
#define Fault_signal_ID 0xEC100006

//Variáveis para sensores Break, Gas, Rain e Fault
bool Break_pedal_sensor = false;
bool Gas_pedal_sensor = false;
bool Fault_signal = false;

bool Rain = false;
unsigned int Time_gap_base = 3;
unsigned int Time_gap;

//Variables for frame data CAN message
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char mDATA[8];
unsigned char mDLC = 0;

//Create MCP_CAN object and configure Chip select pin as digital 10.
MCP_CAN CAN1(10);  //This pin (10) will be defined as the CS and put on output mode

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



//---------------------------------Send CAN Rain msg.
TASK(SenderRainSensor){

	GetResource(res1);
	if(Rain){
		Time_gap=2*Time_gap_base;
	}else{
		Time_gap = Time_gap_base;
	}
	//Write CAN message.
	mEEC1_data[4]= Time_gap;
	ReleaseResource(res1);

	//Send CAN message with previous DATA and with the desired ID. Desired ID on "CAN_ID_M2" field.
	ret=CAN1.sendMsgBuf(Rain_sensor_ID,CAN_EXTID,mEEC1_DLC,mEEC1_data);

	if (ret==CAN_OK) //Check for sucess.
	{
		 Serial.print("CAN Rain SENT.");
		//Could also print the value that was sent.
	}
	TerminateTask();

}
//---------------------------------Send CAN Break msg.
TASK(SenderBreakSensor){

	GetResource(res1);
	mEEC1_data[4]= Break_pedal_sensor;
	ReleaseResource(res1);

	ret=CAN1.sendMsgBuf(Brake_pedal_ID,CAN_EXTID,mEEC1_DLC,mEEC1_data);

	if (ret==CAN_OK) 
	{
		 Serial.print("CAN MSG Break pedal SENT:");
		 Serial.println(Break_pedal_sensor);
	}
	TerminateTask();  
}
//---------------------------------Send CAN Gas msg.
TASK(SenderGasSensor){
	
	GetResource(res1);
	mEEC1_data[4]= Gas_pedal_sensor;
	ReleaseResource(res1);

	ret=CAN1.sendMsgBuf(Brake_pedal_ID,CAN_EXTID,mEEC1_DLC,mEEC1_data);

	if (ret==CAN_OK) 
	{
		 Serial.print("CAN MSG Gas pedal SENT:");
		 Serial.println(Gas_pedal_base);
	}
	TerminateTask();  
}
//---------------------------------Send CAN Fault msg.
TASK(SenderFault){
	
	GetResource(res1);
	mEEC1_data[4]= Fault_signal;
	ReleaseResource(res1);

	ret=CAN1.sendMsgBuf(Fault_signal_ID,CAN_EXTID,mEEC1_DLC,mEEC1_data);

	if (ret==CAN_OK) 
	{
		 Serial.print("CAN MSG fault SENT:");
		 Serial.println(Fault_signal_base);
	}
	TerminateTask();  
}


