#include <LiquidCrystal.h> 
#include <Keypad.h> 

// Define the Keymap
char keys[4][4] ={
  {'7','8','9','D'},
  {'4','5','6','C'},
  {'1','2','3','B'},
  {'*','0','#','A'}
};

byte rowPins[4] = { 7, 6, 5, 4 };
byte colPins[4] = { 3, 2, 1, 0 }; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 ); //  Create the Keypad
 
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
 long Num1,Num2,Number;
 char key,action;
 boolean result = false;

void setup() { 
  lcd.begin(16, 2); 
  lcd.setCursor(0, 1);
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
