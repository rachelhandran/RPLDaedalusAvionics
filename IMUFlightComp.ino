#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include <SD.h>

// Global constants
#define BNO055_SAMPLERATE_DELAY_MS (100)
#define FIVE (5000)

#define IS_DEBUG TRUE // Keep FALSE cancels most print statements, all functions read/write still work

Sd2Card card;
SdVolume volume;
SdFile root;

//Adafruit_BNO055 myIMU = Adafruit_BNO055();
Adafruit_BNO055 myIMU = Adafruit_BNO055(-1, 0x28, &Wire);

File myFile;
const int CS = 10;

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

void createFile(String fileName){
    myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    Serial.print("Creating " + fileName + "...\n");
    myFile.close();
    Serial.println("done.\n");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + fileName + "\n");
  }
}

void writeFile(String fileName, String contents){
    myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    #if IS_DEBUG
    Serial.print("Writing to " + fileName);
    #endif
    myFile.println(contents);
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  initialize();
  //SD.begin(CS);
  
  myIMU.begin();

  createFile("test.txt");
  int8_t temp = myIMU.getTemp();
  myIMU.setExtCrystalUse(true);
  delay(FIVE);
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
  writeFile("test.txt", imu_output);  
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
