#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"

namespace AGE {
	template<typename T>
	class Array {
		size_t capacity;
		size_t numElems;
		T* elements;

		void increaseCapacity() {
			size_t cap = 2 * capacity;
			if (cap == 0) cap = 5;
			T* newElements = (T*) calloc(cap, sizeof(T));
			memcpy(newElements, elements, cap * sizeof(T));
			capacity = cap;
			if (numElems >= capacity) numElems = capacity;
			free(elements);
			elements = newElements;
		}

	public:
		Array()	: capacity(0), numElems(0), elements(nullptr) {}
		Array(size_t capacity)
			: capacity(capacity), numElems(0), elements(calloc(capacity, sizeof(T))) {}

		~Array() {
			free(elements);
		}

		void push(const T& elem) {
			if (numElems >= capacity) increaseCapacity();
			elements[numElems] = elem;
			numElems++;
		}

		T pop() {
			T elem = elements[numElems - 1];
			delete elements[numElems - 1];
			numElems--;
			return elem;
		}

		T& at(size_t i) const {
			// TODO das ist dumm.
			if (i >= numElems) throw 0;
			return elements[i];
		}

		size_t size() const {
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
		Array<Array<const void(*) ()>> callbackListBuffer;

		void pushCallbackList();

	public:
		void on(uint16_t eventId, const void(*callback)());

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
		Array<Component> buffer;

		bool didChange = false;

	public:
		void add(const Component& child);

		void draw(LiquidCrystal& lcd);

		bool shouldRedraw();

		void didRedraw();
	};

	class Text : public Component {
		const Utils::LCDString text;
		const uint8_t x;
		const uint8_t y;
	
	public:
		Text(const Utils::LCDString& text, uint8_t x, uint8_t y);

		void draw(LiquidCrystal& lcd);
	};
}