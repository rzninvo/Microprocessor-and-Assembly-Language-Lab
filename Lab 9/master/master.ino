#include<SPI.h>
#define slave_1 5
#define slave_2 10                

void setup (void)
{             
  pinMode(slave_1,OUTPUT);           
  pinMode(slave_2,OUTPUT); 
  digitalWrite(slave_1,HIGH);
  digitalWrite(slave_2,HIGH);
  SPI.begin();                   
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop(void)
{
  int tmp = 0;       
  tmp = analogRead(A0);
  int light_voltage = map(tmp, 0, 1023, 0, 5);
  int tmp2 = 0;
  tmp2 = analogRead(A1);
  int temperatureC = map(tmp2, 0, 1023, 0, 500); 
  int light_resistance = (5000.0 / light_voltage) - 1000; // (5 * Z2)/Vo - Z2 = Z1
  digitalWrite(slave_1, LOW); 
  delay(50);
                          
  SPI.transfer(light_resistance);
  digitalWrite(slave_1, HIGH);
  delay(50);
  
  digitalWrite(slave_2, LOW); 
  delay(50);
  
  SPI.transfer(temperatureC); 
  digitalWrite(slave_2, HIGH);
  delay(50);
}
