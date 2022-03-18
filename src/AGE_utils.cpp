#include <Arduino.h>
#include "AGE_utils.h"
#include "AGE_debug.h"


void* operator new(size_t size) {
	void* ptr = malloc(size);
	ASSERT(ptr != NULL, "");
	return ptr;
}

void* operator new[](size_t size) {
	void* ptr = malloc(size);
	ASSERT(ptr != NULL, "");
	return ptr;
}

void * operator new(size_t size, void * ptr) noexcept {
	return ptr;
}

void operator delete(void * ptr) {
	free(ptr);
}

void operator delete[](void * ptr) {
	free(ptr);
}

namespace AGE::Utils {
	UUID uuid() {
		return random();
	}

	float randFloat() {
		return ((float) random()) / RANDOM_MAX;
	}
}