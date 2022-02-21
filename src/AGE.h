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
	
	protected:
		void addChild(Component* child);

	public:
		const Utils::Array<Component*>& getChildren();

		void rebuild();

		virtual void draw(CharacterBuffer& buffer);

		virtual void build();

		virtual bool shouldRebuild();

		virtual void didRebuild();
	};

	class Group : public Component {
		Utils::Array<Component*> childPtrs;

		bool didChange = false;

	public:		
		Group() = default;
		Group(size_t initialCapacity);

		void add(Component* child);

		void build();

		bool shouldRebuild();

		void didRebuild();
	};

	class Text : public Component {
		Utils::LCDString text;
		uint8_t x;
		uint8_t y;
	
	public:
		Text(const Utils::LCDString& text, uint8_t x, uint8_t y);

		Text(const Text& text);

		Text& operator=(const Text& other);

		void draw(CharacterBuffer& registry);
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
		bool firstBuild = true;

		void build(Component* component);

		void render(Component* component);

	public:
		Renderer() = delete;
		Renderer(size_t width, size_t height);

		void setRoot(Component* root);

		void render(LiquidCrystal& lcd);
	};
}

#endif