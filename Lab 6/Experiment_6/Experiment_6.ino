#include <Servo.h>
int servos = 12;
int frontlight = 13;
int buzzer = 11;
int armitures = 9;
Servo wheel1;
Servo wheel2;
int input=0;
void setup() {
  pinMode(frontlight, OUTPUT);
  pinMode(armitures, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  wheel1.attach(servos,1000,2000);
  wheel2.attach(servos,1000,2000);
}

void loop() {
  if (Serial.available() > 0){
    input=Serial.parseInt();
    Serial.println(input);
  }
   
  
  if (input == 200){
   digitalWrite(frontlight, HIGH);
   Serial.print("\n");
   Serial.print("Frontlights ON!");
   Serial.print("\n");
   input = 300;
   }

  if (input == 201){
   digitalWrite(frontlight, LOW);
   Serial.print("\n");
   Serial.print("Frontlights OFF");
   Serial.print("\n");
   input = 300;
   }

  if (input >= 0 && input <= 180){
    wheel1.write(input);
    wheel2.write(input);
    Serial.print("\n");
    Serial.print("Wheel angle set to: ");
    Serial.print(input);
    Serial.print("\n");
    input = 300;
  }

  if (input == 203) {
    tone(buzzer, 1000);
    Serial.print("\n");
    Serial.print("Buzzer turned on");
    delay(1000);
    noTone(buzzer);
    delay(1000);
    input = 300;
  }

   if (input == 204) {
    digitalWrite(armitures, HIGH);
    Serial.print("\n");
    Serial.print("Armitures turned ON: ");
    Serial.print("\n");
    input = 300;
  }

  if (input == 205) {
    digitalWrite(armitures, LOW);
    Serial.print("\n");
    Serial.print("Armitures turned OFF: ");
    Serial.print("\n");
    input = 300;
  }
}
