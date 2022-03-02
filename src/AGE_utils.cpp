#include <Arduino.h>
#include "AGE_utils.h"

void* operator new(size_t size) {
	return malloc(size);
}

void* operator new[](size_t size) {
	return malloc(size);
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
}