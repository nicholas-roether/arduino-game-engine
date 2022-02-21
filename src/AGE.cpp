#include "AGE.h"

namespace AGE {
	// Event manager
	void EventManager::pushCallbackList() {
		Utils::Array<void(*)()> callbackList;
		callbackListBuffer.push(callbackList);
	}

	EventManager::EventManager() : callbackListBuffer() {}

	void EventManager::on(Event event, void(*callback)()) {
		if (event >= callbackListBuffer.size()) return;
		callbackListBuffer[event].push(callback);
	}

	void EventManager::dispatch(Event event) {
		if (event >= callbackListBuffer.size()) return;
		for (void(*callback)() : callbackListBuffer[event])
			callback();
	}

	Event EventManager::newEvent() {
		pushCallbackList();
		return callbackListBuffer.size() - 1;
	}

	// Group
	Group::Group(size_t initialCapacity)
		: childPtrs(initialCapacity), childrenBuffer(initialCapacity * sizeof(Component)) {}

	void Group::draw(LiquidCrystal& lcd) {
		Serial.println("draw");
		for (Component* cPtr : childPtrs) {
			Serial.print("Component at: ");
			Serial.println((unsigned int) cPtr);
			delay(100);
			cPtr->draw(lcd);
		}
	}

	void Group::didRedraw() {
		didChange = false;
	}

	bool Group::shouldRedraw() {
		return didChange;
	}

	// Component

	void Component::draw(LiquidCrystal& lcd) {
		Serial.println("Component::draw");
		delay(100);
	}

	void Component::didRedraw() {}

	bool Component::shouldRedraw() {
		return false;
	}

	// Text

	Text::Text(const Utils::LCDString& text, uint8_t x, uint8_t y)
		: text(text), x(x), y(y) {}
	
	Text::Text(const Text& text)
		: text(text.text), x(text.x), y(text.y)
	{}

	Text& Text::operator=(const Text& other) {
		text = other.text.c_str();
		x = other.x;
		y = other.y;
		Serial.print("text assignment ");
		Serial.print(x);
		Serial.print(",");
		Serial.println(y);
	}
	
	void Text::draw(LiquidCrystal& lcd) {
		Serial.print("text draw ");
		Serial.print(x);
		Serial.print(",");
		Serial.println(y);
		lcd.setCursor(x, y);
		lcd.write(text.c_str());
	}
}