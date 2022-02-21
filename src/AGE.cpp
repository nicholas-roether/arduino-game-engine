#include "AGE.h"

namespace AGE {
	// Component

	void Component::addChild(Component* child) {
		children.push(child);
	}

	void Component::setState() {
		stateDidChange = true;
	}

	const Utils::Array<Component*>& Component::getChildren() {
		return children;
	}

	void Component::rebuild() {
		children.clear();
		build();
	}

	void Component::update(unsigned int dt) {}

	void Component::draw(CharacterBuffer& buffer) {}

	void Component::build() {}

	bool Component::shouldRebuild() {
		return stateDidChange;
	}

	void Component::didRebuild() {
		stateDidChange = false;
	}

	// Text

	Text::Text()
		: text(""), x(0), y(0) {}

	Text::Text(const Utils::LCDString& text, uint8_t x, uint8_t y)
		: text(text), x(x), y(y) {}
	
	Text::Text(const Text& text)
		: text(text.text), x(text.x), y(text.y)
	{}
	
	void Text::draw(CharacterBuffer& buffer) {
		buffer.write(text.c_str(), x, y);
	}

	void Text::setText(const Utils::LCDString& str) {
		text = str;
		setState();
	}

	void Text::setX(uint8_t x) {
		this->x = x;
		setState();
	}

	void Text::setY(uint8_t y) {
		this->y = y;
		setState();
	}

	void Text::setPos(uint8_t x, uint8_t y) {
		setX(x);
		setY(y);
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

	void Renderer::update(Component* component, unsigned int dt) {
		component->update(dt);
		for (Component* child : component->getChildren())
			render(child);
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
		unsigned int now = millis();
		build(root);
		update(root, now - lastRender);
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
		lastRender = now;
	}

	// Trigger

	Trigger::Trigger(bool initial) : isActive(initial) {}

	void Trigger::active() {
		isActive = true;
	}

	bool Trigger::state() {
		return isActive;
	}

	bool Trigger::fired() {
		return isActive && !didFire;
	}

	void Trigger::update(unsigned int dt) {
		bool nextState = checkActive(dt);
		if (!nextState || !isActive) {
			didFire = false;
		} else if (isActive) didFire = true;
		isActive = nextState;
	}

	// ClickTrigger

	ClickTrigger::ClickTrigger(unsigned int pin)
		: Trigger(false), pin(pin), edge(BTN_DOWN) {}

	ClickTrigger::ClickTrigger(unsigned int pin, ClickTriggerEdge edge)
		: Trigger(edge == BTN_UP), pin(pin), edge(edge) {}

	bool ClickTrigger::checkActive(unsigned int dt) {
		if (sinceLastUp < DEBOUNCE_DELAY) {
			sinceLastUp += dt;
			return state();
		}
		if (edge == BTN_DOWN) return digitalRead(pin);
		if (edge == BTN_UP) return !digitalRead(pin);
	}
}