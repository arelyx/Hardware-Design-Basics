// include the library code:
#include <LiquidCrystal.h>

// Serial input for setting the lock
String input;
char output[26];
// Initializing the potentiometer pins
int potRed = A0;
int potGreen = A1;
int potBlue = A2;

// Initializing the Lock Variables
int redLock = -1;
int greenLock = -1;
int blueLock = -1;

// Initializing the LED pins
int red = 9;
int green = 10;
int blue = 11;

// Initializing the Green and Red LED pins
int success = 8;
int locked = 13;

// Variable to store the value of the potentiometers
int potRedVal = 0;
int potGreenVal = 0;
int potBlueVal = 0;

// Stores the text for the RGB values displayed on the LCD
char lcdText[16];


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Print a message to the LCD.
  lcd.print(" LockboxProject");
  Serial.println("Set a code XX-XX-XX (0-63)");
  // set up the LED
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);

  pinMode(success,OUTPUT);
  pinMode(locked,OUTPUT);
  Serial.setTimeout(15);
  Serial.begin(9600);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  potRedVal = analogRead(potRed); // read the input pin
  potGreenVal = analogRead(potGreen);
  potBlueVal = analogRead(potBlue);

  if (Serial.available()){
    // Get the user input    
    input = Serial.readString();
    
    // Parse the input and set the lock
    redLock = input.substring(0,input.indexOf("-")).toInt();
    input.remove(0,input.indexOf("-")+1);
    greenLock = input.substring(0,input.indexOf("-")).toInt();
    input.remove(0,input.indexOf("-")+1);
    blueLock = input.substring(0,input.indexOf("-")).toInt();
    input.remove(0,input.indexOf("-")+1);
    sprintf(output,"Set Lock! R:%02d G:%02d B:%02d",redLock,greenLock,blueLock);
    Serial.println(output);
    digitalWrite(locked,HIGH);
    digitalWrite(success,LOW);
  }
  //  Set the RGB led to the potentiomer values
  analogWrite(red,potRedVal/16);
  analogWrite(green,potGreenVal/16);
  analogWrite(blue,potBlueVal/16);
  
  // Displaying the RGB values onto the LCD display 
  lcd.setCursor(0, 1);
  sprintf(lcdText, " R:%02d G:%02d B:%02d",potRedVal/16, potGreenVal/16, potBlueVal/16);
  lcd.print(lcdText);
  delay(25);

  // Check if the lock has been unlocked
  if (potRedVal/16 == redLock and potGreenVal/16 == greenLock and potBlueVal/16 == blueLock){
    Serial.println("Unlocked!");
    digitalWrite(success,HIGH);
    digitalWrite(locked,LOW);
    redLock = -1;
    greenLock = -1;
    blueLock = -1;
  }
}
