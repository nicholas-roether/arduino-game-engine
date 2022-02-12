#pragma once

#include <Arduino.h>

namespace AGE {
	namespace Utils {
		char getCharCode(char16_t character);

		class LCDString {
			String string;
		
		public:
			LCDString();
			LCDString(const char* str);
			LCDString(const char16_t* str);

			size_t length() const;

			char charAt(size_t i) const;

			char operator[](size_t i) const;

			const char* c_str() const;

			const char* begin() const;

			const char* end() const;
		};
	}
}