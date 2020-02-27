#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>


#define RH_HAVE_SERIAL
#define N_NODES 3
#define RFM95_CS 8
#define RFM95_INT 3

//nodes
#define SERVER_ADDRESS 1
#define CLIENT_ADDRESS 2 // 2-however many clients

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(rf95, CLIENT_ADDRESS);

int inPin = 5;   // choose the input pin (for a pushbutton)
int val = 0;     // variable for reading the pin status
boolean state = true;

void setup()
{
  pinMode(inPin, INPUT);
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
  rf95.setThisAddress(CLIENT_ADDRESS);

}


uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
void loop()
{
  val = digitalRead(inPin);  // read input value
  if (val == HIGH && state == true) {
    Serial.println("Button pressed");
    Serial.println("Sending to SERVER_ADDRESS");

    if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS) == RH_ROUTER_ERROR_NONE) {
      Serial.println("Data sent");
    } else {
      Serial.println("sendtoWait failed. Are the intermediate mesh servers running?");
    }
    state = false;

  }

    if (val == LOW) {
    state = true;
  }

}
