#include <Arduino.h>

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

	CharacterBuffer::~CharacterBuffer() {
		free(buffer);
	}

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

	Component::Component(size_t x, size_t y) : x(x), y(y) {}

	void Component::update(uint16_t dt) {}

	void Component::draw(CharacterBuffer& charBuffer, size_t xOffs, size_t yOffs) const {}

	LetterComponent::LetterComponent(char letter, size_t x, size_t y)
		: Component(x, y), letter(letter) {}

	void LetterComponent::draw(CharacterBuffer& charBuffer, size_t xOffs, size_t yOffs) const {
		charBuffer.set(x + xOffs, y + yOffs, Character(LETTER, letter));
	}

	void LetterComponent::setLetter(char letter) {
		this->letter = letter;
	}

	TextureComponent::TextureComponent(char texture, size_t x, size_t y)
		: Component(x, y), texture(texture) {}

	void TextureComponent::draw(CharacterBuffer& charBuffer, size_t xOffs, size_t yOffs) const {
		charBuffer.set(x + xOffs, y + yOffs, Character(TEXTURE, texture));
	}

	GroupComponent::GroupComponent(size_t initialCapacity = 5, size_t x, size_t y)
		: Component(x, y), capacity(capacity), children((Component*) calloc(capacity, sizeof(Component)))
	{}

	GroupComponent::~GroupComponent() {
		free(children);
	}

	void GroupComponent::add(const Component& child) {
		if (numChildren == capacity - 1) increaseCapacity();
		children[numChildren] = child;
		numChildren++;
	}

	void GroupComponent::increaseCapacity() {
		capacity *= 2;
		Component* newChildren = (Component*) calloc(capacity, sizeof(Component));
		memcpy(newChildren, children, numChildren * sizeof(Component));
		free(children);
		children = newChildren;
	}

	void GroupComponent::draw(CharacterBuffer& charBuffer, size_t xOffs, size_t yOffs) const {
		for (size_t i = 0; i < numChildren; i++) children[i].draw(charBuffer, x + i + xOffs, y + yOffs);
	}

	TextComponent::TextComponent(const String& str, size_t x, size_t y) : GroupComponent(str.length(), x, y) {
		for (size_t i = 0; i < str.length(); i++)
			add(LetterComponent(str[i], x, y));
	}
}