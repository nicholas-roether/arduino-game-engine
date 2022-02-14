#include <Arduino.h>

#include "AGE.h"
#include "AGE_text.h"

namespace AGE {
	void EventManager::pushCallbackList() {
		callbackListBuffer.push(Array<void*>());
	}

	
}