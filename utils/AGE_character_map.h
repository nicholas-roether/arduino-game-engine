#pragma once

#include <Arduino.h>

namespace AGE {
	constexpr uint16_t ARROW_LEFT = '←';
	constexpr uint16_t ARROW_RIGHT = '→';
	constexpr uint16_t BLOCK = '∎';

	namespace Utils {
		uint8_t getCharCode(char character);

		void strToLCDEncoding(String& string);
	}
}