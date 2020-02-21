#include <ArduinoJson.h>

// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h>

/* for feather m0  */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0
#define LED 13
#define BOARD_ID 1



// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

int tempuraturePin = 1;
int inPin = 5;   // choose the input pin (for a pushbutton)
int val = 0;     // variable for reading the pin status
boolean state = true;

void setup()
{
  pinMode(inPin, INPUT);    // declare pushbutton as input
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }

  delay(100);

  Serial.println("Feather LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

void loop() {
  val = digitalRead(inPin);  // read input value

  if (val == HIGH && state == true) { // check if the input is HIGH (button released)
    Serial.println("Button pressed");
    Serial.println("Transmitting...");
    sendData();

    state = false;
  }
  
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len)){
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
    } else {
      Serial.println("Receive failed");
    }
  }

  if (val == LOW) {
    state = true;
  }
}

void sendData() {

  StaticJsonDocument<200> doc;
  doc["id"] = BOARD_ID;
  doc["temp"] = getTemp();

  char payload[RH_RF95_MAX_MESSAGE_LEN];

  for (int i = 0; i < RH_RF95_MAX_MESSAGE_LEN; i++) {
    payload[i] = '\0';
  }
  
  serializeJson(doc, payload);

  Serial.println("Sending...");
  Serial.println(payload);
  
  delay(50);
  rf95.send((uint8_t *)payload, strlen(payload));

  delay(10);
  rf95.waitPacketSent();

}

int getTemp() {
  /*
  int reading = analogRead(tempuraturePin);
  float voltage = reading * 3.3;
  voltage /= 1024.0;
  int temperatureC = (voltage - 0.5) * 100 ;
  return temperatureC;
  */

  return 6;
}
