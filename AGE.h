#include <vector>

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
	};

	class CharacterBuffer {
	private:
		std::size_t width;
		std::size_t height;
		std::vector<std::vector<Character>> buffer;

	public:
		CharacterBuffer(std::size_t width, std::size_t height);

		Character get(std::size_t x, std::size_t y);

		void set(std::size_t x, std::size_t y, Character character);
	};

	class CharacterRenderer {
	private:
		CharacterBuffer* charBuffer;
		CharacterBuffer lastCharBuffer;

	public:
		CharacterRenderer(CharacterBuffer* charBuffer);

		void render();
	};
}