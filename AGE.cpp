#include <Arduino.h>
#include <alloca.h>

#include "AGE.h"

namespace AGE {
	constexpr size_t CHARACTER_SIZE = sizeof(CharacterType) + sizeof(char);

	Character::Character(CharacterType type, char value)
		: type(type), value(value) {}

	bool Character::operator==(Character other) {
		return type == other.type && value == other.value;
	}

	bool Character::operator!=(Character other) {
		return !operator==(other);
	}

	CharacterBuffer::CharacterBuffer(size_t width, size_t height)
		: width(width), height(height), buffer((Character*) calloc(width * height, sizeof(Character)))
	{}

	size_t CharacterBuffer::getBufferIndex(size_t x, size_t y, bool& valid) const {
		if (x >= width || y >= height) {
			valid = false;
			return 0;
		}
		valid = true;
		return x + y * width;
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
		for (size_t y = 0; y < charBuffer.height; y++) {
			for (size_t x = 0; x < charBuffer.width; x++) {
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

	Component::Component(size_t x = 0, size_t y = 0) : x(x), y(y) {}

	void Component::update(uint16_t dt) {}

	void Component::draw(CharacterBuffer& charBuffer, size_t xOffs = 0, size_t yOffs = 0) {}

	LetterComponent::LetterComponent(char letter, size_t x = 0, size_t y = 0)
		: Component(x, y), letter(letter) {}

	void LetterComponent::draw(CharacterBuffer& charBuffer, size_t xOffs = 0, size_t yOffs = 0) {
		charBuffer.set(x + xOffs, y + yOffs, Character(LETTER, letter));
	}

	void LetterComponent::setLetter(char letter) {
		this->letter = letter;
	}

	TextureComponent::TextureComponent(char texture, size_t x = 0, size_t y = 0)
		: Component(x, y), texture(texture) {}

	void TextureComponent::draw(CharacterBuffer& charBuffer, size_t xOffs = 0, size_t yOffs = 0) {
		charBuffer.set(x + xOffs, y + yOffs, Character(TEXTURE, texture));
	}
}