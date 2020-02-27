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
#include <ArduinoJson.h>


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

  if (!manager.init()) {
    Serial.println("init failed");
  } else {
    Serial.println("init success");
  }

  rf95.setTxPower(23, false);
  rf95.setFrequency(915.0);
  rf95.setCADTimeout(500);
  rf95.setThisAddress(SERVER_ADDRESS);

}

// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
void loop()
{
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from))
  {
//    Serial.print("got message from :");
//    Serial.println(buf[0]);
//    Serial.print("Tempurature: ");
//    Serial.println(buf[1]);
//    Serial.print("Humidity: ");
//    Serial.println(buf[2]);
//    Serial.print("Light intensity: ");
//    Serial.println(buf[3]);

    jsonifyReceivedData(buf);
  }
}

void jsonifyReceivedData(uint8_t * buf) {
  StaticJsonDocument<200> doc;
  doc["id"] = (char*)buf[0];
  doc["temp"] = (char*)buf[1];
  doc["humi"] = (char*)buf[2];
  doc["light"] = (char*)buf[3];

  char payload[500];

  for (int i = 0; i < 500; i++) {
    payload[i] = '\0';
  }

  serializeJson(doc, payload);

  Serial.print(payload);

}
