#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <utility/imumaths.h>

// Global constants
#define BNO055_SAMPLERATE_DELAY_MS (100)
#define FIVE (5000)

//Adafruit_BNO055 myIMU = Adafruit_BNO055();
Adafruit_BNO055 myIMU = Adafruit_BNO055(-1, 0x28, &Wire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  myIMU.begin();
  delay(1000);
  int8_t temp = myIMU.getTemp();
  //Serial.println(temp);
  myIMU.setExtCrystalUse(true);
  //delay(FIVE);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t system, gyros, accel, mg = 0;
  myIMU.getCalibration(&system, &gyros, &accel,&mg);

  imu::Vector<3> acc = myIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyr = myIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag = myIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  String imu_output = String(accel) + "," + String(gyros)+ "," + String(mg) + "," + String(system)  + "," + 
      String(acc.x()) + "," + String(acc.y()) + "," + String(acc.z()) + "," + 
      String(gyr.x()) + "," + String(gyr.y()) + "," + String(gyr.z()) + "," +
      String(mag.x()) + "," + String(mag.y()) + "," + String(mag.z()) + ",";
  Serial.println(imu_output);

  delay(BNO055_SAMPLERATE_DELAY_MS);
}
