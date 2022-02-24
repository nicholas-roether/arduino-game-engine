#ifndef _AGE_UTILS_H_
#define _AGE_UTILS_H_

#include <Arduino.h>

namespace AGE::Utils {
	template<typename T>
	class Array {
		size_t capacity;
		size_t numElems;
		T* elements;

		void increaseCapacity() {
			resizeTo(capacity == 0 ? 1 : 2 * capacity);
		}

	public:
		Array() : capacity(0), numElems(0), elements(nullptr) {}
		Array(size_t capacity)
			: capacity(capacity), numElems(0), elements((T*) malloc(capacity * sizeof(T)))
		{}

		Array(const Array<T>& other)
			: capacity(other.capacity), numElems(other.numElems), elements((T*) malloc(capacity * sizeof(T)))
		{
			for (unsigned int i = 0; i < numElems; i++)
				elements[i] = other.elements[i];
		}

		~Array() {
			free(elements);
		}

		void push(const T& elem) {
			if (numElems == capacity) increaseCapacity();
			elements[numElems++] = elem;
		}

		T pop() {
			return elements[--numElems];
		}

		T& at(unsigned int i) {
			if (i >= numElems) abort();
			return elements[i];
		}

		const T& at(unsigned int i) const {
			if (i >= numElems) abort();
			return elements[i];
		}

		void clear() {
			numElems = 0;
		}

		void resizeTo(size_t newCapacity) {
			T* newElems = (T*) malloc(newCapacity * sizeof(T));
			for (unsigned int i = 0; i < numElems && i < newCapacity; i++)
				newElems[i] = elements[i];
			free(elements);
			elements = newElems;
			capacity = newCapacity;		
		}

		bool includes(const T& element) const {
			for (const T& elem : *this)
				if (elem == element) return true;
			return false;
		}

		size_t size() const {
			return numElems;
		}

		T* begin() {
			return elements;
		}

		const T* begin() const {
			return elements;
		}

		T* end() {
			return elements + numElems;
		}

		const T* end() const {
			return elements + numElems;
		}

		T& operator[](unsigned int i) {
			return at(i);
		}

		const T& operator[](unsigned int i) const {
			return at(i);
		}
	};

	typedef uint32_t UUID;

	UUID uuid();
}

#endif