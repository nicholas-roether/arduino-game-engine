#include "AGE.h"

namespace AGE {
	// Component

	void Component::addChild(Component* child) {
		if (!child) return;
		children.push(child);
	}

	void Component::requestRebuild() {
		rebuildRequested = true;
	}

	const Utils::List<Component*>& Component::getChildren() const {
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

	// CharacterBuffer

	CharacterBuffer::CharacterBuffer(size_t width, size_t height)
		: width(width), 
		  height(height), 
		  characters((char*) calloc(sizeof(char), width * height))
	{
		if (width > 0 && height > 0)
			ASSERT_F(characters != NULL, "Failed to allocate character buffer of size %d by %d: insufficient memory", width, height);
		clear();
	}

	CharacterBuffer::CharacterBuffer(const CharacterBuffer& other)
		: width(other.width),
		  height(other.height),
		  characters((char*) calloc(sizeof(char), width * height))
	{
		if (width > 0 && height > 0)
			ASSERT_F(characters != NULL, "Failed to allocate character buffer of size %d by %d: insufficient memory", width, height);
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
			ASSERT_F(characters != NULL, "Failed to allocate character buffer of size %d by %d: insufficient memory", width, height);
		}
		memcpy(characters, other.characters, width * height * sizeof(char));
	}

	char CharacterBuffer::get(unsigned int x, unsigned int y) {
		ASSERT_F(x < width && y < height, "Out of bounds: can't access position (%d, %d) in buffer with dimensions (%d, %d).", x, y, width, height);
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
		if (component->shouldRebuild()) {
			component->rebuild();
			component->didRebuild();
		}
	}

	void Renderer::update(Component* component, unsigned int dt) {
		component->update(dt);
		build(component);
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

	void Renderer::render(LiquidCrystal& lcd, unsigned int dt) {
		frontBuffer->clear();
		update(root, dt);
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
		swapCharBuffers();
	}

	// TextureRegistry

	unsigned int TextureRegistry::numTextures;

	TextureRegistry::TextureRegistry(LiquidCrystal& lcd)
		: lcd(lcd) {}

	TextureID TextureRegistry::create(Utils::Array<byte, 8> textureData) {
		ASSERT_F(numTextures < maxTextures, "Can't create texture: maximum of %d custom textures reached", maxTextures);
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
		unsigned long now = millis();
		unsigned int dt = now - lastLoop;
		for (Trigger* trigger : triggers) trigger->update(dt);
		renderer.render(lcd, dt);
		delay(loopDelay);
		lastLoop = now;
	}

	unsigned int Process::getWidth() {
		return width;
	}

	unsigned int Process::getHeight() {
		return height;
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
}