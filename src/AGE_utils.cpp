#include <Arduino.h>
#include "AGE_utils.h"
#include "AGE_debug.h"

void* operator new(size_t size) {
	void* ptr = malloc(size);
	ASSERT_F(ptr != NULL, "Failed to dynamically allocate %d bytes: insufficient memory available", size);
	return ptr;
}

void* operator new[](size_t size) {
	void* ptr = malloc(size);
	ASSERT_F(ptr != NULL, "Failed to dynamically allocate %d bytes: insufficient memory available", size);
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
}