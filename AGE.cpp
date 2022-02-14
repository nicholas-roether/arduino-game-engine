#include <Arduino.h>

#include "AGE.h"
#include "AGE_text.h"

namespace AGE {
	void EventManager::resizeCallbackPtrArray(size_t size) {
		void** newArray = (void**) calloc(size, sizeof(void*));
		memcpy(newArray, callbackPtrArray, numEvents * sizeof(void*));
		free(callbackPtrArray);
		callbackPtrCapacity = size;
		if (numEvents > callbackPtrCapacity) numEvents = callbackPtrCapacity;
		callbackPtrArray = newArray;
	}

	void EventManager::createNextEvent() {
		if (numEvents >= callbackPtrCapacity)
			resizeCallbackPtrArray(2 * callbackPtrCapacity);
	}
}