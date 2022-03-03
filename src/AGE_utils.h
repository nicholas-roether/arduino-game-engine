#ifndef _AGE_UTILS_H_
#define _AGE_UTILS_H_

#include <Arduino.h>
#include <stdarg.h>
#include <stdlib.h>
#include <new.h>

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
			return *(getPointer() + i);
		}

		const T& operator[](unsigned int i) const {
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
		{}

		List(const List<T>& other)
			: capacity(other.capacity), numElems(other.numElems), elements((T*) malloc(other.capacity * sizeof(T)))
		{
			for (unsigned int i = 0; i < numElems; i++)
				new (elements + i) T(other.elements[i]);
		}

		~List() {
			for (const T& elem : *this) elem.~T();
			free(elements);
		}

		List& operator=(const List& other) {
			if (capacity < other.capacity) resizeTo(other.capacity);
			clear();
			numElems = other.numElems;
			for (unsigned int i = 0; i < numElems; i++)
				elements[i] = other[i];
		}

		size_t size() const {
			return numElems;
		}

		void push(const T& elem) {
			if (numElems == capacity) increaseCapacity();
			new (elements + numElems) T(elem);
			numElems++;
		}

		T pop() {
			T elem = elements[numElems];
			elements[numElems].~T();
			numElems--;
			return elem;
		}

		void remove(unsigned int i) {
			Serial.println("removing...");
			if (i >= numElems) abort();
			for (unsigned int j = i + 1; j < numElems; j++)
				elements[j - 1] = elements[j];
			elements[numElems - 1].~T();
			numElems--;
		}

		void clear() {
			for (unsigned int i = 0; i < numElems; i++)
				elements[i].~T();
			numElems = 0;
		}

		void resizeTo(size_t newCapacity) {
			// FIXME this crashes :(
			T* newElems = (T*) realloc(elements, newCapacity * sizeof(T));
			if (newElems != elements) {
				for (unsigned int i = 0; i < numElems; i++) {
					new (newElems + i) T(elements[i]);
					elements[i].~T();
				}
				free(elements);
			}
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
}

#endif