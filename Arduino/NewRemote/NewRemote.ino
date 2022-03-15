#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Adafruit_RGBLCDShield.h"

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//Variables for menu
int column = 0;
int function = 0;
int buttonValue = 0;

int buttons[] = {2, 3, 4, 7, 8, 9, 10, 11, 12, 13};

int text_length = 0;
enum states{interlock, down, leg3, leg2, up};


void setup() 
{
  lcd.begin(16, 2);
  lcd.setBacklight(YELLOW);
  lcd.setCursor(0,0);
  lcd.print("R6 Droid Control");
  delay(2000);
  lcd.clear();
  lcd.print("Actions         ");
  lcd.setBacklight(GREEN);
  
  pinMode(5, OUTPUT); // channel 5
  pinMode(6, OUTPUT); // channel 6

  for(int i = 0; i < 10; i++)
    pinMode(buttons[i], INPUT_PULLUP);
}



void loop() 
{
  buttonValue = 0;

  //read all the buttons
  for(int i = 0; i < 10; i++)
  {
    if(!digitalRead(buttons[i]))
      buttonValue = i*10+10;//assign button value 
  }
  
  //if (buttonValue!=0)
  //{
    analogWrite(6, buttonValue);//write button value PWM
    lcd.setCursor(13,0);
    lcd.print(buttonValue);
    lcd.print("   ");
//    delay(500);
//    analogWrite(6, 0);
//    lcd.setCursor(13,0);
//    lcd.print("0");
//    lcd.print("    ");
  //}
  
  uint8_t LCD_buttons = lcd.readButtons();//read LCD buttons

  if (LCD_buttons) 
  {
    if (LCD_buttons & BUTTON_LEFT) 
    {
      lcd.setCursor(0,0);
      lcd.print("Actions         ");
      lcd.setBacklight(GREEN);
      column = column - 130;
      column = constrain(column,0,130);   

      print_screen(function);
    }
    if (LCD_buttons & BUTTON_RIGHT) 
    {
      lcd.setCursor(0,0);
      lcd.print("Sounds           ");
      lcd.setBacklight(RED);
      column = column + 130;
      column = constrain(column,0,130);
      
      lcd.setCursor(0,1);
      lcd.print(function);
      lcd.print("             ");
    }
    if (LCD_buttons & BUTTON_DOWN) 
    {
      function = function - 10;   
      print_screen(function);
    }
    if (LCD_buttons & BUTTON_UP) 
    {
      function = function + 10;
      print_screen(function); 
    }

    
    if (LCD_buttons & BUTTON_SELECT) 
    {
      lcd.setCursor(text_length,1);

      //handle state machine here to bind 2-3-2 actions together

      //generate remaning spaces to fill rest of line after action text
      String spaces;
      for(int i = 0; i < 13-text_length; i++)
        spaces+= " ";
        
      lcd.print(spaces);
      lcd.print(column+function);
     
      analogWrite(5, column+function);
      delay(500);
      analogWrite(5, 0);
    }
  }
}

//This function allows for custom labels for actions
void print_screen(int func)
{
  function = constrain(function,0,120);
  lcd.setCursor(0,1);

  if(column == 130)
  {
    lcd.print(func);
    lcd.print("             ");
  }
  else
  {
    //can add as many names as you want here, be sure to include text length <13 chars
    switch(func)
    {
      case 10:
        lcd.print("INTERLOCK       ");
        text_length = 9;
      break;
      case 20:
        lcd.print("LEG DOWN        ");
        text_length = 8;
      break;
      case 30:
        lcd.print("3 LEG MODE      ");
        text_length = 10;
      break;
      case 40:
        lcd.print("2 LEG MODE      ");
        text_length = 10;
      break;
      case 50:
        lcd.print("LEG UP          ");
        text_length = 6;
      break;
      default: 
        lcd.print(func);
        lcd.print("             ");
    }
  }
  delay(200);
}
