#include "AGE_character_map.h"

namespace AGE::Utils {
	uint8_t getCharCode(char character) {
		switch(character) {
			case 'g': return 0xE7;
			case 'j': return 0xEA;
			case 'p': return 0xF0;
			case 'q': return 0xF1;
		}
		if (character >= ' ' && character <= '}') return character;
		if (character >= 'ア' && character <= 'ト' && character % 2 == 0)
			return 0xB1 + (character - 'ア') / 2;
		if (character >= 'ナ' && character <= 'ノ')
			return 0xC5 + character - 'ナ'; 
		if (character  >= 'ハ' && character <= 'ホ' && character % 3 == 0)
			return 0xCA + (character - 'ハ') / 3;
		if (character >= 'マ' && character <= 'モ')
			return 0xCF + character - 'マ';
		if (character >= 'ヤ' && character <= 'ヨ' && character % 2 == 0)
			return 0xD4 + (character - 'ヤ') / 2;
		if (character >= 'ラ' && character <= 'ロ')
			return 0xD7 + character - 'ラ';
		switch(character) {
			case '→': return 0x7E;
			case '←': return 0x7F;
			case '　': return 0xA0;
			case '。': return 0xA1;
			case '「': return 0xA2;
			case '」': return 0xA3;
			case '、': return 0xA4;
			case '・': return 0xA5;
			case '∙': return 0xA5;
			case '⋅': return 0xA5;
			case 'ヲ': return 0xA6;
			case 'ァ': return 0xA7;
			case 'ィ': return 0xA8;
			case 'ゥ': return 0xA9;
			case 'ェ': return 0xAA;
			case 'ォ': return 0xAB;
			case 'ャ': return 0xAC;
			case 'ュ': return 0xAD;
			case 'ョ': return 0xAE;
			case 'ッ': return 0xAF;
			case 'ー': return 0xB0;
			case 'ワ': return 0xDC;
			case 'ン': return 0xDD;
			case '゛': return 0xDE;
			case '゜': return 0xDF;
			case '°': return 0xDF;
			case 'α': return 0xE0;
			case 'Ä': return 0xE1;
			case 'ä': return 0xE1;
			case 'β': return 0xE2;
			case 'ß': return 0xE2;
			case 'ε': return 0xE3;
			case 'μ': return 0xE4;
			case 'σ': return 0xE5;
			case 'ρ': return 0xE6;
			case '√': return 0xE8;
			case '⁻': return 0xE9;
			case 'ˣ': return 0xEB;
			case '¢': return 0xEC;
			case '₵': return 0xEC;
			case '£': return 0xED;
			case 'Ñ': return 0xEE;
			case 'ñ': return 0xEE;
			case 'Ö': return 0xEF;
			case 'ö': return 0xEF;
			case 'θ': return 0xF2;
			case '∞': return 0xF3;
			case 'Ω': return 0xF4;
			case 'Ü': return 0xF5;
			case 'ü': return 0xF5;
			case 'Σ': return 0xF6;
			case 'π': return 0xF7;
			case 'x̄': return 0xF8;
			case '千': return 0xFA;
			case '万': return 0xFB;
			case '円': return 0xFE;
			case '÷': return 0xFD;
			case '∎': return 0xFF;
		}
		return 0xFF;
	}

	void strToLCDEncoding(String& string) {
		for (char& character : string)
			character = getCharCode(character);
	}
}