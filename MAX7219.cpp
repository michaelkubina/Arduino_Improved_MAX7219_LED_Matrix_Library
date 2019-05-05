/*
	MAX7219.cpp - Library for MAX7219-driven 8*8 LED-Matrix.
	Copyright (C) by Michael Kubina, February 14, 2019.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Arduino.h"
#include "MAX7219.h"

// OPCODES

#define NO_OP			0
#define DIGIT_0			1
#define DIGIT_1			2
#define DIGIT_2			3
#define DIGIT_3			4
#define DIGIT_4			5
#define DIGIT_5			6
#define DIGIT_6			7
#define DIGIT_7			8
#define DECODE_MODE		9
#define INTENSITY		10
#define SCAN_LIMIT		11
#define SHUTDOWN		12
#define DISPLAY_TEST	15

/*
 *	Constructor
 */

MAX7219::MAX7219(byte devices, byte dataPin, byte clockPin, byte loadPin)
{
	if(devices > 0)
	{
		_maxDevices = devices;
		
		_dataPin = dataPin;
		_clockPin = clockPin;
		_loadPin = loadPin;
		
		pinMode(_dataPin, OUTPUT);
		pinMode(_clockPin, OUTPUT);
		pinMode(_loadPin, OUTPUT);
		
		// initialize Matrix-Modes
		_decodeMode 		= new byte[_maxDevices];
		_intensity 			= new byte[_maxDevices];
		_scanLimit 			= new byte[_maxDevices];
		_shutdown 			= new bool[_maxDevices];
		_displayTest 		= new bool[_maxDevices];
		
		_isUpdateMode		= true;
		
		for (int i = 0; i < _maxDevices; i++)
		{
			_decodeMode[i] 	= 0b00000000;
			_intensity[i] 	= 1;
			_scanLimit[i] 	= 7;
			_shutdown[i] 	= false;
			_displayTest[i] = false;
		}
		
		_updateMode();
		
		// initialize Matrix-Screen
		_onScreenBuffer 	= new byte[_maxDevices*8];
		_offScreenBuffer 	= new byte[_maxDevices*8];
		
		for (int i = 0; i < _maxDevices*8; i++)
		{
			_onScreenBuffer[i] 	= 0b00000000;
			_offScreenBuffer[i] = _onScreenBuffer[i];
		}
		
		_updateScreen();
	}
};
 
void MAX7219::_updateMode()
{
	if(_isUpdateMode)
	{
		/* 
		 *	DECODE_MODE
		 */
		
		digitalWrite(_loadPin, LOW);
		for(int i = _maxDevices-1; i >= 0; i--)
		{
			// shift out data - 8Bit Opcode, 8Bit Value
			shiftOut(_dataPin, _clockPin, MSBFIRST, DECODE_MODE);
			shiftOut(_dataPin, _clockPin, MSBFIRST, _decodeMode[i]);
		}
		digitalWrite(_loadPin, HIGH);
		
		/* 
		 *	INTENSITY
		 */
		digitalWrite(_loadPin, LOW);
		for(int i = _maxDevices-1; i >= 0; i--)
		{
			// shift out data - 8Bit Opcode, 8Bit Value
			shiftOut(_dataPin, _clockPin, MSBFIRST, INTENSITY);
			shiftOut(_dataPin, _clockPin, MSBFIRST, _intensity[i]);
		}
		digitalWrite(_loadPin, HIGH);
		
		/* 
		 *	SCAN_LIMIT
		 */
		digitalWrite(_loadPin, LOW);
		for(int i = _maxDevices-1; i >= 0; i--)
		{
			// shift out data - 8Bit Opcode, 8Bit Value
			shiftOut(_dataPin, _clockPin, MSBFIRST, SCAN_LIMIT);
			shiftOut(_dataPin, _clockPin, MSBFIRST, _scanLimit[i]);
		}
		digitalWrite(_loadPin, HIGH);
		
		/* 
		 *	SHUTDOWN
		 */
		digitalWrite(_loadPin, LOW);
		for(int i = _maxDevices-1; i >= 0; i--)
		{
			// shift out data - 8Bit Opcode, 8Bit Value
			shiftOut(_dataPin, _clockPin, MSBFIRST, SHUTDOWN);
			shiftOut(_dataPin, _clockPin, MSBFIRST, (byte) ~_shutdown[i]);
		}
		digitalWrite(_loadPin, HIGH);
		
		/* 
		 *	DISPLAY_TEST
		 */
		digitalWrite(_loadPin, LOW);
		for(int i = _maxDevices-1; i >= 0; i--)
		{
			// shift out data - 8Bit Opcode, 8Bit Value
			shiftOut(_dataPin, _clockPin, MSBFIRST, DISPLAY_TEST);
			shiftOut(_dataPin, _clockPin, MSBFIRST, (bool) _displayTest[i]);
		}
		digitalWrite(_loadPin, HIGH);
	
		// All Mode changes made
		_isUpdateMode = false;
	}
};

void MAX7219::_updateScreen()
{
	/*
	 *	1.	Load _offScreenBuffer to _onScreenBuffer
	 */
	 
	for (int i = 0; i < _maxDevices*8; i++)
	{
		_onScreenBuffer[i] = _offScreenBuffer[i];
	}
	
	/*
	 *	2.	Send _onScreenBuffer-Data to Screen
	 */

	for (int digitRegister = 1; digitRegister <= 8; digitRegister++)
	{
		digitalWrite(_loadPin, LOW);
		for(int device = _maxDevices; device >= 1; device--)
		{
			shiftOut(_dataPin, _clockPin, MSBFIRST, (byte) digitRegister);
			shiftOut(_dataPin, _clockPin, MSBFIRST, (byte) _onScreenBuffer[((device-1)*8)+(digitRegister-1)]);
		}
		digitalWrite(_loadPin, HIGH);
	}
	
	/*
	 *	3.	Flush _offScreenBuffer
	 */
	
	for (int i = 0; i < _maxDevices*8; i++)
	{
		_offScreenBuffer[i] = 0;
	}
};

void MAX7219::update()
{
	_updateMode();
	_updateScreen();
};

void MAX7219::drawPixel(byte x, byte y)
{
	// check if within limit
		if((x >= 1 && x <= 40) && (y >= 1 && y <= 8))
		{
			byte pixel = 0b00000001 << (y - 1);
			_offScreenBuffer[x - 1] |= pixel;
		}
};

void MAX7219::drawHLine(byte y, byte x1, byte x2)
{
	if (y > 0 && y <= 8)
	{
		while(x1 <= x2)
		{
			drawPixel(x1, y);
			x1++;
		}
	}
};

void MAX7219::drawVLine(byte x, byte y1, byte y2)
{
	// check if within borders
	if ( x > 0 && x <= 40)
	{
		byte mask = 0b11111111;
		byte top = mask << (y1 - 1);
		byte bottom = mask >> (8 - y2);
		mask = top & bottom;
		
		this->_offScreenBuffer[x - 1] |= mask;
		return;
	}
};

void MAX7219::drawRect(byte x, byte y, byte width, byte height)
{
	drawHLine(y, x, x+width-1);
	drawHLine(y+height-1, x, x+width-1);
	drawVLine(x, y, y+height-1);
	drawVLine(x+width-1, y, y+height-1);
};

void MAX7219::drawChar(const byte *symbol, byte x)
{
	for(int i = 0; i <= 7; i++)
	{
		for (int val = 0; val <= 7; val++)
		{
			int mask = 0b00000001;
			int position = mask << (7 - val);
			int masked_value = position & symbol[i];
			masked_value = masked_value >> (7 - val);
			if (masked_value)
			{
				drawPixel(val+1+x, i+1);
			}
		}
	}
};

/*
void MAX7219::drawString(String message, byte x)
{	
	for(unsigned int i = 0; i <= message.length(); i++)
	{
		switch(message.charAt(i))
		{
			case 'L':
				drawChar(L, x+(i*8));
				break;
			case 'A':
				drawChar(A, x+(i*8));
				break;
			case 'U':
				drawChar(U, x+(i*8));
				break;
			case 'R':
				drawChar(R, x+(i*8));
				break;
			default:
				break;
		}
	}
};
*/

void MAX7219::invert()
{
	for (int i = 0; i < _maxDevices*8; i++)
	{
		_offScreenBuffer[i] = ~_offScreenBuffer[i];
	}
};