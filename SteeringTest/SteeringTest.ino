/*
 Name:		SteeringTest.ino
 Created:	4/29/2016 5:41:22 PM
 Author:	xn
 Editor:	http://www.visualmicro.com
*/

//x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x
// CAN Bus initialization
//START
//x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x

// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>
#include <stdio.h>
#define INT8U unsigned char

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

unsigned char node = 0x00; //creating a variable for choosing the CAN node
unsigned char stmp[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // can Message!!

//int len = 0;
int TimeBySide = 2000;

//x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x
// CAN Bus initialization
//END
//x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x

INT8U len = 0;
INT8U buf[8];
INT8U lenpos = 0;
INT8U bufpos[8];
INT8U lenspeed = 0;
INT8U bufspeed[8];
INT32U canId = 0x000;



//PWM ports
int PWMinSA1 = 10;    // PWM signal to put piston accel in
int PWMoutSA1 = 11;    // PWM signal to put piston accel out
int PWMinSA2 = 12;    // PWM signal to put piston brake in
int PWMoutSA2 = 13;    // PWM signal to put piston brake out

int ParkControl = A5;    // Remote control right

int AccelLogic = LOW;
int BrakeLogic = LOW;
int RightLogic = LOW;
int LeftLogic = LOW;
int ParkLogic = LOW;

int AccelControl = A0;    // Remote control Accel
int BrakeControl = A1;    // Remote control Brake

int timesteps = 100;

int sa1powrel = 40;
int sa1tenrel = 42;
int sa1ienrel = 44;


int sa2powrel = 41;
int sa2tenrel = 43;
int sa2ienrel = 45;


void setup() {

	Serial.begin(115200);

START_INIT:

	if (CAN_OK == CAN.begin(CAN_500KBPS))                  // init can bus : baudrate = 500k
	{
		Serial.println("CAN BUS Shield init ok!");
		Serial.println("");
	}
	else
	{
		Serial.println("CAN BUS Shield init fail");
		Serial.println("Init CAN BUS Shield again");
		delay(100);
		goto START_INIT;
	}

	// put your setup code here, to run once:
	
	// SAP- Maximum positioning speed
	node = 0x05;
	stmp[0] = 0x05;
	stmp[1] = 0x04;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x00;
	stmp[6] = 0x64;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);
	CAN.readMsgBuf(&len, buf);
	delay(100);

	//SAP- Max Acceleration
	node = 0x05;
	stmp[0] = 0x05;
	stmp[1] = 0x05;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x01;
	stmp[6] = 0xf4;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);
	CAN.readMsgBuf(&len, buf);
	delay(100);

	//SAP- Set Max current 
	node = 0x05;
	stmp[0] = 0x05;
	stmp[1] = 0x06;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x00;
	stmp[6] = 0x64;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);
	CAN.readMsgBuf(&len, buf);
	delay(100);

	//SAP- Set standby current
	node = 0x05;
	stmp[0] = 0x05;
	stmp[1] = 0x07;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x00;
	stmp[6] = 0x32;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);
	CAN.readMsgBuf(&len, buf);
	delay(100);

	//SAP- Set microstep solution
	node = 0x05;
	stmp[0] = 0x05;
	stmp[1] = 0x08c;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x00;
	stmp[6] = 0x04;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);
	CAN.readMsgBuf(&len, buf);
	delay(100);

	
	//SAP Reset actual position
	node = 0x05;
	stmp[0] = 0x05;
	stmp[1] = 0x01;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x00;
	stmp[6] = 0x00;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);
	CAN.readMsgBuf(&len, buf);
	delay(100);
	
	//Move to position
	node = 0x05;
	stmp[0] = 0x04;
	stmp[1] = 0x01;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x9c;
	stmp[6] = 0x40;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);
	CAN.readMsgBuf(&len, buf);
	delay(100);

}


void readCANbusPos()  
{
	
	//Get axis parameter- Current position
	node = 0x05;
	stmp[0] = 0x06;
	stmp[1] = 0x01;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x00;
	stmp[6] = 0x00;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);


	//Get Can bus data
	CAN.readMsgBuf(&lenpos, bufpos);
	canId = CAN.getCanId();
	//Print data to the serial console
	Serial.println("CAN_BUS GET DATA!");
	Serial.print("CAN ID:");
	Serial.println(canId);
	Serial.print("data len="); Serial.println(lenpos);
	//This loops through each byte of data and prints it
	for (int i = 0; i < lenpos; i++)
	{
		Serial.print(bufpos[i], HEX); Serial.print("\t");
	}
	Serial.println();

	Serial.print("\nPosition:  ");

	int byte3 = bufpos[3];
	int byte2 = bufpos[4];
	int byte1 = bufpos[5];
	int byte0 = bufpos[6];

	signed long allpos = (signed long)byte3 << 24 | (unsigned long)byte2 << 16 | (unsigned long)byte1 << 8 | (unsigned long)byte0;
	//Serial.println(allpos, HEX);
	
	Serial.println(allpos, DEC);


	delay(500);
	
}


void readCANbusSpeed()
{
	//Get axis parameter- Current Speed
	node = 0x05;
	stmp[0] = 0x06;
	stmp[1] = 0x03;
	stmp[2] = 0x00;
	stmp[3] = 0x00;
	stmp[4] = 0x00;
	stmp[5] = 0x00;
	stmp[6] = 0x00;
	stmp[7] = 0x00;
	CAN.sendMsgBuf(node, 0, 8, stmp);

	//Get Can bus data
	CAN.readMsgBuf(&lenspeed, bufspeed);
	canId = CAN.getCanId();
	//Print data to the serial console

	Serial.println("CAN_BUS GET DATA!");
	Serial.print("CAN ID:");
	Serial.println(canId);
	Serial.print("data len="); Serial.println(lenspeed);
	//This loops through each byte of data and prints it
	for (int i = 0; i < lenspeed; i++)
	{
		Serial.print(bufspeed[i], HEX); Serial.print("\t");
	}
	Serial.println();

	int bytespeed3 = bufspeed[3];
	int bytespeed2 = bufspeed[4];
	int bytespeed1 = bufspeed[5];
	int bytespeed0 = bufspeed[6];

	signed long allspeed = (signed long)bytespeed3 << 24 | (unsigned long)bytespeed2 << 16 | (unsigned long)bytespeed1 << 8 | (unsigned long)bytespeed0;
	//Serial.println(allspeed, HEX);
	Serial.print("\nSpeed:  ");
	Serial.println(allspeed, DEC);

	delay(500);

}

void loop() {

	readCANbusPos();
	readCANbusSpeed();
	delay(1000);
}