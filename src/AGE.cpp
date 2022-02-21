#include "AGE.h"

namespace AGE {
	// Event manager
	void EventManager::pushCallbackList() {
		Utils::Array<void(*)()> callbackList;
		callbackListBuffer.push(callbackList);
	}

	EventManager::EventManager() : callbackListBuffer() {}

	void EventManager::on(Event event, void(*callback)()) {
		if (event >= callbackListBuffer.size()) return;
		callbackListBuffer[event].push(callback);
	}

	void EventManager::dispatch(Event event) {
		if (event >= callbackListBuffer.size()) return;
		for (void(*callback)() : callbackListBuffer[event])
			callback();
	}

	Event EventManager::newEvent() {
		pushCallbackList();
		return callbackListBuffer.size() - 1;
	}

	// Group
	Group::Group(size_t initialCapacity)
		: childPtrs(initialCapacity) {}

	void Group::add(Component* child) {
		childPtrs.push(child);
		didChange = true;
	}

	void Group::build() {
		for (Component* cPtr : childPtrs) addChild(cPtr);
	}

	void Group::didRebuild() {
		didChange = false;
	}

	bool Group::shouldRebuild() {
		return didChange;
	}

	// Component

	void Component::addChild(Component* child) {
		children.push(child);
	}

	const Utils::Array<Component*>& Component::getChildren() {
		return children;
	}

	void Component::rebuild() {
		children.clear();
		build();
	}

	void Component::draw(CharacterBuffer& buffer) {}

	void Component::build() {}

	void Component::didRebuild() {}

	bool Component::shouldRebuild() {
		return false;
	}

	// Text

	Text::Text(const Utils::LCDString& text, uint8_t x, uint8_t y)
		: text(text), x(x), y(y) {}
	
	Text::Text(const Text& text)
		: text(text.text), x(text.x), y(text.y)
	{}

	Text& Text::operator=(const Text& other) {
		text = other.text.c_str();
		x = other.x;
		y = other.y;
	}
	
	void Text::draw(CharacterBuffer& buffer) {
		buffer.write(text.c_str(), x, y);
	}

	// CharacterBuffer

	CharacterBuffer::CharacterBuffer(size_t width, size_t height)
		: width(width), 
		  height(height), 
		  characters((char*) calloc(sizeof(char), width * height))
	{}

	CharacterBuffer::CharacterBuffer(const CharacterBuffer& other)
		: width(other.width),
		  height(other.height),
		  characters((char*) calloc(sizeof(char), width * height))
	{
		memcpy(characters, other.characters, width * height * sizeof(char));
	}

	CharacterBuffer::~CharacterBuffer() {
		free(characters);
	}

	CharacterBuffer& CharacterBuffer::operator=(const CharacterBuffer& other) {
		if (width != other.width || height != other.height) {
			free(characters);
			width = other.width;
			height = other.height;
			characters = (char*) malloc(width * height * sizeof(char));
		}
		memcpy(characters, other.characters, width * height * sizeof(char));
	}

	char CharacterBuffer::get(unsigned int x, unsigned int y) {
		if (x >= width || y >= height) abort();
		return characters[x + y * width];
	}

	void CharacterBuffer::put(char character, unsigned int x, unsigned int y) {
		if (x >= width || y >= height) return;
		characters[x + y * width] = character;
	}

	void CharacterBuffer::write(const char* characters, unsigned int x, unsigned int y) {
		for (int i = 0; ; i++) {
			char c = characters[i];
			if (c == '\0') break;
			put(c, x + i, y);
		}
	}
	
	char* CharacterBuffer::begin() {
		return characters;
	}

	char* CharacterBuffer::end() {
		return characters + width * height;
	}

	size_t CharacterBuffer::getWidth() {
		return width;
	}

	size_t CharacterBuffer::getHeight() {
		return height;
	}

	// Renderer

	Renderer::Renderer(size_t width, size_t height)
		: charBuffer(width, height), prevCharBuffer(width, height) {}

	void Renderer::build(Component* component) {
		if (firstBuild) component->build();
		else if (component->shouldRebuild()) {
			component->rebuild();
			component->didRebuild();
		}
		for (Component* child : component->getChildren())
			build(child);
	}

	void Renderer::render(Component* component) {
		component->draw(charBuffer);
		for (Component* child : component->getChildren())
			render(child);
	}

	void Renderer::setRoot(Component* root) {
		this->root = root;
	}

	void Renderer::render(LiquidCrystal& lcd) {
		build(root);
		render(root);
		for (unsigned int y = 0; y < charBuffer.getHeight(); y++) {
			for (unsigned int x = 0; x < charBuffer.getWidth(); x++) {
				char c = charBuffer.get(x, y);
				if (c != prevCharBuffer.get(x, y)) {
					lcd.setCursor(x, y);
					lcd.write(c);
				}
			}
		}
		firstBuild = false;
	}
}