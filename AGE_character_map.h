#pragma once

#include <Arduino.h>

namespace AGE {
	namespace Utils {
		uint8_t getCharCode(char16_t character);

		void strToLCDEncoding(String& string);
	}
}