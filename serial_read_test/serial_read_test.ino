void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if(Serial1.available() >0){
//    String s = Serial1.readStringUntil('\n');
//    Serial.println(s);
    Serial.print("received");
  }
  delay(1000);
}
