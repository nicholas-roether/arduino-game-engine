#ifndef _AGE_H_
#define _AGE_H_

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"
#include "AGE_structures.h"

namespace AGE {
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
	public:
		virtual void draw(LiquidCrystal& lcd);

		virtual bool shouldRedraw();

		virtual void didRedraw();
	};

	class Group : public Component {
		Utils::Array<Component*> children;

		bool didChange = false;

	public:		
		Group() = default;
		Group(size_t initialCapacity);

		void add(Component* child);

		void draw(LiquidCrystal& lcd);

		bool shouldRedraw();

		void didRedraw();
	};

	class Text : public Component {
		Utils::LCDString text;
		uint8_t x;
		uint8_t y;
	
	public:
		Text(const Utils::LCDString& text, uint8_t x, uint8_t y);

		Text(const Text& text);

		void draw(LiquidCrystal& lcd);
	};
}

#endif