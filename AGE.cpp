#include <Arduino.h>

#include "AGE.h"
#include "AGE_text.h"

namespace AGE {
	void EventManager::pushCallbackList() {
		callbackListBuffer.push(Array<const void(*) ()>());
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
}