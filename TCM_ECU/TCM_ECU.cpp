//////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE		  
// Residência Tecnológica em Software Automotivo      			  
// Módulo: SWT1 - RTOS e Sistemas Distribuídos 
//////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
//#include "board.h"


//Macro para definicoes
#define CAN_ID_M1			0x18F00503
//#define mEEC1_ID			0x0CF00400
#define mEEC1_DLC			8
#define mEEC1_EXT_FRAME		1



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
	//Inicializa a interface serial: baudrate = 115200
	Serial.begin(115200);
	
	//Inicializa o controlador can : baudrate = 250K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
    {         
        delay(200);        
    }
	Serial.println("MCP2515 can_send inicializado com sucesso!");
	//Modifica para o modo normal de operação
	CAN1.setMode(MCP_NORMAL);
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