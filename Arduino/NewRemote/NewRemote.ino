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
int function = 1;
int buttonValue = 0;

int buttons[] = {2, 5, 4, 7, 8, 9, 11, 10, 13, 12};

int text_length = 0;
enum states{interlock, down, leg3, leg2, up};

unsigned long clear_ch5_time = 0, clear_ch6_time = 0;
bool clear_ch5 = 0, clear_ch6 = 0;

void setup() 
{
  lcd.begin(16, 2);
  lcd.setBacklight(YELLOW);
  lcd.setCursor(0,0);
  lcd.print("R6 Droid Control");
  delay(2000);
  lcd.clear();
  lcd.setBacklight(GREEN);
  lcd.print("Actions         ");
  print_screen(function); 
  
  pinMode(3, OUTPUT); // channel 5
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
    {
      buttonValue = i*1+1;//assign button value 
      break;
    }
  }
  
  if(buttonValue != 0)//check if we had a button press
  {
    if(buttonValue == 10)//handle text shifting between signle and double digit
      lcd.setCursor(14,0);
    else
      lcd.setCursor(15,0);
      
    lcd.print(buttonValue);
 
    analogWrite(6, buttonValue*15);//write button value PWM
    clear_ch6 = 1;
    clear_ch6_time = millis();

    delay(1000);
  }


  uint8_t LCD_buttons = lcd.readButtons();//read LCD buttons

  if (LCD_buttons) //handle the LCD buttons
  {
    if (LCD_buttons & BUTTON_LEFT) 
    {
      lcd.setCursor(0,0);
      lcd.print("Actions         ");
      lcd.setBacklight(GREEN);

      if(column == 10)
        column = 0;   

      print_screen(function);
    }
    if (LCD_buttons & BUTTON_RIGHT) 
    {
      lcd.setCursor(0,0);
      lcd.print("Sounds           ");
      lcd.setBacklight(RED);

      if(column == 0)
        column = 10;
         
      lcd.setCursor(0,1);
      lcd.print(function);
      lcd.print("             ");
    }
    if (LCD_buttons & BUTTON_DOWN) 
    {
      if(function - 1 != 0)
      {
        function = function - 1;   
        print_screen(function);
      }
    }
    if (LCD_buttons & BUTTON_UP) 
    {
      if(function + 1 < 11)
      {
        function = function + 1;
        print_screen(function); 

      }
    }

    if (LCD_buttons & BUTTON_SELECT) 
    {
      if(column == 10)//sounds only called numbers
      {
        if(function >= 10)
          lcd.setCursor(14,1);
        else
          lcd.setCursor(15,1);
            
        lcd.print(function);
      }

      else//actions can be displayed as text
      {
        lcd.setCursor(text_length,1);
  
        //generate remaning spaces to fill rest of line after action text
        String spaces;
        int num_spaces = 15;
        if(function >= 10)
          num_spaces = 14;
          
        for(int i = 0; i < num_spaces-text_length; i++)
          spaces+= " ";
          
        lcd.print(spaces);
        lcd.print(function);
      }
      
      analogWrite(3, (column*7)+(function*7));
      clear_ch5 = 1;
      clear_ch5_time = millis();
    }
  }

  if(clear_ch5 || clear_ch6)
  {
    //shut off command after period of time
    if(millis() - clear_ch5_time >= 500)
    {
      analogWrite(3, 0);
  
      lcd.setCursor(13,1);
      lcd.print("   ");
      clear_ch5 = 0;
    }
  
    //shut off command after period of time
    if(millis() - clear_ch6_time >= 500)
    {
      analogWrite(6, 0);
  
      lcd.setCursor(13,0);
      lcd.print("   ");
      clear_ch6 = 0;
    }
  }
}


//This function allows for custom labels for actions
void print_screen(int func)
{
  function = constrain(function,0,10);
  lcd.setCursor(0,1);

  if(column == 10)
  {
    lcd.print(func);
    lcd.print("              ");
  }
  else
  {
    //can add as many names as you want here, be sure to include text length <13 chars
    switch(func)
    {
      case 1:
        lcd.print("INTERLOCK       ");
        text_length = 9;
      break;
      case 2:
        lcd.print("LEG DOWN        ");
        text_length = 8;
      break;
      case 3:
        lcd.print("3 LEG MODE      ");
        text_length = 10;
      break;
      case 4:
        lcd.print("2 LEG MODE      ");
        text_length = 10;
      break;
      case 5:
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
