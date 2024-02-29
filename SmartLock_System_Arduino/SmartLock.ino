#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo servo;


const int ROW_NUM = 4;
const int COLUMN_NUM = 3;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {10, 9, 8, 7}; // pinii pentru randuri
byte pin_column[COLUMN_NUM] = {6, 5, 4}; // pinii pentru coloane

// Instanta keypad-ului
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

String introducedPassword;
char keyPressed;
String password = "6622";

bool verif = false;

String newPIN = "";
String currentNumber = "";
String s = "";
bool ok = false;

int redPin = 12;
int greenPin = 13;
String myPhone = "0742068758";

void setup()
{
  // initialize the LCD
  lcd.begin();
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
  mySerial.println("AT"); 
  updateSerial();
  lcd.setCursor(0, 0);
  lcd.print("1");
  delay(1000);
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  lcd.setCursor(0, 0);
  lcd.print("2");
  delay(1000);
  mySerial.println("AT+CNMI=1,2,0,0,0");
  updateSerial();
  lcd.setCursor(0, 0);
  lcd.print("3");
  delay(1000);
  
  // Turn on the blacklight and print a message.
  lcd.backlight();
  servo.write(15);
  //lcd.print("Hello, world!");
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  servo.attach(11);
}

void loop()
{
  updateSerial();
  // Do nothing here...
  readKeypad();
 lcd.setCursor(0, 0);
 lcd.print("PIN:");
 lcd.print(introducedPassword);
 



}
void readKeypad() {
  keyPressed = keypad.getKey();
  if (keyPressed >= '1' && keyPressed <= '9') {
    String konv = String(keyPressed);
    introducedPassword += konv;
  }
  else if(keyPressed == '#'){
    if (introducedPassword == password){
      lcd.setCursor(0,1);
      lcd.print("Correct!!");
      digitalWrite(greenPin, HIGH);
      servo.write(90);
    }
    else{
      lcd.setCursor(0,1);
      lcd.print("Try again!");
      digitalWrite(redPin, HIGH);
      delay(2000);
      lcd.clear();
      digitalWrite(redPin, LOW);
      introducedPassword = "";
      }
    }
    else if(keyPressed == '*'){
      servo.write(15);
      lcd.clear();
      digitalWrite(greenPin, LOW);
      introducedPassword = "";
    }
}

void updateSerial()
{
  ok = false;
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    if(ok == false){
      s = "";
      ok = true;
      verif = true;
    }
    char c = mySerial.read();
    s = s + c;
    Serial.write(c);
  }
  int len = s.length();
  newPIN = s.substring(50, len - 2);
  currentNumber = s.substring(11, 11 + 10);
  if(verif){
   if(currentNumber == myPhone){
      
      password = newPIN;
      lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("PIN changed!!");
     delay(2000);
     lcd.clear();
     currentNumber = "";
    }
    verif = false;
  }
}
