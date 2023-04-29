# RPL NMP Avionics

This is the flight computer for our NMP Avionics system. 

*The software is designed for BMP388 only. Software is necessarily compatible for BMP280. Hardware IS compatible with both, but you must write your own code for the BMP280.*

`flightComputer.ino` is the flight program. Once uploaded, there will be a 5s delay, then the BMP388 sensor will read in temp, pressure, and altitude data, and write to a file in the SD card once every 100ms.

`flightCompDebug.ino` is outdated. It was a duplicate sandbox code to practice and debug. 

# Hardware:
## Avionics PCB:
Ensure the BMP388 and BMP280 are wired/soldered to the PCB correctly. 
On the PCB, GND is the 2nd pin from the right. Match the GND pin of your component (BMP280) and the GND pin on the PCB when orienting your components to ensure functionality.

# Procedure:
1. Solder PCB components: Arduino Nano, BMP388, SD Card Module, and battery. Ensure you have a new battery. Keep battery OFF. Mount an SD Card.
2. Download flightComputer.ino to your Arduino IDE.
3. Edit flightComputer.ino to Arduino Nano. For testing, ensure number in the filename in line 164 is 100+. If flying, change number to 1-99. This helps for post-processing to distinguish the correct data.
4. Upload your version of flightComputer.ino to the Nano thru USB cable. Check serial monitor to ensure valid output stream. Unplug the cable, now you have successfully uploaded the program and will supply power from now on through the battery pack.
5. Mount the avionics bay in your rocket. When flight time comes, turn on the battery pack. Remember, after 5s, the program will begin. It will end when it loses power or runs out of memory. (Battery goes out much quicker (~10h) than filling up 32GB of storage with plain text, see notes.)
6. Recover the rocket and turn OFF the switch. Read from SD Card for data analysis.


# Notes:
## Memory
How long will it take to fill a 32gb SD card?
- In CSV file, 25 chars = 25 bytes per line
	- 25 bytes / 0.1 second
	- 250 bytes / 1 second
	- 15,000 bytes / 1 min
	- 900,000 bytes / 1 hour
	- 21,600,000 / 24hrs or 1 day
- 32GB = 32,000,000,000 bytes
- It will take 35,555 hours or 1481 days to fill the SD Card at a rate of 25 bytes / sec and 32GB storage
