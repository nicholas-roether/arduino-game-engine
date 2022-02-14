#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"

namespace AGE {
	template<typename T>
	class Array {
		size_t capacity;
		size_t size;
		T* elements;

		void increaseCapacity() {
			size_t cap = 2 * capacity;
			if (cap == 0) cap = 5;
			T* newElements = calloc(cap, sizeof(T));
			memcpy(newElements, elements, cap * sizeof(T));
			capacity = cap;
			if (size >= capacity) size = capacity;
			free(elements);
			elements = newElements;
		}

	public:
		Array()	: capacity(0), size(0), elements(nullptr) {}
		Array(size_t capacity)
			: capacity(capacity), size(0), elements(calloc(capacity, sizeof(T))) {}

		~Array() {
			free(elements);
		}

		void push(const T& elem) {
			if (size >= capacity) increaseCapacity();
			elements[size] = elem;
			size++;
		}

		T pop() {
			T elem = elements[size - 1];
			delete elements[size - 1];
			size--;
			return elem;
		}

		T& at(size_t i) const {
			// TODO das ist dumm.
			if (i >= size) return (T) 0;
			return elements[i];
		}

		size_t size() const {
			return size;
		}

		T* begin() const {
			return elements;
		}

		T* end() const {
			return elements + size;
		}

		T& operator[](size_t i) const {
			return at(i);
		}
	};

	class EventManager {
		Array<Array<void*>> callbackListBuffer;

		void pushCallbackList();

	public:
		void on(uint16_t eventId, const void* callback);

		void dispatch(uint16_t eventId);

		uint16_t newEvent();
	};

	class Component {
	public:
		virtual void draw(const LiquidCrystal& lcd);

		virtual bool shouldRedraw();

		virtual void didRedraw();
	};

	class Group : public Component {
		Array<Component> buffer;

		bool didChange = false;

	public:
		void add(const Component& child);

		void draw(const LiquidCrystal& lcd);

		bool shouldRedraw();

		void didRedraw();
	};
}