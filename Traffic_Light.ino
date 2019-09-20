
#include <ESP8266WiFi.h>

int redLED = D6;
int yellowLED = D7;
int greenLED = D8;

void greenLight() {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, HIGH);
}
void yellowLight() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, HIGH);
}

void redLight() {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, HIGH);
}

void offLight() {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Serial Output Beginning.");
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  Serial.println("Done Initializing LEDs.");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Red");
  redLight();
  delay(5000);
  Serial.println("Green");
  greenLight();
  delay(5000);
  Serial.println("Yellow");
  yellowLight();
  delay(5000);
}
