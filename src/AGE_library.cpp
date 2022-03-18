#include "AGE_library.h"

namespace AGE {
	// Text

	Text::Text()
		: text(""), x(0su), y(0su) {}

	Text::Text(const Prop<Utils::LCDString>& text)
		: text(text), x(0su), y(0su) {}

	Text::Text(
		const Prop<Utils::LCDString>& text,
		const Prop<uint8_t>& x,
		const Prop<uint8_t>& y
	)
		: text(text), x(x), y(y) {}
	
	void Text::draw(CharacterBuffer& buffer) {
		buffer.write((*text).c_str(), *x, *y);
	}

	// Texture

	Texture::Texture(const Prop<uint8_t>& textureId)
		: textureId(textureId), x(0su), y(0su)
	{}

	Texture::Texture(
		const Prop<uint8_t>& textureId,
		const Prop<uint8_t>& x,
		const Prop<uint8_t>& y
	)
		: textureId(textureId), x(x), y(y)
	{}

	void Texture::draw(CharacterBuffer& buffer) {
		buffer.put(*textureId, *x, *y);
	}

	// Toggled

	Toggled::Toggled(Component* child)
		: child(child), showing(true), visible(true) {}

	Toggled::Toggled(Component* child, const Prop<bool>& visible)
		: child(child), showing(*visible), visible(visible) {}

	void Toggled::setVisible(bool visible) {
		this->visible = visible;
	}

	void Toggled::toggle() {
		setVisible(!*visible);
	}

	bool Toggled::isVisible() {
		return *visible;
	}

	void Toggled::build() {
		if (showing) addChild(child);
	}

	void Toggled::update(unsigned int dt) {
		if (showing != *visible) {
			showing = *visible;
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
	Spawner::Spawner(uint8_t limit) : limit(limit) {}

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
}