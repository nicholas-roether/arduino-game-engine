#include <Arduino.h>

#include "AGE.h"
#include "AGE_text.h"

namespace AGE {
	void EventManager::pushCallbackList() {
		callbackListBuffer.push(Array<const void(*) ()>());
	}

	void EventManager::on(uint16_t eventId, const void(*callback) ()) {
		if (eventId >= callbackListBuffer.size()) return;
		Array<const void(*) ()> list = callbackListBuffer[eventId];
		list.push(callback);
	}

	void EventManager::dispatch(uint16_t eventId) {
		if (eventId >= callbackListBuffer.size()) return;
		for (const void(*callback)() : callbackListBuffer[eventId])
			(*callback)();
	}

	uint16_t EventManager::newEvent() {
		pushCallbackList();
		return callbackListBuffer.size() - 1;
	}

	void Group::add(const Component& child) {
		buffer.push(child);
		didChange = true;
	}

	void Group::draw(const LiquidCrystal& lcd) {
		for (Component child : buffer) child.draw(lcd);
	}

	void Group::didRedraw() {
		didChange = false;
	}

	bool Group::shouldRedraw() {
		return didChange;
	}

	Text::Text(const Utils::LCDString& text, uint8_t x, uint8_t y)
		: text(text), x(x), y(y) {}
	
	void Text::draw(LiquidCrystal& lcd) {
		lcd.setCursor(x, y);
		lcd.write(text.c_str());
	}
}