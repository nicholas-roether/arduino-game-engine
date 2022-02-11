#include "AGE_text.h"

namespace AGE::Utils {
	uint8_t getCharCode(char16_t character) {
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

	void strToLCDEncoding(const char16_t* string, String& target) {
		size_t length = (strlen((const char*) string) + 1) / 2;
		for (unsigned int i = 0; i < length / 2; i++) {
			char16_t c = string[i];
			target.setCharAt(i, getCharCode(c));
		}
	}
}