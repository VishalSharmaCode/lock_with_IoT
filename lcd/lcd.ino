#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define RELAY_PIN   33 // ESP32 pin GIOP14 connected to the IN pin of relay
#define ROW_NUM     4  // four rows
#define COLUMN_NUM  4  // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
}

byte pin_rows[ROW_NUM] = {13, 12, 14, 27};    // GIOP19, GIOP18, GIOP5, GIOP17 connected to the row pins
byte pin_column[COLUMN_NUM] = {26, 25, 32, 35}; // GIOP16, GIOP4, GIOP0, GIOP2 connected to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

const String password_1 = "1234"; // change your password here
const String password_2 = "5678";  // change your password here
const String password_3 = "2211";  // change your password here
String input_password;

void setup() {
  Serial.begin(9600);
  input_password.reserve(32);    // maximum input characters is 32
  pinMode(RELAY_PIN, OUTPUT); 
  digitalWrite(RELAY_PIN, HIGH);
  lcd.init();                    // initialize the lcd
  lcd.backlight();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '*') {
      input_password = ""; // reset the input password
      lcd.clear();
    } else if (key == '#') {
      lcd.clear();
      if (input_password == password_1 || input_password == password_2 || input_password == password_3) {
        Serial.println("Valid Password => unlock the door");
        lcd.setCursor(0, 0);
        lcd.print("CORRECT!");
        lcd.setCursor(0, 1);
        lcd.print("DOOR UNLOCKED!");

        digitalWrite(RELAY_PIN, LOW);  // unlock the door for 20 seconds
        delay(20000);
        digitalWrite(RELAY_PIN, HIGH); // lock the door
      } else {
        Serial.println("Invalid Password => Try again");
        lcd.setCursor(0, 0);
        lcd.print("INCORRECT!");
        lcd.setCursor(0, 1);
        lcd.print("ACCESS DENIED!");
      }

      input_password = ""; // reset the input password
    } else {
      if (input_password.length() == 0) {
        lcd.clear();
      }

      input_password += key; // append new character to input password string
      lcd.setCursor(input_password.length(), 0); // move cursor to new position
      lcd.print('*');                 // print * key as hiden character
    }
  }
}
