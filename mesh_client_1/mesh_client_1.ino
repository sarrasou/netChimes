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

void sendData() {
  uint8_t client_id = CLIENT_ADDRESS;
  uint8_t temp = getTemp();
  uint8_t humidity = getHumidity();
  uint8_t light = getLightIntensity();

  uint8_t data [] = {client_id, temp, humidity, light};

  Serial.println("Button pressed");
  Serial.println("Sending to SERVER_ADDRESS");

  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS) == RH_ROUTER_ERROR_NONE) {
    Serial.println("Data sent");
  } else {
    Serial.println("sendtoWait failed. Are the intermediate mesh servers running?");
  }

}

// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
void loop()
{
  val = digitalRead(inPin);  // read input value
  if (val == HIGH && state == true) {
    sendData();
    state = false;
  }

  if (val == LOW) {
    state = true;
  }

}

int getTemp() {
  /*
    int reading = analogRead(tempuraturePin);
    float voltage = reading * 3.3;
    voltage /= 1024.0;
    int temperatureC = (voltage - 0.5) * 100 ;
    return temperatureC;
  */
  return 26;
}

int getHumidity() {
  //reads humidity sensor and returns humidity
  return 7;
}

int getLightIntensity() {
  // reads light sensor and returns intensity
  return 13;
}
