#include <LiquidCrystal.h> 
#include <Keypad.h> 
#include <Servo.h>

// Define the Keymap
char keys[4][4] ={
  {'7','8','9','D'},
  {'4','5','6','C'},
  {'1','2','3','B'},
  {'*','0','#','A'}
};

byte rowPins[4] = { 22, 23, 24, 25 };
byte colPins[4] = { 26, 27, 28, 29 }; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 ); //  Create the Keypad
 
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myservo;
 
 long Num1,Num2,Number;
 char key,action;
 boolean result = false;

void setup() { 
  lcd.begin(16, 2); 
  lcd.setCursor(0, 1);
  myservo.attach(7, 1000, 2000);
}

void loop() {
  
key = kpd.getKey(); //storing pressed key value in a char
 
if (key!=NO_KEY)
  DetectButtons();
 
if (result==true)
  CalculateResult();
 
DisplayResult();   

}

void DetectButtons()
{ 
    lcd.clear(); 

    if(key >= '0' && key <= '9') 
    {
      Serial.print ("Button ");
      Serial.println (key - '0'); 

      Number = (Number*10) + (key - '0');
    }

     
    if (key=='*') //If cancel Button is pressed
    {
      Serial.println ("Button Cancel"); 
      Number=Num1=Num2=0; 
      myservo.write(Number);
      action = 0x00;
      result=false;
    }

 
    if (key == '#')
    {
      Serial.println ("Button Equal"); 
      Num2=Number;
      result = true;
    }
    
    
    
    if (key == 'A' || key == 'B' || key == 'C' || key == 'D') //Detecting Buttons on Column 4
  {
    Num1 = Number;
    Num2 = 0 ;    
    Number =0;
    result = false;
    
    if (key == 'A')
    {Serial.println ("Addition"); action = '+';}
     if (key == 'B')
    {Serial.println ("Subtraction"); action = '-'; }
     if (key == 'C')
    {Serial.println ("Multiplication"); action = '*';}
     if (key == 'D')
    {Serial.println ("Devesion"); action = '/';}  
 
    delay(100);
  }
  
}
 
void CalculateResult()
{
  if (action=='+')
    Number = Num1+Num2;
 
  if (action=='-')
    Number = Num1-Num2;
 
  if (action=='*')
    Number = Num1*Num2;
 
  if (action=='/')
    Number = Num1/Num2; 
  myservo.write(Number);
}
 
void DisplayResult()
{
  lcd.setCursor(0, 0);   // set the cursor to column 0, line 0
  lcd.print(Num1); lcd.print(action); lcd.print(Num2); 
  
  if (result==true)
  {lcd.print(" ="); lcd.print(Number);} //Display the result
  
  lcd.setCursor(0, 1);   // set the cursor to column 0, line 1
  lcd.print(Number); //Display the result
}
