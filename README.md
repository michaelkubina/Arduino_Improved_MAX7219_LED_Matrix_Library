# Arduino_Improved_MAX7219_LED_Matrix_Library
This is a improved library for the 8*8 LED-Matrix with a MAX7219 IC. The goal was to achieve better performance in comparison to my first MAX7219-Library. If there are multiple matrices daisychained, they are treated as a single display with increased pixel-dimensions.  
  
The main difference to my Arduino_MAX7219_LED_Matrix_Library is the usage of a framebuffer, which reduces SPI-Transfer-Calls. Furthermore it uses rendering primitives, such as drawing single pixelns, lines and rectangles. As of this, there are not getter and setter for individual LEDs.
  
## Features
* daisychaining multiple LED-Matrices  
* double-framebuffered  
* drawing-primitives (pixels, lines, rectangles)  
* first tests of drawing characters on screen  
  
## Release Notes
  
### Version 0.8  - 2019-05-05
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
  
const static byte HEART[8] = {  
  0b01100110,  
  0b10011001,  
  0b10000001,  
  0b10000001,  
  0b01000010,  
  0b00100100,  
  0b00011000,  
  0b00000000  
};  

void setup() {  
  // not the droids you are looking for  
}  
  
...  
  
void loop() {  
  // draw pixel in upper left corner (Position: x = 1, y = 1)  
  Matrix.drawPixel(1, 1);   
    
  // draw vertical line in third column from y1 = 1 to y2 = 4  (Position: x = 3, y1 = 1, y2 = 4)  
  Matrix.drawVLine(3, 1, 4);  
    
  // draw heart-shaped character with horizontal offset of 1  
  Matrix.drawChar(HEART, 1);  
  
  // render to screen and clear off-screenbuffer  
  Matrix.update();  
  delay(1000);  
}  
```  
