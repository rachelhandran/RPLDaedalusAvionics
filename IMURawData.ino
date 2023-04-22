#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include <math.h>

// Global constants
float thetaM;
float phiM;
float thetaFold=0;
float thetaFnew;
float phiFold=0;
float phiFnew;
 
float thetaG=0;
float phiG=0;
 
float dt;
unsigned long millisOld;

#define BNO055_SAMPLERATE_DELAY_MS (100)
#define FIVE (5000)

Adafruit_BNO055 myIMU = Adafruit_BNO055();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  
  myIMU.begin();
  delay(1000);
  int8_t temp = myIMU.getTemp();
  Serial.println(temp);
  myIMU.setExtCrystalUse(true);
  //delay(FIVE);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t system, gyro, accel, mg = 0;
  myIMU.getCalibration(&system, &gyro, &accel, &mg);
  imu::Vector<3> acc =myIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyr =myIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
   
  thetaM=-atan2(acc.x()/9.8,acc.z()/9.8)/2/3.141592654*360;
  phiM=-atan2(acc.y()/9.8,acc.z()/9.8)/2/3.141592654*360;
  phiFnew=.95*phiFold+.05*phiM;
  thetaFnew=.95*thetaFold+.05*thetaM;
   
  dt=(millis()-millisOld)/1000.;
  millisOld=millis();
   
  thetaG=thetaG+gyr.y()*dt;
  phiG=phiG-gyr.x()*dt;
   
  Serial.print(acc.x()/9.8);
  Serial.print(",");
  Serial.print(acc.y()/9.8);
  Serial.print(",");
  Serial.print(acc.z()/9.8);
  Serial.print(",");
  Serial.print(accel);
  Serial.print(",");
  Serial.print(gyro);
  Serial.print(",");
  Serial.print(mg);
  Serial.print(",");
  Serial.print(system);
  Serial.print(",");
  Serial.print(thetaM);
  Serial.print(",");
  Serial.print(phiM);
  Serial.print(",");
  Serial.print(thetaFnew);
  Serial.print(",");
  Serial.print(phiFnew);
  Serial.print(",");

  Serial.print(thetaG);
  Serial.print(",");
  Serial.println(phiG);
   
  phiFold=phiFnew;
  thetaFold=thetaFnew;
   
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
