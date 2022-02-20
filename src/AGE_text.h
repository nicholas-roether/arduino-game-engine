#ifndef _AGE_TEXT_H_
#define _AGE_TEXT_H_

#include <Arduino.h>

namespace AGE {
	namespace Utils {
		char getCharCode(char16_t character);

		class LCDString : public String {		
		public:
			LCDString();
			LCDString(const char* str);
			LCDString(const char16_t* str);
			LCDString(const LCDString& str);
		};
	}
}

#endif