#ifndef AGE_H
#define AGE_H

#define DEBUG_LOG(str)

#ifdef AGE_DEBUG
#define DEBUG_LOG(str) Serial.println(str)
#endif

#define THROW(id) DEBUG_LOG("An error with id " + String(id) + " occurred."); throw id

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"
#include "AGE_structures.h"

namespace AGE {
	class EventManager {
		Utils::Array<Utils::Array<void(*)()>> callbackListBuffer;

		void pushCallbackList();

	public:
		EventManager();

		void on(uint16_t eventId, void(*callback)());

		void dispatch(uint16_t eventId);

		uint16_t newEvent();
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