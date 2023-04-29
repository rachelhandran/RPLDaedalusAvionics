# RPL NMP Avionics

This is the flight computer for our NMP Avionics system.

flightComputer.ino has the main system. Once uploaded, there will be a 10s delay, then the BMP388 sensor will read in temp, pressure, and altitude data, and write to a file in the SD card every second.

flightCompDebug.ino is a duplicate sandbox code to practice and debug. 

Use flightCompDebug.ino when connected to a computer and for all testing purposes. Upload and use flightComputer.ino for flight and when only connected to a power source. Ensure the source has a switch, and that it is powered off at all times except for launch and recovery. 

# Hardware:
Ensure the BMP388 and BMP280 are wired correctly. On the PCB, GND is the 2nd pin from the right. Match these when orienting your components to ensure functionality.
