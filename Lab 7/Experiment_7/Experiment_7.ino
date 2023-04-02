#include <MenuBackend.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>
#include <Keypad.h>
#include <Servo.h>

Servo servo[12];

class VendingItem{
  public:
    int price;
    int count;
};

const int buttonPinClr = 2;       // pin for the Clear button
const int buttonPinMode = 3;      // pin for the Mode button
const int buttonPinLeft = 4;      // pin for the Up button
const int buttonPinRight = 5;    // pin for the Down button
const int buttonPinEsc = 6;     // pin for the Esc button
const int buttonPinEnter = 7;   // pin for the Enter button

int lastButtonPushed = 0;

String mode = "User";

Bounce clr = Bounce();
Bounce mod = Bounce ();
Bounce esc = Bounce ();
Bounce enter = Bounce ();
Bounce left = Bounce ();
Bounce right = Bounce ();

char keys[4][3] ={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

VendingItem vendingItems[12];
byte rowPins[4] = { 22, 23, 24, 25 };
byte colPins[3] = { 26, 27, 28}; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, 4, 3); //  Create the Keypad

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8; 
LiquidCrystal lcd(rs, 14, en, d4, d5, d6, d7);

void rotateMotor(int servoID, int count, boolean reverse)
{
  for (int i = 0 ; i < count; i++)
  {
    delay(1000);
    digitalWrite(39,HIGH);
    servo[servoID].write((reverse == true)?0:180);
    delay(1000);
    digitalWrite(39,LOW);
    servo[servoID].write((reverse == true)?180:0);
  }
}

void menuChange (MenuChangeEvent changed)
  {
  lcd.clear ();
  MenuItem newMenuItem = changed.to;

  if ( newMenuItem.getName() == "Drinks" ){
    lcd.print ("Drinks       ");
    }
  else if ( newMenuItem.getName () == "Hot Drinks" ){
    lcd.print ("Hot Drinks       ");
    }
  else if ( newMenuItem.getName () == "Tea" ){
    lcd.print ("Tea");
    }
  else if ( newMenuItem.getName () == "Coffee" ){
    lcd.print("Coffee");
    }
  else if ( newMenuItem.getName () == "Nescafe" ){
    lcd.print ("Nescafe");
    }
  else if ( newMenuItem.getName () == "Cold Drinks" ){
    lcd.print ("Cold Drinks       ");
    }
  else if ( newMenuItem.getName () == "Orange Juice" ){
    lcd.print ("Orange Juice");
    }
  else if ( newMenuItem.getName () == "Apple Juice" ){
    lcd.print ("Apple Juice");
    }
  else if ( newMenuItem.getName () == "Cherry Juice" ){
    lcd.print ("Cherry Juice");
    }
  else if ( newMenuItem.getName () == "Sandwiches" ){
    lcd.print ("Sandwiches       ");
    }
  else if ( newMenuItem.getName () == "Hot Sandwiches" ){
    lcd.print ("Hot Sandwiches       ");
    }
  else if ( newMenuItem.getName () == "Hotdog" ){
    lcd.print ("Hotdog");
    }
  else if ( newMenuItem.getName () == "Hamburger" ){
    lcd.print ("Hamburger");
    }
  else if ( newMenuItem.getName () == "Cheeseburger" ){
    lcd.print ("Cheeseburger");
    }
  else if ( newMenuItem.getName () == "Cold Sandwiches" ){
    lcd.print ("Cold Sandwiches       ");
    }
  else if ( newMenuItem.getName () == "Meat Ham" ){
    lcd.print ("Meat Ham");
    }
  else if ( newMenuItem.getName () == "Chicken Ham" ){
    lcd.print ("Chicken Ham");
    }
  else if ( newMenuItem.getName () == "Olivia Salad" ){
    lcd.print ("Olivia Salad");
    }
  else{
    lcd.print ("Main Menu       ");
    }
  }

void menuUse (MenuUseEvent used)
  {
    int itemID = 0;
  lcd.clear ();
  lcd.print (used.item.getName ());
  String itemName = used.item.getName();
  if (itemName == "Tea")
    itemID = 3;
  else if (itemName == "Coffee")
    itemID = 4;
  else if (itemName == "Nescafe")
    itemID = 5;
  else if (itemName == "Orange Juice")
    itemID = 0;
  else if (itemName == "Apple Juice")
    itemID = 1;
  else if (itemName == "Cherry Juice")
    itemID = 2;
  else if (itemName == "Hotdog")
    itemID = 6;
  else if (itemName == "Hamburger")
    itemID = 7;
  else if (itemName == "Cheeseburger")
    itemID = 8;
  else if (itemName == "Meat Ham")
    itemID = 9;
  else if (itemName == "Chicken Ham")
    itemID = 10;
  else if (itemName == "Olivia Salad")
    itemID = 11;
    int stat = 1;
    while (true)
    {
      esc.update();
      if (stat == 0)
      {
        lcd.clear();
        lcd.print (used.item.getName ());
        break;
      }
      lcd.clear();
      lcd.print (used.item.getName ());
      lcd.setCursor(0,1);
      lcd.print ("Count: ");
      lcd.print(vendingItems[itemID].count);
      lcd.setCursor(0,2);
      lcd.print("Price: ");
      lcd.print(vendingItems[itemID].price);
      lcd.setCursor(0, 3);
      int num = 0;
      int modeInc = 0;
      while (true)
      {
        lcd.setCursor(0, 3);
        enter.update();
        clr.update();
        esc.update();
        if (clr.fell())
        {
          num = 0;
          modeInc = 0;
          lcd.clear();
          lcd.print (used.item.getName ());
          lcd.setCursor(0, 1);
          lcd.print ("Count: ");
          lcd.print(vendingItems[itemID].count);
          lcd.setCursor(0, 2);
          lcd.print("Price: ");
          lcd.print(vendingItems[itemID].price);
          lcd.setCursor(0, 3);
        }
        char key = kpd.getKey();
        if (key != NO_KEY)
        {
          if (mode == "User")
          {
            if ( key >= '0' && key <= '9' )
              num = num * 10 + (key - '0');
          }
          else
          {
            if ( key >= '0' && key <= '9' && modeInc == 0)
              num = num * 10 + (key - '0');
            else if ( key == '#' )
              modeInc = 1;
            else if ( key == '*' )
              modeInc = 2;
          }
          lcd.print(num);
          if (modeInc == 1)
            lcd.print("#");
          if (modeInc == 2)
            lcd.print("*");
        }
        if (enter.fell())
        {
          stat = 1;
          break;
        }
        if (esc.fell())
        {
          stat = 0;
          break;
        }
      }
      if (mode == "User" && stat == 1)
      {
        if (num <= vendingItems[itemID].count)
        {
          vendingItems[itemID].count -= num;
          rotateMotor(itemID, num, false);
        }
        else
          {
            lcd.clear();
            lcd.print("Not enough items!");
            delay(2000);
            lcd.clear();
            lcd.print (used.item.getName ());
            lcd.setCursor(0, 1);
            lcd.print ("Count: ");
            lcd.print(vendingItems[itemID].count);
            lcd.setCursor(0, 2);
            lcd.print("Price: ");
            lcd.print(vendingItems[itemID].price);
            lcd.setCursor(0, 3);
            num = 0;
          }
      }
      else if (stat == 1)
      {
        if (modeInc == 1)
        {
          vendingItems[itemID].count += num;  
          rotateMotor(itemID, num, true);
        }
        else if (modeInc == 2)
          vendingItems[itemID].price = num;
        else if(modeInc == 0)
          {
            lcd.clear();
            lcd.print("Incorrect Command");
            delay(2000);
            lcd.clear();
          }
      }
    }
  }

MenuBackend menu = MenuBackend (menuUse, menuChange);
  MenuItem drinks = MenuItem ("Drinks");
    MenuItem  hot_drinks = MenuItem ("Hot Drinks");
      MenuItem hot_drinks_item1 = MenuItem ("Tea");
      MenuItem hot_drinks_item2 = MenuItem ("Coffee");
      MenuItem hot_drinks_item3 = MenuItem ("Nescafe");
    MenuItem cold_drinks = MenuItem ("Cold Drinks");
      MenuItem cold_drinks_item1 = MenuItem ("Orange Juice");
      MenuItem cold_drinks_item2 = MenuItem ("Apple Juice");
      MenuItem cold_drinks_item3 = MenuItem ("Cherry Juice");
  MenuItem sandwiches = MenuItem ("Sandwiches");
    MenuItem hot_sandwiches = MenuItem ("Hot Sandwiches");
      MenuItem hot_sandwiches_item1 = MenuItem ("Hotdog");
      MenuItem hot_sandwiches_item2 = MenuItem ("Hamburger");
      MenuItem hot_sandwiches_item3 = MenuItem ("Cheeseburger");
    MenuItem cold_sandwiches = MenuItem ("Cold Sandwiches");
      MenuItem cold_sandwiches_item1 = MenuItem ("Meat Ham");
      MenuItem cold_sandwiches_item2 = MenuItem ("Chicken Ham");
      MenuItem cold_sandwiches_item3 = MenuItem ("Olivia Salad"); 

void setup ()
  {
    lcd.begin(20, 4); 
    lcd.setCursor(0, 0);

    for (int i = 0; i < 12; i++)
    {
      servo[i].attach(40+i,1000,2000);
      servo[i].write(0);
    }

    pinMode (buttonPinMode, INPUT_PULLUP);
    pinMode (buttonPinEsc, INPUT_PULLUP);
    pinMode (buttonPinEnter, INPUT_PULLUP);
    pinMode (buttonPinLeft, INPUT_PULLUP);
    pinMode (buttonPinRight, INPUT_PULLUP);
    pinMode(21, OUTPUT);
    digitalWrite(21,HIGH);
    pinMode(39, OUTPUT);
    digitalWrite(39,LOW);

    clr.attach (buttonPinMode);
    mod.attach (buttonPinMode);
    esc.attach (buttonPinEsc);
    enter.attach (buttonPinEnter);
    right.attach (buttonPinRight);
    left.attach (buttonPinLeft);
    clr.interval (50);
    mod.interval (50);
    esc.interval (50);
    enter.interval (50);
    right.interval (50);
    left.interval (50);
    for (int i = 0 ; i < 12; i++)
    {
      vendingItems[i].price = 0;
      vendingItems[i].count = 0;
    }
     
    menu.getRoot().add (drinks);
    drinks.addRight(sandwiches);
    drinks.add(cold_drinks).addRight(hot_drinks);
      cold_drinks.add(cold_drinks_item1).addRight(cold_drinks_item2).addRight(cold_drinks_item3);
      hot_drinks.add(hot_drinks_item1).addRight(hot_drinks_item2).addRight(hot_drinks_item3);
    sandwiches.add(cold_sandwiches).addRight(hot_sandwiches);
      cold_sandwiches.add(cold_sandwiches_item1).addRight(cold_sandwiches_item2).addRight(cold_sandwiches_item3);
      hot_sandwiches.add(hot_sandwiches_item1).addRight(hot_sandwiches_item2).addRight(hot_sandwiches_item3);
    menu.toRoot();
  }

void loop ()
  {
    readPressedButton();
    navigateMenus();
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
      case buttonPinMode:
        if (mode == "User")
        {
          mode = "Admin";
          digitalWrite(21,LOW);
          for (int i = 0; i < 12; i++)
          {
            //servo[i].attach(40+i,1000,2000);
            servo[i].write(180);
          }
        }
        else
        {
          mode = "User";
          digitalWrite(21,HIGH);
          for (int i = 0; i < 12; i++)
          {
            //servo[i].attach(40+i,1000,2000);
            servo[i].write(0);
          }
        }
        break;
    }
  lastButtonPushed = 0;
}

void readPressedButton ()
  {
  enter.update ();
  right.update ();
  left.update ();
  esc.update ();
  mod.update();
 
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
  else if ( mod.fell () )
    {
    lastButtonPushed = buttonPinMode;
    }
  else{
    lastButtonPushed = 0;
 }
  }
