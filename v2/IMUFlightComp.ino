// Flight Computer 5.5.23

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>  // IMU
#include "Adafruit_BMP3XX.h" // BMP
#include <Wire.h>
#include <utility/imumaths.h>   // IMU
#include <SPI.h>
#include <SD.h>

// Global constants
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

String readBMP() {
  // readBMP reads the current value from the BMP388, returns a string in CSV format
  if (! bmp.performReading()) {
    #if IS_DEBUG_BMP
    Serial.println("Failed to perform reading :(");
    #endif
    return;
  }
  
  String bmp_output = String(bmp.temperature) + "*C," + String(bmp.pressure/100.0) + "hPa," + String(bmp.readAltitude(SEALEVELPRESSURE_HPA)) + "m" ;
  
  if(!IS_DEBUG_BMP){
    return bmp_output;  
  }
  else {
    return "Temperature (*C) = " + String(bmp.temperature) + "\nPressure (hPa) = " + String(bmp.pressure/100.0) + "\nApprox. Altitude (m) = " + String(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  }
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
    //Serial.print("Writing to " + fileName);
    #endif
    myFile.println(contents);
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}

// IMU Functions
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  initialize();
  setupBMP(); 
  myIMU.begin();

  createFile("test.txt");
  int8_t temp = myIMU.getTemp();
  myIMU.setExtCrystalUse(true);
  delay(DELAY_FIVE);
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
  
  String altitude = String(bmp.readAltitude(SEALEVELPRESSURE_HPA));

  //String combo = String(imu_output) + String(bmp_output);
  // Print to terminal

  Serial.println(imu_output);
  //Serial.println(altitude);
  //Serial.println(combo);
  
  // Write to SD Card file
  //writeFile("test5.txt", imu_output);
  //writeFile("test4.txt", bmp_output);

  delay(BNO055_SAMPLERATE_DELAY_MS);
}
