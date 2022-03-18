#include <Arduino.h>
#include "AGE_utils.h"
#include "AGE_debug.h"

#ifdef AGE_DEBUG_MODE
const char* allocationErrMsg = "Dynamic allocation failed: Insufficient memory available";
#endif

void* operator new(size_t size) {
	void* ptr = malloc(size);
	ASSERT(ptr != NULL, allocationErrMsg);
	return ptr;
}

void* operator new[](size_t size) {
	void* ptr = malloc(size);
	ASSERT(ptr != NULL, allocationErrMsg);
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