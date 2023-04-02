void setup() {
  // put your setup code here, to run once:
  pinMode (52, OUTPUT);
  pinMode (7, INPUT);
  pinMode (8, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int button1 = digitalRead(8);
  int button2 = digitalRead(7);
   if(button1 == HIGH)
      digitalWrite(52,HIGH);
   if(button2 == HIGH)
      digitalWrite(52,LOW);
}
