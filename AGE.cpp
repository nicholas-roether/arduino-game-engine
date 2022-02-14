#include <Arduino.h>

#include "AGE.h"
#include "AGE_text.h"

namespace AGE {
	void EventManager::pushCallbackList() {
		callbackListBuffer.push(Array<void*>());
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
}