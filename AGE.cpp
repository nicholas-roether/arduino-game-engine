#include <Arduino.h>
#include <alloca.h>

#include "AGE.h"

namespace AGE {
	Character::Character(CharacterType type, char value)
		: type(type), value(value) {}

	bool Character::operator==(Character other) {
		return type == other.type && value == other.value;
	}

	bool Character::operator!=(Character other) {
		return !operator==(other);
	}

	CharacterBuffer::CharacterBuffer(size_t width, size_t height)
		: width(width), height(height), buffer((Character*) alloca(width * height * sizeof(Character)))
	{}

	size_t CharacterBuffer::getBufferIndex(size_t x, size_t y, bool& valid) const {
		if (x >= width || y >= height) {
			valid = false;
			return 0;
		}
		valid = true;
		return x % width + y * width;
	}

	Character CharacterBuffer::get(size_t x, size_t y) const {
		bool valid;
		size_t index = getBufferIndex(x, y, valid);
		if (valid) return buffer[index];
		return Character();
	}

	void CharacterBuffer::set(size_t x, size_t y, Character character) {
		bool valid;
		size_t index = getBufferIndex(x, y, valid);
		if (valid) buffer[index] = character;
	}

	CharacterRenderer::CharacterRenderer(LiquidCrystal& lcd, const CharacterBuffer& charBuffer)
		: lcd(lcd), charBuffer(charBuffer), lastCharBuffer(charBuffer.width, charBuffer.height)
	{}
	
	void CharacterRenderer::render() {
		for (size_t x = 0; x < charBuffer.width; x++) {
			for (size_t y = 0; y < charBuffer.height; y++) {
				Character current = charBuffer.get(x, y);
				Character previous = lastCharBuffer.get(x, y);
				if (current != previous) {
					lcd.setCursor(x, y);
					switch (current.type) {
					case EMPTY:
						lcd.print(" ");
						break;
					case LETTER:
						lcd.print(current.value);
						break;
					case TEXTURE:
						lcd.write(current.value);
					}
					lastCharBuffer.set(x, y, current);
				}
			}
		}
		lcd.setCursor(0, 0);
	}
}