#ifndef _AGE_COMPONENTS_H_
#define _AGE_COMPONENTS_H_

#include <Arduino.h>
#include "AGE.h"
#include "AGE_utils.h"

namespace AGE {
	// Components

	class SpawnableComponent : public Component {
		bool deathFlag = false;

	public:
		virtual ~SpawnableComponent() = default;

		void die();

		bool shouldDie();
	};

	class Spawner : public Component {
		uint8_t limit = 0;
		Utils::List<SpawnableComponent*> spawnedComponents;

	public:
		Spawner() = default;
		Spawner(uint8_t limit);

		virtual ~Spawner();

		template<typename C>
		bool spawn(const C& component) {
			if (limit != 0 && spawnedComponents.size() == limit) return false;
			spawnedComponents.push(new C(component));
			requestRebuild();
			return true;
		}
	
		void build();

		void update(unsigned int dt);
	};

	// Triggers

	enum ClickTriggerEdge {
		BTN_DOWN,
		BTN_UP
	};

	class ClickTrigger : public Trigger {
		static constexpr unsigned int DEBOUNCE_DELAY = 10;
		uint8_t pin;
		ClickTriggerEdge edge;
		unsigned int sinceLastUp = 0;

	protected:
		bool checkActive(unsigned int dt);

	public:
		ClickTrigger(unsigned int pin);
		ClickTrigger(unsigned int pin, ClickTriggerEdge edge);
	};

	class RandomTrigger : public Trigger {
		unsigned int minTime;
		unsigned int maxTime;
		unsigned int nextTime;
		unsigned int time;
	
	protected:
		bool checkActive(unsigned int dt);

	public:
		RandomTrigger(unsigned int minTime, unsigned int maxTime);
	};

	// Utilities
	class Animation {
		unsigned int duration;

	public:
		Animation(unsigned int duration);

		float progress() const;
	};

	class Delay {
		unsigned int duration;
		unsigned long start;

	public:
		Delay(unsigned int duration);

		bool finished() const;
	};
}

#endif