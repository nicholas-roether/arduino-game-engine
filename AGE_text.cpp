#include "AGE_text.h"

namespace AGE::Utils {
	size_t strlen16(const char16_t* string) {
		size_t size = 0;
		while(string[size] != 0x0000) size++;
		return size;
	}

	// UnicodeString::UnicodeString()
	// 	: capacity(0), len(0), charPtr(nullptr) {}

	// UnicodeString::UnicodeString(const char* str)
	// 	: len(strlen(str)), capacity(len + 1)
	// {
	// 	charPtr = (char16_t*) calloc(capacity, sizeof(char16_t));
	// 	for (unsigned int i = 0; i <= len; i++) charPtr[i] = str[i];
	// }

	// UnicodeString::UnicodeString(const char16_t* str)
	// 	: len(strlen16(str)), capacity(len + 1)
	// {
	// 	charPtr = (char16_t*) calloc(capacity, sizeof(char16_t));
	// 	for (unsigned int i = 0; i <= len; i++) charPtr[i] = str[i];
	// }

	// UnicodeString::UnicodeString(const UnicodeString& str)
	// 	: len(str.len), capacity(str.capacity)
	// {
	// 	charPtr = (char16_t*) calloc(capacity, sizeof(char16_t));
	// 	for (unsigned int i = 0; i <= len; i++) charPtr[i] = str.charAt(i);
	// }

	// UnicodeString::~UnicodeString() {
	// 	free(charPtr);
	// }
	
	// size_t UnicodeString::length() const {
	// 	return len;
	// }

	// char16_t UnicodeString::charAt(size_t i) const {
	// 	if (i >= len) return 0x00000000;
	// 	return charPtr[i];
	// }

	// void UnicodeString::setCharAt(size_t i, const char16_t& character) {
	// 	if (i >= len) return;
	// 	charPtr[i] = character;
	// }

	// const char* UnicodeString::c_str() const {
	// 	return (const char*) charPtr;
	// }

	// const char16_t* UnicodeString::charArray() const {
	// 	return charPtr;
	// }

	char getCharCode(char16_t character) {
		Serial.print((char) character);
		switch(character) {
			case 'g': return 0xE7;
			case 'j': return 0xEA;
			case 'p': return 0xF0;
			case 'q': return 0xF1;
		}
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
		string.reserve(len);
		for (unsigned int i = 0; i < len; i++) string += getCharCode(str[i]);
	}
	
	LCDString::LCDString(const char16_t* str) {
		size_t len = strlen16(str);
		string.reserve(len);
		for (unsigned int i = 0; i < len; i++) string += getCharCode(str[i]);
	}

	LCDString::LCDString(const LCDString& str) {
		string = str.string;
	}

	size_t LCDString::length() const {
		return string.length();
	}

	char LCDString::charAt(size_t i) const {
		return string.charAt(i);
	}

	char LCDString::operator[](size_t i) const {
		return charAt(i);
	}

	const char* LCDString::c_str() const {
		return string.c_str();
	}

	const char* LCDString::begin() const {
		return string.begin();
	}

	const char* LCDString::end() const {
		return string.end();
	}

	// void strToLCDEncoding(const Utils::UnicodeString& string, String& target) {
	// 	target = "";
	// 	target.reserve(string.length());
	// 	for (unsigned int i = 0; i < string.length(); i++) {
	// 		char16_t c = string.charAt(i);
	// 		target += getCharCode(c);
	// 	}
	// }
}