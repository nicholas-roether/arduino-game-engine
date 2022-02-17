#include "AGE.h"
#include "AGE_text.h"

namespace AGE {
	// Event manager
	void EventManager::pushCallbackList() {
		callbackListBuffer.push(Array<void(*) ()>());
	}

	void EventManager::on(uint16_t eventId, void(*callback) ()) {
		if (eventId >= callbackListBuffer.size()) return;
		Array<void(*) ()> list = callbackListBuffer[eventId];
		list.push(callback);
	}

	void EventManager::dispatch(uint16_t eventId) {
		if (eventId >= callbackListBuffer.size()) return;
		for (void(*callback)() : callbackListBuffer[eventId])
			(*callback)();
	}

	uint16_t EventManager::newEvent() {
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