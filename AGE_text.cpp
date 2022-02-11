#include "AGE_text.h"

namespace AGE::Utils {
	size_t strlen16(const char16_t* string) {
		size_t size = 0;
		while(string[size] != 0x0000) size++;
		return size;
	}

	size_t strlen32(const char32_t* string) {
		size_t size = 0;
		while(string[size] != 0x00000000) size++;
		return size;
	}

	void* allocAndCopy(const void* source, size_t capacity, size_t length) {
		void* ptr = malloc(capacity);
		memcpy(ptr, source, length);
		return ptr;
	}

	UnicodeString::UnicodeString()
		: charSize(sizeof(char)), capacity(0), len(0), charPtr(nullptr) {}

	UnicodeString::UnicodeString(const char* str)
		: charSize(sizeof(char)), len(strlen(str)), capacity(len)
	{
		charPtr = (char*) allocAndCopy(str, capacity * charSize, len * charSize);
	}

	UnicodeString::UnicodeString(const char16_t* str)
		: charSize(sizeof(char16_t)), len(strlen16(str)), capacity(len)
	{
		charPtr = (char*) allocAndCopy(str, capacity * charSize, len * charSize);
	}

	UnicodeString::UnicodeString(const char32_t* str)
		: charSize(sizeof(char32_t)), len(strlen32(str)), capacity(len)
	{
		charPtr = (char*) allocAndCopy(str, capacity * charSize, len * charSize);
	}

	UnicodeString::UnicodeString(const String& str) : UnicodeString(str.c_str()) {}

	UnicodeString::UnicodeString(const UnicodeString& str)
		: charSize(str.charSize), len(str.len), capacity(str.capacity)
	{
		charPtr = (char*) allocAndCopy(str.charPtr, capacity * charSize, len * charSize);
	}

	UnicodeString::~UnicodeString() {
		free(charPtr);
	}
	
	size_t UnicodeString::length() const {
		return len;
	}

	size_t UnicodeString::getCharSize() const {
		return charSize;
	}

	char32_t UnicodeString::charAt(size_t i) const {
		if (i >= len) return 0x00000000;
		char32_t character = 0;
		for (int j = 0; j < charSize; j++) character += charPtr[i * charSize + j] << (charSize - j);
		return character;
	}

	void UnicodeString::setCharAt(size_t i, const char32_t& character) {
		if (i >= len) return;
		memcpy(charPtr + i * charSize, &character, charSize);
	}

	const char* UnicodeString::c_str() const {
		return charPtr;
	}

	char getCharCode(char32_t character) {
		Serial.print((char) character);
		switch(character) {
			case 'g': return 0xE7;
			case 'j': return 0xEA;
			case 'p': return 0xF0;
			case 'q': return 0xF1;
		}
		if (character >= ' ' && character <= '}') return character;
		if (character >= L'ア' && character <= L'ト' && character % 2 == 0)
			return 0xB1 + (character - L'ア') / 2;
		if (character >= L'ナ' && character <= L'ノ')
			return 0xC5 + character - L'ナ'; 
		if (character  >= L'ハ' && character <= L'ホ' && character % 3 == 0)
			return 0xCA + (character - L'ハ') / 3;
		if (character >= L'マ' && character <= L'モ')
			return 0xCF + character - L'マ';
		if (character >= L'ヤ' && character <= L'ヨ' && character % 2 == 0)
			return 0xD4 + (character - L'ヤ') / 2;
		if (character >= L'ラ' && character <= L'ロ')
			return 0xD7 + character - L'ラ';
		switch(character) {
			case L'→': return 0x7E;
			case L'←': return 0x7F;
			case L'　': return 0xA0;
			case L'。': return 0xA1;
			case L'「': return 0xA2;
			case L'」': return 0xA3;
			case L'、': return 0xA4;
			case L'・': return 0xA5;
			case L'∙': return 0xA5;
			case L'⋅': return 0xA5;
			case L'ヲ': return 0xA6;
			case L'ァ': return 0xA7;
			case L'ィ': return 0xA8;
			case L'ゥ': return 0xA9;
			case L'ェ': return 0xAA;
			case L'ォ': return 0xAB;
			case L'ャ': return 0xAC;
			case L'ュ': return 0xAD;
			case L'ョ': return 0xAE;
			case L'ッ': return 0xAF;
			case L'ー': return 0xB0;
			case L'ワ': return 0xDC;
			case L'ン': return 0xDD;
			case L'゛': return 0xDE;
			case L'゜': return 0xDF;
			case L'°': return 0xDF;
			case L'α': return 0xE0;
			case L'Ä': return 0xE1;
			case L'ä': return 0xE1;
			case L'β': return 0xE2;
			case L'ß': return 0xE2;
			case L'ε': return 0xE3;
			case L'μ': return 0xE4;
			case L'σ': return 0xE5;
			case L'ρ': return 0xE6;
			case L'√': return 0xE8;
			case L'⁻': return 0xE9;
			case L'ˣ': return 0xEB;
			case L'¢': return 0xEC;
			case L'₵': return 0xEC;
			case L'£': return 0xED;
			case L'Ñ': return 0xEE;
			case L'ñ': return 0xEE;
			case L'Ö': return 0xEF;
			case L'ö': return 0xEF;
			case L'θ': return 0xF2;
			case L'∞': return 0xF3;
			case L'Ω': return 0xF4;
			case L'Ü': return 0xF5;
			case L'ü': return 0xF5;
			case L'Σ': return 0xF6;
			case L'π': return 0xF7;
			case L'千': return 0xFA;
			case L'万': return 0xFB;
			case L'円': return 0xFE;
			case L'÷': return 0xFD;
			case L'∎': return 0xFF;
		}
		return 0xFF;
	}

	void strToLCDEncoding(const Utils::UnicodeString& string, String& target) {
		target = "";
		target.reserve(string.length());
		for (unsigned int i = 0; i < string.length(); i++) {
			char32_t c = string.charAt(i);
			target += getCharCode(c);
		}
	}
}