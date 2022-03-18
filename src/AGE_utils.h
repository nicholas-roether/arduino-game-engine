#ifndef _AGE_UTILS_H_
#define _AGE_UTILS_H_

#include <Arduino.h>
#include <stdarg.h>
#include <stdlib.h>
#include <new.h>

#include "AGE_debug.h"

inline constexpr uint8_t operator ""su(unsigned long long arg) noexcept {
	return static_cast<uint8_t>(arg);
}

namespace AGE::Utils {
	template<typename T>
	class Iterable {
	protected:
		Iterable() = default;

		virtual T* getPointer() const = 0;

	public:
		virtual size_t size() const = 0;

		T* begin() {
			return getPointer();
		}

		const T* begin() const {
			return getPointer();
		}

		T* end() {
			return begin() + size();
		}

		const T* end() const {
			return begin() + size();
		}

		bool includes(const T& element) const {
			for (const T& elem : *this)
				if (elem == element) return true;
			return false;
		}

		T& operator[](unsigned int i) {
			ASSERT_F(i < size(), "Out of bounds: can't access index %d in iterable of size %d", i, size());
			return *(getPointer() + i);
		}

		const T& operator[](unsigned int i) const {
			ASSERT_F(i < size(), "Out of bounds: can't access index %d in iterable of size %d", i, size());
			return *(getPointer() + i);
		}
	};

	template<typename T>
	class List : public Iterable<T> {
		size_t capacity;
		size_t numElems;
		T* elements;

		void increaseCapacity() {
			resizeTo(2 * capacity);
		}

	protected:
		T* getPointer() const {
			return elements;
		}

	public:
		List() : capacity(1), numElems(0), elements((T*) malloc(sizeof(T))) {}
		List(size_t capacity)
			: capacity(capacity), numElems(0), elements((T*) malloc(capacity * sizeof(T)))
		{
			ASSERT_F(elements != NULL, "Failed to allocate list of size %d: insufficient memory", capacity);
		}

		List(const List<T>& other)
			: capacity(other.capacity), numElems(other.numElems), elements((T*) malloc(other.capacity * sizeof(T)))
		{
			ASSERT_F(elements != NULL, "Failed to allocate list of size %d: insufficient memory", capacity);
			memcpy(elements, other.elements, numElems * sizeof(T));
		}

		~List() {
			free(elements);
		}

		List& operator=(const List& other) {
			if (capacity < other.capacity) resizeTo(other.capacity);
			numElems = other.numElems;
			memcpy(elements, other.elements, numElems * sizeof(T));
		}

		size_t size() const {
			return numElems;
		}

		void push(const T& elem) {
			if (numElems == capacity) increaseCapacity();
			elements[numElems++] = elem;
		}

		T pop() {
			return elements[--numElems];
		}

		void remove(unsigned int i) {
			ASSERT_F(i < numElems, "Out of bounds: can't remove index %d from array of size %d", i, numElems);
			memmove(elements + i, elements + i + 1, (numElems - i - 1) * sizeof(T));
			numElems--;
		}

		void removeElement(const T& elem) {
			for (int i = 0; i < size(); i++)
				if ((*this)[i] == elem) remove(i);
		}

		void clear() {
			numElems = 0;
		}

		void resizeTo(size_t newCapacity) {
			T* newElems = (T*) realloc(elements, newCapacity * sizeof(T));
			ASSERT_F(newElems != NULL, "Resizing list to %d failed: insufficient memory", newCapacity);
			elements = newElems;
			capacity = newCapacity;	
		}
	};

	template<typename T, size_t numElems>
	class Array : public Iterable<T> {
		T elements[numElems];

	protected:
		T* getPointer() const {
			return (T*) elements;
		}

	public:
		Array() = default;
		Array(const T elements[numElems]) {
			memcpy(this->elements, elements, numElems);
		}
		Array(T first...) {
			elements[0] = first;

			va_list args;
			va_start(args, first);

			for (unsigned int i = 1; i < numElems; i++)
				elements[i] = va_arg(args, int);
			
			va_end(args);
		}

		size_t size() const {
			return numElems;
		}
	};

	typedef uint32_t UUID;

	UUID uuid();

	float randFloat();
}

#endif