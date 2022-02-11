#pragma once

#include <Arduino.h>

namespace AGE {
	namespace Utils {
		char getCharCode(wchar_t character);

		void strToLCDEncoding(const char16_t* string, String& target);
	}
}