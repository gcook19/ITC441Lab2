
int redLED = D6;
int yellowLED = D7;
int greenLED = D8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Serial Output Beginning.");
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  Serial.print("Done Initializing LEDs.");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Red");
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED,HIGH);
  delay(5000);
  Serial.print("Green");
  digitalWrite(redLED,LOW);
  digitalWrite(greenLED,HIGH);
  delay(5000);
  Serial.print("Yellow");
  digitalWrite(greenLED,LOW);
  digitalWrite(yellowLED,HIGH);
  delay(5000);
}
