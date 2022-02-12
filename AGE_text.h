#pragma once

#include <Arduino.h>

namespace AGE {
	namespace Utils {
		class UnicodeString {
			size_t capacity;
			size_t len;
			char16_t* charPtr;
		
		public:
			UnicodeString();
			UnicodeString(const char* str);
			UnicodeString(const char16_t* str);
			UnicodeString(const UnicodeString& str);

			virtual ~UnicodeString();

			size_t length() const;

			char16_t charAt(size_t i) const;

			void setCharAt(size_t i, const char16_t& character);

			const char* c_str() const;

			const char16_t* charArray() const;
		};

		char getCharCode(char32_t character);

		void strToLCDEncoding(const UnicodeString& string, String& target);
	}
}