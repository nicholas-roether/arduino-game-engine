#include "AGE_components.h"

namespace AGE {
	// Text

	Text::Text()
		: text(""), x(0), y(0) {}

	Text::Text(const Utils::LCDString& text, uint8_t x, uint8_t y)
		: text(text), x(x), y(y) {}
	
	Text::Text(const Text& text)
		: text(text.text), x(text.x), y(text.y)
	{}
	
	void Text::draw(CharacterBuffer& buffer) {
		buffer.write(text.c_str(), x, y);
	}

	void Text::setText(const Utils::LCDString& str) {
		text = str;
	}

	void Text::setX(uint8_t x) {
		this->x = x;
	}

	void Text::setY(uint8_t y) {
		this->y = y;
	}

	void Text::setPos(uint8_t x, uint8_t y) {
		setX(x);
		setY(y);
	}

	// Texture

	Texture::Texture(uint8_t textureId) : textureId(textureId) {}

	Texture::Texture(uint8_t textureId, uint8_t x, uint8_t y)
		: textureId(textureId), x(x), y(y) {}

	void Texture::draw(CharacterBuffer& buffer) {
		buffer.put(textureId, x, y);
	}

	void Texture::setTexture(uint8_t textureId) {
		this->textureId = textureId;
	}

	void Texture::setX(uint8_t x) {
		this->x = x;
	}

	void Texture::setY(uint8_t y) {
		this->y = y;
	}

	void Texture::setPos(uint8_t x, uint8_t y) {
		setX(x);
		setY(y);
	}

	// Toggled

	Toggled::Toggled(Component* child)
		: child(child), showing(true), visible(true) {}

	Toggled::Toggled(Component* child, bool visible)
		: child(child), showing(visible), visible(visible) {}

	void Toggled::setVisible(bool visible) {
		this->visible = visible;
	}

	void Toggled::toggle() {
		setVisible(!visible);
	}

	bool Toggled::isVisible() {
		return visible;
	}

	void Toggled::build() {
		if (showing) addChild(child);
	}

	void Toggled::update(unsigned int dt) {
		if (showing != visible) {
			showing = visible;
			requestRebuild();
		}
	}
}