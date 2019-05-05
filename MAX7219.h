/*
	MAX7219.h - Library for MAX7219-driven 8*8 LED-Matrix.
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

#ifndef MAX7219_h
#define MAX7219_h

#include "Arduino.h"

class MAX7219
{
	// Private variables
	private:
		
		byte _dataPin;
		byte _clockPin;
		byte _loadPin;
		
		byte _maxDevices;
		
		// status if mode changed
		bool _isUpdateMode;
		// individual modes
		byte *_decodeMode;
		byte *_intensity;
		byte *_scanLimit;
		bool *_shutdown;
		bool *_displayTest;
		
		// screenbuffers
		byte *_onScreenBuffer;
		byte *_offScreenBuffer;		
	
	// Private functions
	private:
		
		// updates the operational modes
		void _updateMode();
		void _updateScreen();
	
	// Public variables
	public:
		
	// Public Functions
	public:
	
		/*
		 *	Constructor with initialy blanked display
		 */
		
		MAX7219(byte devices, byte dataPin, byte clockPin, byte loadPin);
		
		/*
		 *	update Matrix-Mode-Data and Matrix-Screen-Data
		 */
		
		void update();
		
		/*
		 *	Draw functions
		 */
		
		// Single Pixel
		void drawPixel(byte x, byte y);
		
		// Lines
		void drawHLine(byte y, byte x1, byte x2);
		void drawVLine(byte x, byte y1, byte y2);
		
		// Forms
		void drawRect(byte x, byte y, byte width, byte height);

		// Symbols
		void drawChar(const byte *symbol, byte x);
		void drawString(String message, byte x);
		
		/*
		 *	Screen manipulation
		 */
		
		void invert();
};

#endif