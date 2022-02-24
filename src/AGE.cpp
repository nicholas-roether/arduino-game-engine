#include "AGE.h"

namespace AGE {
	// Component

	void Component::addChild(Component* child) {
		children.push(child);
	}

	void Component::requestRebuild() {
		rebuildRequested = true;
	}

	const Utils::List<Component*>& Component::getChildren() {
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
		return rebuildRequested;
	}

	void Component::didRebuild() {
		rebuildRequested = false;
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
	}

	void Text::setX(uint8_t x) {
		this->x = x;
	}

	void Text::setY(uint8_t y) {
		this->y = y;
	}

	void Text::setPos(uint8_t x, uint8_t y) {
		setX(x);
		setY(y);
	}

	// Texture

	Texture::Texture(uint8_t textureId) : textureId(textureId) {}

	Texture::Texture(uint8_t textureId, uint8_t x, uint8_t y)
		: textureId(textureId), x(x), y(y) {}

	void Texture::draw(CharacterBuffer& buffer) {
		buffer.put(textureId, x, y);
	}

	void Texture::setTexture(uint8_t textureId) {
		this->textureId = textureId;
	}

	void Texture::setX(uint8_t x) {
		this->x = x;
	}

	void Texture::setY(uint8_t y) {
		this->y = y;
	}

	void Texture::setPos(uint8_t x, uint8_t y) {
		setX(x);
		setY(y);
	}

	// CharacterBuffer

	CharacterBuffer::CharacterBuffer(size_t width, size_t height)
		: width(width), 
		  height(height), 
		  characters((char*) calloc(sizeof(char), width * height))
	{
		clear();
	}

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
	
	void CharacterBuffer::clear() {
		memset(characters, ' ', width * height * sizeof(char));
	}

	size_t CharacterBuffer::getWidth() {
		return width;
	}

	size_t CharacterBuffer::getHeight() {
		return height;
	}

	// Renderer

	Renderer::Renderer(size_t width, size_t height)
		: buffer1(width, height), buffer2(width, height) {}

	void Renderer::swapCharBuffers() {
		CharacterBuffer* newFrontBuffer = backBuffer;
		backBuffer = frontBuffer;
		frontBuffer = newFrontBuffer;
	}

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
			update(child, dt);
	}

	void Renderer::render(Component* component) {
		component->draw(*frontBuffer);
		for (Component* child : component->getChildren())
			render(child);
	}

	void Renderer::setRoot(Component* root) {
		this->root = root;
	}

	void Renderer::render(LiquidCrystal& lcd) {
		unsigned int now = millis();
		frontBuffer->clear();
		update(root, now - lastRender);
		build(root);
		render(root);
		for (unsigned int y = 0; y < frontBuffer->getHeight(); y++) {
			for (unsigned int x = 0; x < frontBuffer->getWidth(); x++) {
				char c = frontBuffer->get(x, y);
				if (c != backBuffer->get(x, y)) {
					lcd.setCursor(x, y);
					lcd.write(c);
				}
			}
		}
		firstBuild = false;
		lastRender = now;
		swapCharBuffers();
	}

	// TextureRegistry

	unsigned int TextureRegistry::numTextures;

	TextureRegistry::TextureRegistry(LiquidCrystal& lcd)
		: lcd(lcd) {}

	TextureID TextureRegistry::create(Utils::Array<byte, 8> textureData) {
		if (numTextures >= maxTextures) abort();
		TextureID texture = numTextures++;
		lcd.createChar(texture, textureData.begin());
		return texture;
	}

	// Process

	Process::Process(const ProcessConfig& cfg)
		: width(cfg.width),
		  height(cfg.height),
		  loopDelay(1000 / cfg.loopsPerSecond),
		  lcd(
			  cfg.lcdConfig.rs,
			  cfg.lcdConfig.enable,
			  cfg.lcdConfig.d0,
			  cfg.lcdConfig.d1,
			  cfg.lcdConfig.d2,
			  cfg.lcdConfig.d3
		  ),
		  renderer(cfg.width, cfg.height),
		  textureRegistry(lcd)
	{}

	void Process::start(Component* root) {
		if (running) return;
		lcd.begin(width, height);
		renderer.setRoot(root);
		running = true;
	}

	void Process::loop() {
		if (!running) return;
		renderer.render(lcd);
		delay(loopDelay);
	}

	CollisionSystem& Process::getCollisionSystem() {
		return collisionSystem;
	}

	const CollisionSystem& Process::getCollisionSystem() const {
		return collisionSystem;
	}

	TextureID Process::createTexture(Utils::Array<byte, 8> textureData) {
		return textureRegistry.create(textureData);
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

	// CollisionTrigger

	CollisionTrigger::CollisionTrigger(
		const CollidingPhysicsObject* collider,
		unsigned int objType,
		const Process* process
	) : Trigger(false), collider(collider), objType(objType), process(process) {}

	bool CollisionTrigger::checkActive(unsigned int dt) {
		return process->getCollisionSystem()
			.getCollisionList(*collider)
			.includes(objType);
	}
}