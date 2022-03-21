#include "AGE_library.h"

namespace AGE {
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

	// RandomTrigger
	RandomTrigger::RandomTrigger(unsigned int minTime, unsigned int maxTime)
		: Trigger(false), minTime(minTime), maxTime(maxTime), time(0), nextTime(random(minTime, maxTime)) {}

	bool RandomTrigger::checkActive(unsigned int dt) {
		time += dt;
		if (time > nextTime) {
			time = 0;
			nextTime = random(minTime, maxTime);
		}
	}

	// Animation
	Animation::Animation(unsigned int duration) : duration(duration) {}

	float Animation::progress() const {
		return fmod(millis(), duration) / duration;
	}
}