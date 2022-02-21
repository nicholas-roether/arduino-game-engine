#ifndef _AGE_H_
#define _AGE_H_

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"
#include "AGE_structures.h"

namespace AGE {
	class CharacterBuffer;

	typedef unsigned int Event;

	class EventManager {
		Utils::Array<Utils::Array<void(*)()>> callbackListBuffer;

		void pushCallbackList();

	public:
		EventManager();

		void on(Event event, void(*callback)());

		void dispatch(Event event);

		Event newEvent();
	};

	class Component {
		Utils::Array<Component*> children;
		bool stateDidChange = false;
	
	protected:
		void addChild(Component* child);

		void setState();

	public:
		const Utils::Array<Component*>& getChildren();

		void rebuild();

		virtual void update(unsigned int dt);

		virtual void draw(CharacterBuffer& buffer);

		virtual void build();

		bool shouldRebuild();

		void didRebuild();
	};

	class Text : public Component {
		Utils::LCDString text;
		uint8_t x;
		uint8_t y;
	
	public:
		Text();

		Text(const Utils::LCDString& text, uint8_t x, uint8_t y);

		Text(const Text& text);

		void draw(CharacterBuffer& registry);

		void setText(const Utils::LCDString& str);

		void setX(uint8_t x);

		void setY(uint8_t y);

		void setPos(uint8_t x, uint8_t y);
	};

	class CharacterBuffer {
		size_t width;
		size_t height;
		char* characters;

	public:
		CharacterBuffer() = delete;
		CharacterBuffer(size_t width, size_t height);
		CharacterBuffer(const CharacterBuffer& other);

		~CharacterBuffer();

		CharacterBuffer& operator=(const CharacterBuffer& other);

		char get(unsigned int x, unsigned int y);

		void put(char character, unsigned int x, unsigned int y);

		void write(const char* characters, unsigned int x, unsigned int y);

		char* begin();

		char* end();

		size_t getWidth();

		size_t getHeight();
	};

	class Renderer {
		CharacterBuffer charBuffer;
		CharacterBuffer prevCharBuffer;
		Component* root;
		unsigned int lastRender = 0;
		bool firstBuild = true;

		void build(Component* component);

		void update(Component* component, unsigned int dt);

		void render(Component* component);

	public:
		Renderer() = delete;
		Renderer(size_t width, size_t height);

		void setRoot(Component* root);

		void render(LiquidCrystal& lcd);
	};
}

#endif