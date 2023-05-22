/*
 * Rachel Handran 
 * Flight Computer 5.19.23 with PCB v2.0
 * Current working state: Successfully prints out BMP and IMU calibration and accel, mag, 
 * and gyro data to the serial monitor and writes to a file called on the SD. 
 * 
 * CSV Format: bmp data, imu calibration, then imu raw data
 *    bmp.temp,bmp.pressure,bmp.altitude,system,accel,gyro,mg,acc.x(),acc.y(),acc.z(),gyr.x(),gyr.y(),gyr.z(),mag.x(),mag.y(),mag.z()
*/

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>  // IMU
#include "Adafruit_BMP3XX.h" // BMP
#include <Wire.h>
#include <utility/imumaths.h>   // IMU
#include <SPI.h>
#include <SD.h>

// Global constants
#define FLIGHT FALSE // TRUE means will only upload the flight info, no serial prints (keep FALSE)
#define IS_DEBUG TRUE // FALSE cancels most print statements, all functions read/write still work
#define IS_DEBUG_BMP FALSE // Keep FALSE
#define DELAY_FIVE (5000)

// BNO Constants
#define BNO055_SAMPLERATE_DELAY_MS (100)

// BMP Constants
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
#define SEALEVELPRESSURE_HPA (1013.25) // bmp

// Setting up hardware connections
Sd2Card card;
SdVolume volume;
SdFile root;

Adafruit_BMP3XX bmp; // bmp
Adafruit_BNO055 myIMU = Adafruit_BNO055(-1, 0x28, &Wire); // IMU

File myFile;
const int CS = 10;

// Functions

// BMP Functions
void setupBMP() { //bmp
  if (!bmp.begin_I2C(0x77)) {   // hardware I2C mode, can pass in address & alt Wire
    #if IS_DEBUG_BMP
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    #endif
    while (1);
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

// SD Card Functions
void initialize(){
    #if IS_DEBUG
    Serial.print("Initializing SD card...");
    #endif

  if (!SD.begin(CS)) {
    #if IS_DEBUG
    Serial.println("initialization failed!");
    #endif
    while (1); //stalls the program
  }
  #if IS_DEBUG
  Serial.println("initialization done.");
  #endif
}

// IMU Functions
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  initialize(); //SDcard
  setupBMP(); 
  myIMU.begin();

  int8_t temp = myIMU.getTemp();
  myIMU.setExtCrystalUse(true);
  delay(DELAY_FIVE);
}

void loop() {  
  // put your main code here, to run repeatedly:
  uint8_t sys, gyros, accel, mg = 0;
  myIMU.getCalibration(&sys, &gyros, &accel,&mg);

  imu::Vector<3> acc = myIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyr = myIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag = myIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  /*
  String imu_output = String(accel) + "," + String(gyros)+ "," + String(mg) + "," + String(sys)  + "," + 
      String(acc.x()) + "," + String(acc.y()) + "," + String(acc.z()) + "," + 
      String(gyr.x()) + "," + String(gyr.y()) + "," + String(gyr.z()) + "," +
      String(mag.x()) + "," + String(mag.y()) + "," + String(mag.z()) + ",";
  */

// NEW Development: 5.19.23

#if !FLIGHT
  Serial.println("");
  Serial.print(bmp.temperature);
  Serial.print(",");  
  Serial.print(bmp.pressure/100.0);
  Serial.print(",");  
  Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.print(",");  

  Serial.print(sys);
  Serial.print(",");  
  Serial.print(accel);
  Serial.print(",");  
  Serial.print(gyros);
  Serial.print(",");  
  Serial.print(mg);

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
  Serial.print(mag.z());
#endif
  
  // Write to SD Card file

  //Taken from writeFile() function in previoous versions, for testing, copy that original code
  myFile = SD.open("FLIGHT.txt", FILE_WRITE);
 
  myFile.print(bmp.temperature);
  myFile.write(",");
  myFile.print(bmp.pressure/100.0);
  myFile.write(",");
  myFile.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  myFile.write(",");

  myFile.print(sys);
  myFile.write(",");
  myFile.print(accel);
  myFile.write(",");
  myFile.print(gyros);
  myFile.write(",");
  myFile.print(mg);
  myFile.write(",");
  
  myFile.print(acc.x());
  myFile.write(",");
  myFile.print(acc.y());
  myFile.write(",");
  myFile.print(acc.z());
  myFile.write(",");

  myFile.print(gyr.x());
  myFile.write(",");
  myFile.print(gyr.y());
  myFile.write(",");
  myFile.print(gyr.z());
  
  myFile.print(mag.x());
  myFile.write(",");
  myFile.print(mag.y());
  myFile.write(",");
  myFile.print(mag.z());
 
  myFile.write("\n");

  myFile.close();
 
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
