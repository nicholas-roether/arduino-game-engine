#pragma once

#include <stddef.h>
#include <LiquidCrystal.h>

namespace AGE {
	enum CharacterType {
		EMPTY,
		LETTER,
		TEXTURE
	};

	struct Character {
		CharacterType type = EMPTY;
		char value = 0;

		Character() = default;
		Character(CharacterType type, char value);

		bool operator==(Character other);
		bool operator!=(Character other);
	};

	class CharacterBuffer {
		Character* buffer;

		size_t getBufferIndex(size_t x, size_t y, bool& valid) const;

	public:
		const size_t width;
		const size_t height;

		CharacterBuffer(size_t width, size_t height);

		~CharacterBuffer();

		Character get(size_t x, size_t y) const;

		void set(size_t x, size_t y, Character character);
	};

	class CharacterRenderer {
		LiquidCrystal& lcd;
		const CharacterBuffer& charBuffer;
		CharacterBuffer lastCharBuffer;

	public:
		CharacterRenderer(LiquidCrystal& lcd, const CharacterBuffer& charBuffer);

		void render();
	};

	class Component {
	protected:
		size_t x;
		size_t y;

		Component(size_t x = 0, size_t y = 0);

	public:
		virtual void update(uint16_t dt);

		virtual void draw(CharacterBuffer& charBuffer, size_t xOffs = 0, size_t yOffs = 0) const;

		size_t getX() const;

		size_t getY() const;
	};

	class LetterComponent : public Component {
		char letter;

	public:
		LetterComponent(char letter, size_t x = 0, size_t y = 0);

		void draw(CharacterBuffer& charBuffer, size_t xOffs = 0, size_t yOffs = 0) const;

		void setLetter(char letter);
	};

	class TextureComponent : public Component {
		char texture;

	public:
		TextureComponent(char texture, size_t x = 0, size_t y = 0);

		void draw(CharacterBuffer& charBuffer, size_t xOffs = 0, size_t yOffs = 0) const;

		void setTexture(char texture);
	};

	class GroupComponent : public Component {
	private:
		size_t capacity;
		size_t numChildren = 0;
		Component* children;

		void increaseCapacity();

	protected:
		void add(const Component& child);

	public:
		GroupComponent(size_t initialCapacity = 5);

		~GroupComponent();

		virtual void draw(CharacterBuffer& charBuffer, size_t xOffs = 0, size_t yOffs = 0) const;
	};

	class TextComponent : public GroupComponent {
	public:
		TextComponent(const String& str);
	};
}