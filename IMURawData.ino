#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <utility/imumaths.h>

// Global constants
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
  uint8_t system, gyros, accel, mg = 0;
  myIMU.getCalibration(&system, &gyros, &accel,&mg);

  imu::Vector<3> acc = myIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyr = myIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag = myIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  
  Serial.print(accel);
  Serial.print(",");
  Serial.print(gyros);
  Serial.print(",");
  Serial.print(mg);
  Serial.print(",");
  Serial.print(system);
  Serial.print(",");

  
  Serial.print(acc.x());
  Serial.print(",");
  Serial.print(acc.y());
  Serial.print(",");
  Serial.print(acc.z());
  Serial.print(",");

  Serial.print(gyr.x());
  Serial.print(",");
  Serial.print(gyr.y());
  Serial.print(",");
  Serial.print(gyr.z());
  Serial.print(",");
  
  Serial.print(mag.x());
  Serial.print(",");
  Serial.print(mag.y());
  Serial.print(",");
  Serial.println(mag.z());
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
