char mystr[10];

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.readBytes(mystr, 5); //Read the serial data and store in var
  Serial.print("String: ");
  Serial.println(mystr); //Print data on Serial Monitor
  delay(1000);
}
//
//void loop() {
//  if(Serial.available() >0) {
//    Serial.print("received");
//  } else {
//    Serial.println("not avail");
//  }
//  delay(1000);
//}
