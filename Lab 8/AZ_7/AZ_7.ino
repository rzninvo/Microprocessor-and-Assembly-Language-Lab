#include <MenuBackend.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>
#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>

#define DEVICE_ADDRESS 0b1010000

char keys[4][3] ={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[4] = { 22, 23, 24, 25 };
byte colPins[3] = { 26, 27, 28};

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, 4, 3); //  Create the Keypad

const int buttonPinRst = 3;
const int buttonPinClr = 2;       // pin for the Clear button
const int buttonPinLeft = 4;      // pin for the Up button
const int buttonPinRight = 5;    // pin for the Down button
const int buttonPinEsc = 6;     // pin for the Esc button
const int buttonPinEnter = 7;   // pin for the Enter button

int lastButtonPushed = 0;

Bounce rst = Bounce();
Bounce clr = Bounce();
Bounce esc = Bounce ();
Bounce enter = Bounce ();
Bounce left = Bounce ();
Bounce right = Bounce ();

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8; 
LiquidCrystal lcd(rs, 14, en, d4, d5, d6, d7);

void menuChange (MenuChangeEvent changed)
  {
  lcd.clear ();
  MenuItem newMenuItem = changed.to;

  if ( newMenuItem.getName() == "Start" ){
    lcd.print ("Start");
    }
  else if ( newMenuItem.getName() == "Set Time" ){
    lcd.print ("Set Time");
    }
  else if ( newMenuItem.getName() == "Pre Wash" ){
    lcd.print ("Pre Wash");
    }
  else if ( newMenuItem.getName () == "Tide Wash" ){
    lcd.print ("Tide Wash");
    }
  else if ( newMenuItem.getName () == "Water Wash" ){
    lcd.print ("Water Wash");
    }
  else if ( newMenuItem.getName () == "Drying" ){
    lcd.print("Drying");
    }
  else{
    lcd.print ("Main Menu");
    }
  }

int itemID;
int _time[4];
unsigned long cur_time, timer;

void menuUse (MenuUseEvent used)
  {
  String itemName = used.item.getName();
  if (itemName == "Pre Wash")
    itemID = 0;
  else if (itemName == "Tide Wash")
    itemID = 1;
  else if (itemName == "Water Wash")
    itemID = 2;
  else if (itemName == "Drying")
    itemID = 3;
  else if (itemName == "Set Time")
    {
      int mdnum = 0;
      int num = 0;
      lcd.clear();
      while (true)
      {
        enter.update();
        clr.update();
        esc.update();
        if (clr.fell())
        {
          num = 0;
          lcd.clear();
        }
        char key = kpd.getKey();
        if (key != NO_KEY)
        {
           if ( key >= '0' && key <= '9')
                num = (num * 10) + (key - '0');
        }
        lcd.setCursor(0,0);
        lcd.print(num);
        if (enter.fell())
        {
          if (mdnum == 0)
          {
            if (num >= 1 && num <= 4)
              {
                mdnum = num;
                num = 0;
              }
            else
              {
                num = 0;
              }
            lcd.clear();
          }
          else
          {
            _time[mdnum - 1] = num;
            lcd.clear();
          }
        }
        if (esc.fell())
        {
          Wire.beginTransmission(DEVICE_ADDRESS);
          Wire.write(0);
          Wire.write(0);
          Wire.write(5);
          for (int i = 0 ; i < 4; i++)
          { 
            Wire.write(_time[i]);
          }
          Wire.endTransmission();
          lcd.clear();
          lcd.print (used.item.getName ());
          break;
        }
      }
    }
 }

MenuBackend menu = MenuBackend (menuUse, menuChange);
  MenuItem start = MenuItem ("Start");
  MenuItem set_time = MenuItem ("Set Time");
  MenuItem pre_wash = MenuItem ("Pre Wash");
  MenuItem  tide_wash = MenuItem ("Tide Wash");
  MenuItem water_wash = MenuItem ("Water Wash");
  MenuItem drying = MenuItem ("Drying");

void setup ()
  {
    itemID = -1;
    for(int i = 0 ; i < 4; i++)
      _time[i] = 20;
    Wire.begin();
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(0);
    Wire.write(0);
    Wire.endTransmission();
    Wire.requestFrom(DEVICE_ADDRESS, 5);
    if (Wire.available())
    {
      int ID = Wire.read();
      if (ID < 4 && ID >= 0)
      {
        itemID = ID;
        for (int i = 0 ; i < 4; i++)
        {
           _time[i] = Wire.read(); 
        }
      }
    }

    lcd.begin(20, 4);
    lcd.setCursor(0, 0);

    pinMode (buttonPinRst, INPUT_PULLUP);
    pinMode (buttonPinClr, INPUT_PULLUP);
    pinMode (buttonPinEsc, INPUT_PULLUP);
    pinMode (buttonPinEnter, INPUT_PULLUP);
    pinMode (buttonPinLeft, INPUT_PULLUP);
    pinMode (buttonPinRight, INPUT_PULLUP);
    pinMode (41, INPUT);
    pinMode (42, INPUT);
    pinMode (43, INPUT);
    pinMode (44, INPUT);

    digitalWrite(41, LOW);
    digitalWrite(42, LOW);
    digitalWrite(43, LOW);
    digitalWrite(44, LOW);

    rst.attach (buttonPinRst);
    clr.attach (buttonPinClr);
    esc.attach (buttonPinEsc);
    enter.attach (buttonPinEnter);
    right.attach (buttonPinRight);
    left.attach (buttonPinLeft);
    rst.interval (50);
    clr.interval (50);
    esc.interval (50);
    enter.interval (50);
    right.interval (50);
    left.interval (50);
     
    menu.getRoot().add(start).addRight(set_time);
    start.add (pre_wash).addRight(tide_wash).addRight(water_wash).addRight(drying);
    menu.toRoot();
  }

void loop ()
  {
    int state = 0;
    int escape = 0;
    if (itemID == -1)
    {
      readPressedButton();
      navigateMenus();
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      if (itemID == 0)
      {
        lcd.print("Pre Washing...");
        digitalWrite(41, HIGH);
        digitalWrite(42, LOW);
        digitalWrite(43, LOW);
        digitalWrite(44, LOW);
      }
      else if (itemID == 1)
      {
        lcd.print("Tide Washing...");
        digitalWrite(41, LOW);
        digitalWrite(42, HIGH);
        digitalWrite(43, LOW);
        digitalWrite(44, LOW);
      }
      else if (itemID == 2)
      {
        lcd.print("Water Washing...");
        digitalWrite(41, LOW);
        digitalWrite(42, LOW);
        digitalWrite(43, HIGH);
        digitalWrite(44, LOW);
      }
      else if (itemID == 3)
      {
        lcd.print("Drying");
        digitalWrite(41, LOW);
        digitalWrite(42, LOW);
        digitalWrite(43, LOW);
        digitalWrite(44, HIGH);
      }
      Wire.beginTransmission(DEVICE_ADDRESS);
          Wire.write(0);
          Wire.write(0);
          Wire.write(itemID);
          for (int i = 0 ; i < 4; i++)
          { 
            Wire.write(_time[i]);
          }
      Wire.endTransmission();
      escape = 0;
      cur_time = millis();
      timer = _time[itemID];
      state = 0;
      while (timer > 0 && (escape == 0))
      { 
        enter.update();
        esc.update();
        if (enter.fell())
        {
          if (state == 0)
            state = 1;
          else
            state = 0;
        }
        if (esc.fell())
        {
          escape = 1;
          itemID = -1;
          lcd.clear();
          menu.toRoot();
          Wire.beginTransmission(DEVICE_ADDRESS);
          Wire.write(0);
          Wire.write(0);
          Wire.write(5);
          for (int i = 0 ; i < 4; i++)
          {
            Wire.write(_time[i]);
          }
          Wire.endTransmission();
          digitalWrite(41, LOW);
          digitalWrite(42, LOW);
          digitalWrite(43, LOW);
          digitalWrite(44, LOW);
          break;
        }
        if (((millis() - cur_time) / 1000) == 1)
        { 
          timer = timer - 1;
          cur_time = millis();
          Wire.beginTransmission(DEVICE_ADDRESS);
          Wire.write(0);
          Wire.write(itemID + 1);
          Wire.write(timer);
          Wire.endTransmission();
        }
        if (state == 0)
        {
          lcd.setCursor(0, 1);
          lcd.print(timer);
        }
        else
        {
          cur_time = millis();
        }
      }
      if (timer == 0)
      {
        if (itemID == 3)
        {
          lcd.clear();
          lcd.print("---DONE---");
          digitalWrite(41, HIGH);
          digitalWrite(42, HIGH);
          digitalWrite(43, HIGH);
          digitalWrite(44, HIGH);
          while (true)
          {
            esc.update();
            enter.update();
            right.update();
            left.update();
            if (esc.fell() || enter.fell() || right.fell() || left.fell())
              break;
          }
          lcd.clear();
          menu.toRoot();
          itemID = -1;
          Wire.beginTransmission(DEVICE_ADDRESS);
          Wire.write(0);
          Wire.write(0);
          Wire.write(5);
          for (int i = 0 ; i < 4; i++)
          { 
            Wire.write(20);
          }
          Wire.endTransmission();
          digitalWrite(41, LOW);
          digitalWrite(42, LOW);
          digitalWrite(43, LOW);
          digitalWrite(44, LOW);
        }
        else
          itemID++;
      }
    }
  }
  
void navigateMenus ()
  {
  MenuItem currentMenu = menu.getCurrent ();
 
  switch ( lastButtonPushed ){
      case buttonPinEnter:
        if ( !(currentMenu.moveDown ()) ){
          menu.use();
          }
        else{
        menu.moveDown ();
          }
        break;
      case buttonPinEsc:
        if (currentMenu.moveUp())
           menu.moveUp();
        else
           menu.toRoot();
        break;
      case buttonPinRight:
        menu.moveRight();
        break;
      case buttonPinLeft:
        menu.moveLeft();
        break;
      case buttonPinRst:
        lcd.clear();
          menu.toRoot();
          itemID = -1;
          Wire.beginTransmission(DEVICE_ADDRESS);
          Wire.write(0);
          Wire.write(0);
          Wire.write(5);
          for (int i = 0 ; i < 4; i++)
          {
            Wire.write(20);
          }
          Wire.endTransmission();
          for (int i = 0 ; i < 4; i++)
            _time[i] = 20;
          digitalWrite(41, LOW);
          digitalWrite(42, LOW);
          digitalWrite(43, LOW);
          digitalWrite(44, LOW);
        break;
    }
  lastButtonPushed = 0;
}

void readPressedButton ()
  {
  rst.update ();
  enter.update ();
  right.update ();
  left.update ();
  esc.update ();
 
  if ( enter.fell () )
    {
    lastButtonPushed = buttonPinEnter;
 
    }
  else if ( esc.fell () )
    {
    lastButtonPushed = buttonPinEsc;
    }
  else if ( right.fell () )
    {
    lastButtonPushed = buttonPinRight;
    }
  else if ( left.fell () )
    {
    lastButtonPushed = buttonPinLeft;
    }
  else if ( rst.fell () )
    {
    lastButtonPushed = buttonPinRst;
    }  
  else
    {
    lastButtonPushed = 0;
    }
  }
