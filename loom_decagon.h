// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// Will be update to use state struct soon

// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include "SDI12.h"
#include <string.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define DATAPIN1 11         // change to the proper pin
#define DATAPIN2 A5
#define SENSOR_ADDRESS "?" //"?" broadcasts message


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
SDI12 decagon1(DATAPIN1);
SDI12 decagon2(DATAPIN2); //clare added this 

String sdiResponse1 = "";
String myCommand1 = "";
char   buf1[20];

String sdiResponse2 = "";
String myCommand2 = "";
char   buf2[20];
char   *p1;

char   *p2;

float  dielec_p1 = 0, temp1 = 0, elec_c1 = 0;
float  dielec_p2 = 0, temp2 = 0, elec_c2 = 0;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_decagon();
void measure_decagon();
void package_decagon(OSCBundle * bndl, char packet_header_string[]);


// ================================================================
// ===                          SETUP                           ===
// ================================================================
//
// Runs any Decagon setup
//
void setup_decagon() 
{
  // Setup decagon 1
	decagon1.begin();
	delay(2000);
	//first command to take a measurement
	myCommand1 = String(SENSOR_ADDRESS) + "I!";
	LOOM_DEBUG_Println2("Decagon1: ", myCommand1);     // echo command to terminal

	decagon1.sendCommand(myCommand1);
	delay(30);                     // wait a while for a response

	while (decagon1.available()) {  // build response string
		char c = decagon1.read();
		if ((c != '\n') && (c != '\r')) {
			sdiResponse1 += c;
			delay(5);
		}
	}
	LOOM_DEBUG_Println(sdiResponse1); //write the response to the screen
	decagon1.clearBuffer();

	delay(1000);                 // delay between taking reading and requesting data
	sdiResponse1 = "";            // clear the response string


 // Setup decagon 2
  decagon2.begin();
  delay(2000);
  //first command to take a measurement
  myCommand2 = String(SENSOR_ADDRESS) + "I!";
  LOOM_DEBUG_Println2("Decagon2: ", myCommand2);     // echo command to terminal

  decagon2.sendCommand(myCommand2);
  delay(30);                     // wait a while for a response

  while (decagon2.available()) {  // build response string
    char c = decagon2.read();
    if ((c != '\n') && (c != '\r')) {
      sdiResponse2 += c;
      delay(5);
    }
  }
  LOOM_DEBUG_Println(sdiResponse2); //write the response to the screen
  decagon2.clearBuffer();

  delay(1000);                 // delay between taking reading and requesting data
  sdiResponse2 = "";            // clear the response string
}



// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- MEASURE DECAGON ---
//
// Gets Decagon sensor readings
//
void measure_decagon() 
{
  // Measure decagon 1
  
	// First command to take a measurement
	myCommand1 = String(SENSOR_ADDRESS) + "M!";
	//Serial.println(myCommand);     // echo command to terminal
	
	decagon1.sendCommand(myCommand1);
	delay(30);                     // wait a while for a response
	
	while (decagon1.available()) {  // build response string
		char c = decagon1.read();
		if ((c != '\n') && (c != '\r')) {
			sdiResponse1 += c;
			delay(5);
		}
	}
	//if (sdiResponse.length() > 1) Serial.println(sdiResponse); //write the response to the screen
	decagon1.clearBuffer();
	
	delay(1000);                 // delay between taking reading and requesting data
	sdiResponse1 = "";            // clear the response string
	
	// next command to request data from last measurement
	myCommand1 = String(SENSOR_ADDRESS) + "D0!";
	Serial.println(myCommand1);  // echo command to terminal
	
	decagon1.sendCommand(myCommand1);
	delay(30);                     // wait a while for a response
	
	while (decagon1.available()) {  // build string from response
		char c = decagon1.read();
		if ((c != '\n') && (c != '\r')) {
			sdiResponse1 += c;
			delay(5);
		}
	}
	
	sdiResponse1.toCharArray(buf1, sizeof(buf1));
	
	p1 = buf1;
	
	strtok_r(p1, "+", &p1);
	dielec_p1 = atof(strtok_r(NULL, "+", &p1));
	temp1     = atof(strtok_r(NULL, "+", &p1));
	elec_c1   = atof(strtok_r(NULL, "+", &p1));
	
	
	//if (sdiResponse1.length() > 1) Serial.println(sdiResponse1); //write the response to the screen
	Serial.print("1 Dielectric Permittivity: ");
	Serial.println(dielec_p1);
	Serial.print("1 Temperature (C): ");
	Serial.println(temp1);
	Serial.print("1 Electric Conductivity: ");
	Serial.println(elec_c1);
	decagon1.clearBuffer();



   // Measure decagon 2
  
 // First command to take a measurement
  myCommand2 = String(SENSOR_ADDRESS) + "M!";
  Serial.println(myCommand2);     // echo command to terminal
  
  decagon2.sendCommand(myCommand2);
  delay(30);                     // wait a while for a response
  
  while (decagon2.available()) {  // build response string
    char c = decagon2.read();
    if ((c != '\n') && (c != '\r')) {
      sdiResponse2 += c;
      delay(5);
    }
  }
  //if (sdiResponse.length() > 1) Serial.println(sdiResponse); //write the response to the screen
  decagon2.clearBuffer();
  
  delay(1000);                 // delay between taking reading and requesting data
  sdiResponse2 = "";            // clear the response string
  
  // next command to request data from last measurement
  myCommand2 = String(SENSOR_ADDRESS) + "D0!";
  Serial.println(myCommand2);  // echo command to terminal
  
  decagon2.sendCommand(myCommand2);
  delay(30);                     // wait a while for a response
  
  while (decagon2.available()) {  // build string from response
    char c = decagon2.read();
    if ((c != '\n') && (c != '\r')) {
      sdiResponse2 += c;
      delay(5);
    }
  }
  
  sdiResponse2.toCharArray(buf2, sizeof(buf2));
  
  p2 = buf2;
  
  strtok_r(p2, "+", &p2);
  dielec_p2 = atof(strtok_r(NULL, "+", &p2));
  temp2     = atof(strtok_r(NULL, "+", &p2));
  elec_c2   = atof(strtok_r(NULL, "+", &p2));
  
  
  //if (sdiResponse2.length() > 1) Serial.println(sdiResponse2); //write the response to the screen
  Serial.print("2 Dielectric Permittivity: ");
  Serial.println(dielec_p2);
  Serial.print("2 Temperature (C): ");
  Serial.println(temp2);
  Serial.print("2 Electric Conductivity: ");
  Serial.println(elec_c2);
  decagon2.clearBuffer();
}



// --- PACKAGE DECAGON ---
//
// Populates OSC bundle with last Decagon readings
//
// @param bndl                  The OSC bundle to be populated
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//
void package_decagon(OSCBundle * bndl, char packet_header_string[]) 
{
	char addressString[255];
	sprintf(addressString, "%s%s", packet_header_string, "/DielecPerm1");
	bndl->add(addressString).add(dielec_p1);
	sprintf(addressString, "%s%s", packet_header_string, "/Temp1");
	bndl->add(addressString).add(temp1);
	sprintf(addressString, "%s%s", packet_header_string, "/ElecCond1");
	bndl->add(addressString).add(elec_c1);


  sprintf(addressString, "%s%s", packet_header_string, "/DielecPerm2");
  bndl->add(addressString).add(dielec_p2);
  sprintf(addressString, "%s%s", packet_header_string, "/Temp2");
  bndl->add(addressString).add(temp2);
  sprintf(addressString, "%s%s", packet_header_string, "/ElecCond2");
  bndl->add(addressString).add(elec_c2);
}
