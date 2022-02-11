#pragma once

#include <Arduino.h>

namespace AGE {
	enum SpecialChar {
		ARROW_LEFT = '←',
		ARROW_RIGHT = '→',
		BLOCK = '∎'
	};

	namespace Utils {
		uint8_t getCharCode(char character);

		void strToLCDEncoding(String& string);
	}
}