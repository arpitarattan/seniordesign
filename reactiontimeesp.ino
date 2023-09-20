const int BUTTON = 2;
const int LED = 3;
#include <SoftwareSerial.h>
SoftwareSerial espserial(0,1); //RX, TX

void setup() {
  // put your setup code here, to run once:
  espserial.begin(115200);
  Serial.begin(9600);
  pinMode (BUTTON, INPUT);
  pinMode (LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  while(digitalRead(BUTTON) == 0){} //delay until pressed
  digitalWrite(LED, LOW);
  delay(1000);

  delay(random(5000));
  digitalWrite(LED, HIGH);
  float time = millis();//start counting

  while(digitalRead(BUTTON) == 0){} //delay until clicked
  digitalWrite(LED, LOW);

  //prints time
  float reactiontime = millis() - time;
  //String sendstring = String(reactiontime);
  Serial.print(reactiontime);
  espserial.print(reactiontime);
  delay(1000);
}