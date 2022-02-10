#pragma once

#include <stddef.h>
#include <LiquidCrystal.h>

namespace AGE {
	enum Error {
		OUT_OF_BOUNDS
	};

	enum CharacterType {
		EMPTY,
		LETTER,
		TEXTURE
	};

	struct Character {
		CharacterType type = EMPTY;
		char value = 0;

		bool operator==(Character other);
		bool operator!=(Character other);
	};

	class CharacterBuffer {
	private:
		Character* buffer;

		size_t getBufferIndex(size_t x, size_t y) const;

	public:
		const size_t width;
		const size_t height;

		CharacterBuffer(size_t width, size_t height);

		Character get(size_t x, size_t y) const;

		void set(size_t x, size_t y, Character character);
	};

	class CharacterRenderer {
	private:
		LiquidCrystal& lcd;
		const CharacterBuffer& charBuffer;
		CharacterBuffer lastCharBuffer;

	public:
		CharacterRenderer(LiquidCrystal& lcd, const CharacterBuffer& charBuffer);

		void render();
	};
}