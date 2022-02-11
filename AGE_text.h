#pragma once

#include <Arduino.h>

namespace AGE {
	namespace Utils {
		class UnicodeString {
			const size_t charSize;
			size_t capacity;
			size_t len;
			char* charPtr;
		
		public:
			UnicodeString();
			UnicodeString(const char* str);
			UnicodeString(const char16_t* str);
			UnicodeString(const char32_t* str);
			UnicodeString(const String& str);
			UnicodeString(const UnicodeString& str);

			virtual ~UnicodeString();

			size_t length() const;

			size_t getCharSize() const;

			char32_t charAt(size_t i) const;

			void setCharAt(size_t i, const char32_t& character);

			const char* c_str() const;
		};

		char getCharCode(char32_t character);

		void strToLCDEncoding(const UnicodeString& string, String& target);
	}
}