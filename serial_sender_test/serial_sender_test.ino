void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  String str = "Sending a message";
  Serial1.println(str);
  delay(1000);
}
