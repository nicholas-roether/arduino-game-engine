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

	void Spawner::update(uint8_t dt)  {
		for (unsigned int i = 0; i < spawnedComponents.size(); i++) {
			if (!spawnedComponents[i]->shouldDie()) continue;
			delete spawnedComponents[i];
			spawnedComponents.remove(i);
			requestRebuild();
		}
	}

	// ClickTrigger
	ClickTrigger::ClickTrigger(uint8_t pin)
		: Trigger(false), pin(pin), edge(BTN_DOWN) {}

	ClickTrigger::ClickTrigger(uint8_t pin, ClickTriggerEdge edge)
		: Trigger(edge == BTN_UP), pin(pin), edge(edge) {}

	bool ClickTrigger::checkActive(uint8_t dt) {
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

	bool RandomTrigger::checkActive(uint8_t dt) {
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

	Delay::Delay(unsigned int duration) : duration(duration), start(millis()) {}

	bool Delay::finished() const {
		return millis() - start > duration;
	}
}