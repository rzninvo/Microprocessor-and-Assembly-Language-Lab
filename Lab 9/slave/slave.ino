#include <LiquidCrystal.h>
#include<SPI.h>

LiquidCrystal lcd(13,12, 8, 9, 10, 11);

volatile boolean received;
volatile byte Slavereceived;
void setup()

{
  Serial.begin(9600);
  lcd.begin(16, 2);               
  pinMode(MISO,OUTPUT);                    
  digitalWrite(MISO,LOW);
  
  
  SPCR |= _BV(SPE); 
  SPCR |= _BV(SPIE);
  received = false;

  SPI.attachInterrupt();
  
}

ISR (SPI_STC_vect)
{
  Slavereceived = SPDR; 
  received = true;
}

void loop()
{ 
  if(received)
   {
    lcd.clear();  
    lcd.print(Slavereceived);
   }
  delay(100);
}
