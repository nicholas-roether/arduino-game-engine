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
	Group::Group(size_t initialCapacity) : children(initialCapacity) {}

	void Group::add(Component* childPtr) {
		children.push(childPtr);
		didChange = true;
	}

	void Group::draw(LiquidCrystal& lcd) {
		for (Component* cPtr : children)
			cPtr->draw(lcd);
	}

	void Group::didRedraw() {
		didChange = false;
	}

	bool Group::shouldRedraw() {
		return didChange;
	}

	// Component

	void Component::draw(LiquidCrystal& lcd) {}

	void Component::didRedraw() {}

	bool Component::shouldRedraw() {
		return false;
	}

	// Text

	Text::Text(const Utils::LCDString& text, uint8_t x, uint8_t y)
		: text(text), x(x), y(y) {}
	
	Text::Text(const Text& text)
		: text(text.text), x(x), y(y)
	{}
	
	void Text::draw(LiquidCrystal& lcd) {
		lcd.setCursor(x, y);
		lcd.write(text.c_str());
	}
}