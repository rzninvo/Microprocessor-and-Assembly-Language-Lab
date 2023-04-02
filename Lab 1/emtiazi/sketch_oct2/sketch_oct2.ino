void setup() {
  // put your setup code here, to run once:
  pinMode (1,OUTPUT);
  pinMode (2,OUTPUT);
  pinMode (3,OUTPUT);
  pinMode (4,OUTPUT);
  pinMode (5,OUTPUT);
  pinMode (6, OUTPUT);
  pinMode (7, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int button1 = digitalRead(8);
  int button2 = digitalRead(9);
  int button3 = digitalRead(10);

  if (button1 == HIGH){
    digitalWrite(7,HIGH);
    delay(1000);
    digitalWrite(6,HIGH);
    delay(1000);
    digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(4,HIGH);
    delay(1000);
    digitalWrite(3,HIGH);
    delay(1000);
    digitalWrite(2,HIGH);
    delay(1000);
    digitalWrite(1,HIGH);
    }
    if (button2 == HIGH){
    digitalWrite(1,HIGH);
    delay(1000);
    digitalWrite(2,HIGH);
    delay(1000);
    digitalWrite(3,HIGH);
    delay(1000);
    digitalWrite(4,HIGH);
    delay(1000);
    digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(6,HIGH);
    delay(1000);
    digitalWrite(7,HIGH);  
      }
    if (button3 == HIGH){
    digitalWrite(1,LOW);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);    
      }
}
