#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"

namespace AGE {
	class Component {
	public:
		virtual void draw(const LiquidCrystal& lcd);

		virtual bool shouldRedraw();

		virtual void didRedraw();
	};

	class Group : public Component{
		size_t capacity;
		size_t numChildren;
		Component* buffer;

		void increaseCapacity();

	public:
		void add(const Component& child);

		void draw(const LiquidCrystal& lcd);

		bool shouldRedraw();

		void didRedraw();
	};
}