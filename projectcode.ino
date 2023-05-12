#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <ADXL335.h>
#include <TinyGPSPlus.h>


// define pins for sensors and modules
const int trigPin = 7;
const int echoPin = 6;
const int vibrationPin = A1;
const int alcoholPin = A0;
      
const int xpin = A8;                 
const int ypin = A9;                  
const int zpin = A10;  

const int chipSelect = 4;
const int threshold = 10;

const int RXPin = 2;
const int TXPin = 3;

int GPSBaud = 9600;

int x,y,z;


// create instances of sensors and modules

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

SoftwareSerial sim900A(7, 8);
ADXL335 accelerometer;
SoftwareSerial gpsSerial(RXPin, TXPin);
File dataFile;

void setup() {
  

  Serial.begin(9600);

 
  // initialize LCD and show initial message
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // initialize SIM900A module
  sim900A.begin(9600);
  delay(1000);
  sim900A.println("AT");
  delay(1000);
  sim900A.println("AT+CMGF=1");
  delay(1000);

  // initialize GPS module
  gpsSerial.begin(9600);
  delay(1000);

  // initialize SD card module
  if (!SD.begin(chipSelect)) {
    lcd.clear();
    lcd.print("SD initialization failed");
    while (1);
  }

  // initialize accelerometer/gyroscope
  accelerometer.begin();

  // show ready message on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Black box ready");
  lcd.setCursor(0, 1);
  lcd.print("Drive safely!");
  delay(2000);
}

void loop() {
  // get sensor readings
  
  float distance;
  int vibration = analogRead(vibrationPin);
  int alcohol = analogRead(alcoholPin);
  float xAccel, yAccel, zAccel;
  accelerometer.getXYZ(&x, &y, &z);

  // log data to SD card
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(distance);
    dataFile.print(",");
    dataFile.print(vibration);
    dataFile.print(",");
    dataFile.print(alcohol);
    dataFile.print(",");
    dataFile.print(xAccel);
    dataFile.print(",");
    dataFile.print(yAccel);
    dataFile.print(",");
    dataFile.println(zAccel);
    dataFile.close();
  } else {
    lcd.print("SD write error");
  }

  // display data on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist:");
  lcd.print(distance);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print("Vib:");
  lcd.print(vibration);
  lcd.print(" Alc:");
  lcd.print(alcohol);

  // check for accidents
  float accelSum = abs(xAccel) + abs(yAccel) + abs(zAccel);
  if (accelSum > threshold) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCIDENT!");
    lcd.setCursor(0, 1);
    lcd.print("Sending SMS...");
    delay(2000);

    // send SMS alert
    sim900A.println("AT+CMGS=\"+1234567890\"");
    delay(1000);
    sim900A.println("Accident detected!");
    sim900A.write(26);
    delay(1000);
  }

  // check for GPS data
  while(gpsSerial.available()>0) 
    Serial.write(gpsSerial.read());
    }
  
    
  


float getDistance() {
  // calculate distance using ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}


