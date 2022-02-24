#ifndef _AGE_COMPONENTS_H
#define _AGE_COMPONENTS_H

#include <Arduino.h>
#include "AGE.h"

namespace AGE {
	class Text : public Component {
		Utils::LCDString text;
		uint8_t x;
		uint8_t y;
	
	public:
		Text();

		Text(const Utils::LCDString& text, uint8_t x, uint8_t y);

		Text(const Text& text);

		void draw(CharacterBuffer& buffer);

		void setText(const Utils::LCDString& str);

		void setX(uint8_t x);

		void setY(uint8_t y);

		void setPos(uint8_t x, uint8_t y);
	};

	class Texture : public Component {
		uint8_t textureId;
		uint8_t x;
		uint8_t y;

	public:
		Texture(uint8_t textureId);
		Texture(uint8_t textureId, uint8_t x, uint8_t y);

		void draw(CharacterBuffer& buffer);

		void setTexture(uint8_t texutreId);

		void setX(uint8_t x);

		void setY(uint8_t y);

		void setPos(uint8_t x, uint8_t y);
	};

	class Toggled : public Component {
		Component* child;
		bool showing;
		bool visible;

	public:
		Toggled(Component* child);
		Toggled(Component* child, bool visible);

		void setVisible(bool visible);

		void toggle();

		bool isVisible();

		void build();

		void update(unsigned int dt);
	};
}

#endif