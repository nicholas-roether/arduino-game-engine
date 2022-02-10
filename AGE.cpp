#include "AGE.h"

namespace AGE {
	CharacterBuffer::CharacterBuffer(std::size_t width, std::size_t height)
		: width(width), height(height) {}

	Character CharacterBuffer::get(std::size_t x, std::size_t y)  {
		if (x >= width || y >= height) throw OUT_OF_BOUNDS;
		return buffer[x][y];
	}

	void CharacterBuffer::set(std::size_t x, std::size_t y, Character character) {
		if (x >= width || y >= height) throw OUT_OF_BOUNDS;
		buffer[x][y] = character;
	}

	CharacterRenderer::CharacterRenderer(CharacterBuffer* charBuffer)
		: charBuffer(charBuffer) {}
	
	void CharacterRenderer::render() {
		// TODO implement rendering
	}
}