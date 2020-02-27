// rf22_mesh_server1.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, routed reliable messaging server
// with the RHMesh class.
// It is designed to work with the other examples rf22_mesh_*
// Hint: you can simulate other network topologies by setting the 
// RH_TEST_NETWORK define in RHRouter.h
// Mesh has much greater memory requirements, and you may need to limit the
// max message length to prevent wierd crashes
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>


#define RH_HAVE_SERIAL
#define N_NODES 3
#define RFM95_CS 8
#define RFM95_INT 3

#define SERVER_ADDRESS 1

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(rf95, SERVER_ADDRESS);

void setup()
{
  Serial.begin(115200);
  while (!Serial) ; // Wait for serial port to be available
  
  if (!manager.init()){
    Serial.println("init failed");
  }else{
    Serial.println("init success");
  }
  
  rf95.setTxPower(23, false);
  rf95.setFrequency(915.0);
  rf95.setCADTimeout(500);
  rf95.setThisAddress(SERVER_ADDRESS);
    
}

uint8_t data[] = "And hello back to you from server1";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
void loop()
{
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from))
  {
    Serial.print("got request from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);
  }
}
