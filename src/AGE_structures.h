#ifndef _AGE_STRUCTURES_H_
#define _AGE_STRUCTURES_H_

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
			// TODO proper errors
			if (i >= numElems) abort();
			return elements[i];
		}

		const T& at(unsigned int i) const {
			// TODO proper errors
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

	class Buffer {
		size_t capacity;
		size_t size;
		void* start;

		void changeCapacity(size_t newCapacity) {
			void* newStart = realloc(start, size);
			free(start);
			start = newStart;
			capacity = newCapacity;
			size = min(size, capacity);
		}

		void requireCapacity(size_t requiredCapacity) {
			if (requiredCapacity <= capacity - size) return;
			size_t newCapacity = capacity * 2;
			while(newCapacity - size <= requiredCapacity) newCapacity *= 2;
			changeCapacity(newCapacity);
		}
	
	public:
		Buffer()
			: capacity(0), size(0), start(nullptr) {}

		Buffer(size_t initialCapacity)
			: capacity(initialCapacity), size(0), start(malloc(initialCapacity)) {}

		Buffer(const Buffer& other)
			: capacity(other.capacity), size(other.size), start(malloc(capacity))
		{
			memcpy(start, other.start, size);
		}

		Buffer& operator=(const Buffer& other) {
			changeCapacity(other.capacity);
			memcpy(start, other.start, other.size);
			size = other.size;
			capacity = other.capacity;
		}

		~Buffer() {
			free(start);
		}

		template<typename T>
		T* alloc(size_t num) {
			requireCapacity(num * sizeof(T));
			T* ptr = (T*) (start + size);
			size += num * sizeof(T);
			return ptr;
		}

		template<typename T>
		T* alloc() {
			return alloc<T>(1);
		}

		template<typename T>
		T* put(const T& item) {
			T* ptr = alloc<T>();
			*ptr = item;
			return ptr;
		}
	};
}

#endif