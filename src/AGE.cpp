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

	CharacterBuffer::CharacterBuffer(uint8_t width, uint8_t height)
		: width(width), 
		  height(height), 
		  characters((char*) calloc(sizeof(char), width * height))
	{
		if (width > 0 && height > 0)
			ASSERT(characters != NULL, "");
		clear();
	}

	CharacterBuffer::CharacterBuffer(const CharacterBuffer& other)
		: width(other.width),
		  height(other.height),
		  characters((char*) calloc(sizeof(char), width * height))
	{
		if (width > 0 && height > 0)
			ASSERT(characters != NULL, "");
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
			ASSERT(characters != NULL, "");
		}
		memcpy(characters, other.characters, width * height * sizeof(char));
	}

	char CharacterBuffer::get(uint8_t x, uint8_t y) {
		ASSERT_F(x < width && y < height, "Out of bounds; can't get (%d, %d) from character buffer", x, y);
		return characters[x + y * width];
	}

	void CharacterBuffer::put(char character, uint8_t x, uint8_t y) {
		if (x >= width || y >= height) return;
		characters[x + y * width] = character;
	}

	void CharacterBuffer::write(Utils::LCDString string, uint8_t x, uint8_t y) {
		for (int i = 0; i < string.length(); i++) {
			char c = string[i];
			put(c, x + i, y);
		}
	}

	void CharacterBuffer::write(Utils::LCDString string, uint8_t x, uint8_t y, TextAlignment alignment) {
		switch (alignment) {
			case TextAlignment::LEFT: return write(string, x, y);
			case TextAlignment::CENTER:
				return write(string, x - string.length() / 2, y);
			case TextAlignment::RIGHT:
				return write(string, x - string.length(), y);
		}
	}
	
	void CharacterBuffer::clear() {
		memset(characters, ' ', width * height * sizeof(char));
	}

	uint8_t CharacterBuffer::getWidth() {
		return width;
	}

	uint8_t CharacterBuffer::getHeight() {
		return height;
	}

	// Renderer

	Renderer::Renderer(uint8_t width, uint8_t height)
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
		render(root);
		for (uint8_t y = 0; y < frontBuffer->getHeight(); y++) {
			for (uint8_t x = 0; x < frontBuffer->getWidth(); x++) {
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

	uint8_t TextureRegistry::numTextures = 0;

	TextureRegistry::TextureRegistry(LiquidCrystal& lcd)
		: lcd(lcd) {}

	TextureID TextureRegistry::create(Utils::Array<byte, 8> textureData) {
		ASSERT_F(numTextures < maxTextures, "Can't create texture: maximum of %d custom textures reached", maxTextures);
		TextureID texture = numTextures++;
		lcd.createChar(texture, textureData.begin());
		return texture;
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

	// Game
	Game::Game() : scene(nullptr) {}

	Game::~Game() {
		delete scene;
	}

	void Game::build() {
		if(scene) addChild(scene);
	}

	void Game::update(unsigned int dt) {
		if (sceneShowing != sceneId) {
			if (scene) delete scene;
			scene = buildScene(sceneId);
			sceneShowing = sceneId;
			requestRebuild();
		}
	}

	void Game::setScene(SceneID id) {
		sceneId = id;
	}

	// SoundEffect
	SoundEffect::SoundEffect(unsigned int numTones, ...) {
		va_list args;
		va_start(args, numTones);
		for (int i = 0; i < numTones; i++)
			tones.push(va_arg(args, Tone));
		va_end(args);
	}

	const Utils::List<Tone>& SoundEffect::getTones() const {
		return tones;
	}

	// Process

	Process::Process(const ProcessConfig& cfg)
		: width(cfg.width),
		  height(cfg.height),
		  loopDelay(1000 / cfg.loopsPerSecond),
		  audioPin(cfg.audioPin),
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

	void Process::start(Game* game) {
		if (running) return;
		this->game = game;
		pinMode(audioPin, OUTPUT);
		lcd.begin(width, height);
		renderer.setRoot(game);
		running = true;
	}

	void Process::loop() {
		if (!running) return;

		unsigned long now = millis();
		unsigned int dt = now - lastLoop;

		if (currentSound != nullptr) {
			soundTime += dt;
			if (
				toneIndex == -1 || 
				soundTime > currentSound->getTones()[toneIndex].duration
			) {
				toneIndex++;
				if (toneIndex == currentSound->getTones().size()) currentSound = nullptr;
				else {
					soundTime = 0;
					Tone currentTone = currentSound->getTones()[toneIndex];
					if (currentTone.frequency) tone(audioPin, currentTone.frequency, currentTone.duration);
				}
			}
		}

		for (Trigger* trigger : triggers) trigger->update(dt);
		renderer.render(lcd, dt);

		delay(loopDelay);
		lastLoop = now;
	}

	uint8_t Process::getWidth() {
		return width;
	}

	uint8_t Process::getHeight() {
		return height;
	}

	void Process::setScene(SceneID id) {
		if (game) game->setScene(id);
	}

	void Process::playSound(const SoundEffect& sound) {
		toneIndex = -1;
		currentSound = &sound;
		soundTime = 0;
	}

	TextureID Process::createTexture(Utils::Array<byte, 8> textureData) {
		return textureRegistry.create(textureData);
	}

	void Process::registerTrigger(Trigger* trigger) {
		triggers.push(trigger);
	}
}