# Arduino_MAX7219_LED_Matrix_Library
This is a library for the 8*8 LED-Matrix with a MAX7219 IC. The goal was to write a library from scratch, purely 
based on the datasheet in order to understand how a SPI-Interface works and how OP-Codes and Register come into play. 
It is similar to the LEDControl-Library from Arduino, but is easier to read for beginners, as which i clearly see myself, 
and has some additional features.  
  
## Features
* daisychaining multiple LED-Matrices  
* tracking of each individual LED-state  
* set, get and invert LED-state for each device, row and column  
* tracking of each individual device-state: DecodeMode, Intensity, ScanLimit, Shutdown, DisplayTest  
* set and get device-state  
* clear, fill or invert whole matrices or all matrices  
  
## Release Notes
  
### Version 1.0  - 2019-02-14
* Initial Upload with example-sketch
  
## Minimal requirements
  
1x Arduino Uno R3  
1x 8*8 LED-Matrix with MAX7219 IC  
5x Jumper Wire  
  
You can daisychain multiple LED-Matrices. In theory this library has no limit on how many you could daisychain, in practice your circuit would become unstable.  
  
## Diagram
  
![alt text](https://github.com/Mimaku/Arduino_MAX7219_LED_Matrix_Library/blob/master/examples/MAX7219/LED-Matrix_Steckplatine.png "Diagram / Breadboard layout")
  
## Usage

```c
// create Matrix-Screen object with number of devices and specified pin-layout
MAX7219 Matrix(NUMBER_OF_Devices, DATAPIN, CLOCKPIN, LOADPIN);
  
void setup() {  
  // individual settings, if required  
}  
  
...  
  
void loop() {  
  // First LED in upper left Corner (Position: x = 0, y = 0)
  Matrix.setLed(1, 0, 0, true);
  delay(1000);  
  
  // 3rd LED in second row  (Position: x = 2, y = 1)  
  Matrix.setLed(1, 2, 1, true);  
  delay(1000);  
  
  // clear display  
  Matrix.clearDisplay(1);  
  delay(1000);  
}  
```  