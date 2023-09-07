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

//Variables for frame data CAN message
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char mDATA[8];
unsigned char mDLC = 0;


//Create MCP_CAN object and configure Chip select pin as digital 10.
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



//---------------------------------SEND CAN M3---------------------------//
TASK(SendCANM2)
{
	GetResource(res1);

	ReleaseResource(res1);

	TerminateTask();  
}

//---------------------------------SEND CAN M3---------------------------//
TASK(ReceiveCANM1)
{
	//If there is a interrupt in this pin, there is a message to be read in the CAN buffer.
	if(!digitalRead(2))                        
	{
		unsigned int var = 0;
		
		//Lê os dados: mID = identificador, mDLC = comprimento, mDATA = dados do freame
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);

		GetResource(res1);
		var = (rotacaoH<<8) | (rotacaoL);
		if((mID & CAN_ID_M1)==CAN_ID_M1) //Verify if the CAN message is the desired one. This is done by comparing the values.
		{
			marchaAtual = mDATA[3]; //Store the read data from the CAN message to local variable.
		}		
	ReleaseResource(res1);
	}
	TerminateTask();
}

//---------------------------------SEND CAN M3---------------------------//
TASK(CalcEgoVel)
{
	GetResource(res1);

	ReleaseResource(res1);

	TerminateTask();  
}

//---------------------------------SEND CAN M3---------------------------//
TASK(CalcRelativeVel)
{
	GetResource(res1);

	ReleaseResource(res1);

	TerminateTask();  
}

//---------------------------------SEND CAN M3---------------------------//
TASK(CalcRelativeDis)
{
	GetResource(res1);

	ReleaseResource(res1);

	TerminateTask();  
}

/*
void loop()
{
	if (Serial.available() > 0) //Verifica o buffer da serial.
	{
		//FALTA CONSERTAR, O RESULTADO EH UM FLOAT E POSSUI 4 BYTES
		unsigned int var;
		var = Serial.read(); //Lê a serial. Como o intervalo da marcha é suficiente para um byte, não há perdas na "conversão" para byte
		//var = var*8; //A mesma coisa de var * 8.
		
		rotacaoL=var;
		rotacaoH=var>>8;

		Serial.read(); //Limpa dados indesejados da serial, tal como o new line ou outros caracteres que não faziam parte da informacao original
		
	}
}
*/