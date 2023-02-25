#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
#define SEALEVELPRESSURE_HPA (1013.25) // bmp

#define IS_DEBUG TRUE //FALSE cancels most print statements, all functions read/write still work
#define IS_DEBUG_BMP FALSE 

File myFile;
const int CS = 10;

Sd2Card card;
SdVolume volume;
SdFile root;
Adafruit_BMP3XX bmp; // bmp

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
  
  #if not IS_DEBUG_BMP
  return bmp_output;
  //delay(5000);
  #endif
  
  #if IS_DEBUG_BMP
  Serial.print("Temperature (*C) = ");
  Serial.println(bmp.temperature);
  Serial.print("Pressure (hPa) = ");
  Serial.println(bmp.pressure / 100.0);
  Serial.print("Approx. Altitude (m) = ");
  Serial.println(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  delay(2000);
  #endif
}

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

// DEBUGGING (Serial functions)
#if IS_DEBUG
void listFiles(){
  if (!card.init(SPI_HALF_SPEED, CS)) {
    Serial.println("init failed");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }
  Serial.println("Files found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  root.ls(LS_R | LS_DATE | LS_SIZE);
  Serial.println("\n");
}
#endif

// SD Card Functions v1.0 rachelhandran  2.10.23
void readFile(String fileName){
    myFile = SD.open(fileName);
  if (myFile) {
    Serial.println("Contents of file:");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("error opening file");
  }
  Serial.println("End of File\n");
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
    Serial.println("error opening test.txt");
  }
}

void clearContents(String fileName){
  myFile = SD.open(fileName, FILE_WRITE | O_TRUNC);
  if (myFile) {
    Serial.println("Deleting contents...");
    myFile.close();
  } else {
    Serial.println("error opening file");
  }
}
void deleteFile(String fileName){
  myFile = SD.open(fileName);
  Serial.println("Deleting " + fileName);
  if (myFile){
    myFile.close();
    SD.remove(fileName);
  } else {
    Serial.println(fileName + " does not exist. No action taken.\n");
  }
}

//TODO Check existence


// -------------------------------
//           CODE HERE

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }
  setupBMP(); 
  initialize();
  
  listFiles();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //writeFile("test6.txt", readBMP());  
  //Serial.println(readBMP());
  //delay(5000);

}
