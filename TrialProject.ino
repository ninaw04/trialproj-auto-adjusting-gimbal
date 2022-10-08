#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#define PI 3.1415926535897932384626433832795
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
float LSBSensitivity = 16384.0; // for +/- 2g

int servoPin = 9;
double servoPos = 0;
int servoDegree = 0;
Servo myServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  setupMPU(); // function specified later to setup registers
  myServo.attach(servoPin);
  Serial.println("What angle for servo? ");
  while(Serial.available() == 0) {
  }
  servoDegree = Serial.parseInt();
}

void loop() {
  // put your main code here, to run repeatedly:
  storeAccelValues();
  printData();
  writeToServo(servoDegree);

  delay(100);
}

void setupMPU() {
  // I2C interface setup -- slave address is b110100X
  Wire.beginTransmission(0b1101000);
  // Register 6B
  Wire.write(0x6B); // 4.28 Read/Write
  Wire.write(0b00000000); // sets sleep to 0
  Wire.endTransmission();
  // Repeat with accelerometer setup
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C); // 4.5 Accelerometer config
  Wire.write(0b00000000); // +/- 2g full scale range
  Wire.endTransmission();
}

void storeAccelValues() {
  Wire.beginTransmission(0b1101000);
  Wire.write(0x3B);
  Wire.endTransmission();

  Wire.requestFrom(0b1101000, 6); // Accel Registers 3B - 40
  while(Wire.available() < 6);
  // left shift 8 bits for each register
  accelX = Wire.read()<<8|Wire.read();
  accelY = Wire.read()<<8|Wire.read();
  accelZ = Wire.read()<<8|Wire.read();
  gForceX = accelX/LSBSensitivity;
  gForceY = accelY/LSBSensitivity;
  gForceZ = accelZ/LSBSensitivity;
}

void printData() {
  Serial.print("Accel (g)");
  Serial.print(" X: ");
  Serial.print(gForceX);
  Serial.print(" Y: ");
  Serial.print(gForceY);
  Serial.print(" Z: ");
  Serial.println(gForceZ);
}

void writeToServo(int servoDegree) {
  int printPos = 0;
  if(gForceX == 0) {
    servoPos = 90;
  }
  else{
    servoPos = atan(gForceY/gForceX);
  }
  if(gForceY < 0) {
    printPos = (-servoDegree + 90 - (int) abs(servoPos*57.29));
  } else {
    printPos = (-servoDegree + 90 + (int) abs(servoPos*57.29));
  }
  myServo.write(printPos);
}
