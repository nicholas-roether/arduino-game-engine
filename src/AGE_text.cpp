#include "AGE_text.h"

namespace AGE::Utils {
	size_t strlen16(const char16_t* string) {
		size_t size = 0;
		while(string[size] != 0x0000) size++;
		return size;
	}

	char getCharCode(char16_t character) {
		if (character >= ' ' && character <= '}') return character;
		if (character >= u'ア' && character <= u'オ' && character % 2 == 0)
			return 0xB1 + (character - u'ア') / 2;
		if (character >= u'カ' && character <= u'チ' && character % 2 == 1)
			return 0xB6 + (character - u'カ') / 2;
		if (character >= u'ツ' && character <= u'ト' && character % 2 == 0)
			return 0xC2 + (character - u'ツ') / 2;
		if (character >= u'ナ' && character <= u'ノ')
			return 0xC5 + character - u'ナ'; 
		if (character  >= u'ハ' && character <= u'ホ' && character % 3 == 0)
			return 0xCA + (character - u'ハ') / 3;
		if (character >= u'マ' && character <= u'モ')
			return 0xCF + character - u'マ';
		if (character >= u'ヤ' && character <= u'ヨ' && character % 2 == 0)
			return 0xD4 + (character - u'ヤ') / 2;
		if (character >= u'ラ' && character <= u'ロ')
			return 0xD7 + character - u'ラ';
		switch(character) {
			case u'→': return 0x7E;
			case u'←': return 0x7F;
			case u'　': return 0xA0;
			case u'。': return 0xA1;
			case u'「': return 0xA2;
			case u'」': return 0xA3;
			case u'、': return 0xA4;
			case u'・': return 0xA5;
			case u'∙': return 0xA5;
			case u'⋅': return 0xA5;
			case u'ヲ': return 0xA6;
			case u'ァ': return 0xA7;
			case u'ィ': return 0xA8;
			case u'ゥ': return 0xA9;
			case u'ェ': return 0xAA;
			case u'ォ': return 0xAB;
			case u'ャ': return 0xAC;
			case u'ュ': return 0xAD;
			case u'ョ': return 0xAE;
			case u'ッ': return 0xAF;
			case u'ー': return 0xB0;
			case u'ワ': return 0xDC;
			case u'ン': return 0xDD;
			case u'゛': return 0xDE;
			case u'゜': return 0xDF;
			case u'°': return 0xDF;
			case u'α': return 0xE0;
			case u'Ä': return 0xE1;
			case u'ä': return 0xE1;
			case u'β': return 0xE2;
			case u'ß': return 0xE2;
			case u'ε': return 0xE3;
			case u'μ': return 0xE4;
			case u'σ': return 0xE5;
			case u'ρ': return 0xE6;
			case u'√': return 0xE8;
			case u'⁻': return 0xE9;
			case u'ˣ': return 0xEB;
			case u'¢': return 0xEC;
			case u'₵': return 0xEC;
			case u'£': return 0xED;
			case u'Ñ': return 0xEE;
			case u'ñ': return 0xEE;
			case u'Ö': return 0xEF;
			case u'ö': return 0xEF;
			case u'θ': return 0xF2;
			case u'∞': return 0xF3;
			case u'Ω': return 0xF4;
			case u'Ü': return 0xF5;
			case u'ü': return 0xF5;
			case u'Σ': return 0xF6;
			case u'π': return 0xF7;
			case u'千': return 0xFA;
			case u'万': return 0xFB;
			case u'円': return 0xFE;
			case u'÷': return 0xFD;
			case u'∎': return 0xFF;
		}
		return 0xFF;
	}

	LCDString::LCDString() {}

	LCDString::LCDString(const char* str) {
		size_t len = strlen(str);
		reserve(len);
		for (unsigned int i = 0; i < len; i++) operator+=(getCharCode(str[i]));
	}
	
	LCDString::LCDString(const char16_t* str) {
		size_t len = strlen16(str);
		reserve(len);
		for (unsigned int i = 0; i < len; i++) operator+=(getCharCode(str[i]));
	}

	LCDString::LCDString(const String& str)
		: String::String(str) {}

	LCDString::LCDString(const LCDString& str)
		: String::String(str.c_str()) {}
}