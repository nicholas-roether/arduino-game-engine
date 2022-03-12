#include "AGE_library.h"

namespace AGE {
	// Text

	Text::Text()
		: text(""), x(0), y(0) {}

	Text::Text(const Utils::LCDString& text)
		: text(text), x(x), y(y) {}

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

	// Toggled

	Toggled::Toggled(Component* child)
		: child(child), showing(true), visible(true) {}

	Toggled::Toggled(Component* child, bool visible)
		: child(child), showing(visible), visible(visible) {}

	void Toggled::setVisible(bool visible) {
		this->visible = visible;
	}

	void Toggled::toggle() {
		setVisible(!visible);
	}

	bool Toggled::isVisible() {
		return visible;
	}

	void Toggled::build() {
		if (showing) addChild(child);
	}

	void Toggled::update(unsigned int dt) {
		if (showing != visible) {
			showing = visible;
			requestRebuild();
		}
	}

	// SpawnableComponent

	void SpawnableComponent::die() {
		deathFlag = true;
	}

	bool SpawnableComponent::shouldDie() {
		return deathFlag;
	}

	// Spawner
	Spawner::~Spawner() {
		for (SpawnableComponent* component : spawnedComponents)
				delete component;
	}

	void Spawner::build() {
		for (SpawnableComponent* spc : spawnedComponents)
			addChild(spc);
	}

	void Spawner::update(unsigned int dt)  {
		for (unsigned int i = 0; i < spawnedComponents.size(); i++) {
			if (!spawnedComponents[i]->shouldDie()) continue;
			delete spawnedComponents[i];
			spawnedComponents.remove(i);
			requestRebuild();
		}
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