#ifndef AGE_H
#define AGE_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"

namespace AGE {
	template<class T>
	class Array {
		size_t capacity;
		size_t numElems;
		T* elements;

		void increaseCapacity() {
			resizeTo(capacity == 0 ? 1 : 2 * capacity);
		}

	public:
		Array() : capacity(0), numElems(0), elements(nullptr) {}
		Array(size_t capacity)
			: capacity(capacity), numElems(0), elements((T*) malloc(capacity * sizeof(T))) {}

		~Array() {
			free(elements);
		}

		void push(const T& elem) {
			if (numElems == capacity) increaseCapacity();
			*(elements + numElems++) = elem;
		}

		T pop() {
			return *(elements + --numElems);
		}

		T& at(size_t i) const {
			// TODO proper errors
			// if (i >= numElems) throw 0;
			return elements[i];
		}

		void resizeTo(size_t newCapacity) {
			T* newElems = (T*) realloc(elements, newCapacity);
			if (!newElems) /* throw 0; */ return;
			elements = newElems;
			capacity = newCapacity;		
		}

		size_t size() const  {
			return numElems;
		}

		T* begin() const {
			return elements;
		}

		T* end() const {
			return elements + numElems;
		}

		T& operator[](size_t i) const {
			return at(i);
		}
	};

	class EventManager {
		Array<Array<void(*) ()>> callbackListBuffer;

		void pushCallbackList();

	public:
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
		Array<Component*> children;

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