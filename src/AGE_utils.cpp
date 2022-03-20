#include <Arduino.h>
#include <EEPROM.h>
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
	size_t HardStorage::totalSize = 0;

	HardStorage::HardStorage(size_t size) : size(size), valueBuffer(new byte[size]) {
		address = totalSize;
		totalSize += size;
		ASSERT_F(totalSize <= EEPROM.length(), "Cannot create %d bytes of hard storage; insufficient storage space", size);
		write(valueBuffer);
	}

	size_t HardStorage::getSize() const {
		return size;
	}

	byte* HardStorage::read() const {
		for (int i = 0; i < size; i++) valueBuffer[i] = EEPROM.read(i);
		return valueBuffer;
	}

	void HardStorage::write(const byte* value) {
		for (int i = 0; i < size; i++) EEPROM.write(i, value[i]);
	}

	UUID uuid() {
		return random();
	}

	float randFloat() {
		return ((float) random()) / RANDOM_MAX;
	}
}