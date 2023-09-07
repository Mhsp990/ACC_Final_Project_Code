////////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE		  
//////////////////////////////////////////////////


//Defines CAN ID messages
#define CAN_ID_M1 0x18F00503
#define CAN_ID_M2 0x0CF00400
#define CAN_ID_M3 0x18FEF100   


#include "tpl_os.h"
#include "Arduino.h"
//#include "board.h"


//Variavel para armazenar informacoes do frame recebido
unsigned char 		mDLC = 0;
unsigned char 		mDATA[8];
long unsigned int 	mID;
//If you wish to send, check SENSORS_ECU.CPP example. This example only receives.

//Constroi um objeto MCP_CAN e configura o chip selector para o pino 10.
MCP_CAN CAN1(10); 

void setup()
{	
	//Inicializa a interface serial: baudrate = 115200
	Serial.begin(115200);
	
	//Inicializa o controlador can : baudrate = 500K, clock=08MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK)
    {         
        delay(200);        
    }	
	//Serial.println("Modulo CAN inicializado!");
	//Configura o controlador MCP2515 no modo normal.
	CAN1.setMode(MCP_NORMAL);  
	//Configura o pino de interrupção para recepção	
	pinMode(2, INPUT);   
	//Serial.println("ECU ICM INITIALIZING");

	//Serial.print("ID\t\tType\tDLC\tByte0\tByte1\tByte2");
	//Serial.println("\tByte3\tByte4\tByte5\tByte6\tByte7");
}
//////////////////////////////////////////////////////////////////////////////////				 					   
//  Periodo de execucao da task: 450 ms								   
//////////////////////////////////////////////////////////////////////////////////

//--------------------------SHOW DATA TASK---------------------------------
TASK(ShowDataTask)
{
GetResource(res1);

ReleaseResource(res1);

//Serial.println("-------------");
//Serial.println("Show DATA ICM:");

TerminateTask();
	
}


//-----------------------TASK DE RECEBER M1--------------------
TASK(ReceiveCANM1)
{

CAN1.readMsgBuf(&mID, &mDLC, mDATA);
GetResource(res1);
if((mID & CAN_ID_M1) == CAN_ID_M1) //Check if this is the desired CAN message
{
	marchaAtual = mDATA[3];
}
ReleaseResource(res1);

TerminateTask();
}

//-----------------------TASK DE RECEBER M2--------------------
TASK(ReceiveCANM2)
{
//SHOULD VERIFY pin 2 first for interruptions (INTERRUPTION on pin 2 means NEW CAN MESSAGE). See example in SENSORS_ECU.CPP
CAN1.readMsgBuf(&mID, &mDLC, mDATA);
GetResource(res1);
if((mID & CAN_ID_M2) == CAN_ID_M2) //MENSAGEM M2: Rotacao do motor
{

}
ReleaseResource(res1);
TerminateTask();
}


//-----------------------TASK DE RECEBER M3--------------------
TASK(ReceiveCANM3)
{

CAN1.readMsgBuf(&mID, &mDLC, mDATA);
GetResource(res1);
if((mID & CAN_ID_M3) == CAN_ID_M3) //CHECK IF IT IS CAN MESSAGE INTENDED
{
	velocidadeCalculadaL = mDATA[1];
	velocidadeCalculadaH = mDATA[2];
}
ReleaseResource(res1);

TerminateTask();
}

/*
void loop()
{

	if(!digitalRead(2))                        
	{
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		
		if( (mID & CAN_ID_M1) == CAN_ID_M1) //Verifica se o identificador da mensagem é a mensagem M1.	
			{
				marchaAtual = mDATA[3]; //Marcha atual esta no byte 4 (índice 3) do campo byte. Atribui este valor para a variavel.
				
			}
			else
			{
				if((mID & CAN_ID_M2) == CAN_ID_M2) //MENSAGEM M2: Rotacao do motor
				{
					rotacaoL = mDATA[3];
					rotacaoH = mDATA[4];
				}
				else
				{
					if((mID & CAN_ID_M3) == CAN_ID_M3) //MENSAGEM M3
					{
						velocidadeCalculadaL = mDATA[1];
						velocidadeCalculadaH = mDATA[2];
					}
				}
			}

	}//FIM IF digital read

}
*/
