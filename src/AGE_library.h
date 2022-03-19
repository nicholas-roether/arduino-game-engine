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
		void spawn(const C& component) {
			if (limit != 0 && spawnedComponents.size() == limit) return;
			spawnedComponents.push(new C(component));
			requestRebuild();
		}

		void build();

		void update(unsigned int dt);
	};

	typedef uint8_t SceneID;

	class SceneSelection : public Component {
		Utils::List<Component*> scenes;
		uint8_t scene = 0;
		
	public:
		void build();

		void setScene(SceneID id);

		SceneID createScene(Component* scene);
	};

	// Triggers

	enum ClickTriggerEdge {
		BTN_DOWN,
		BTN_UP
	};

	class ClickTrigger : public Trigger {
		static constexpr unsigned int DEBOUNCE_DELAY = 10;
		unsigned int pin;
		ClickTriggerEdge edge;
		unsigned int sinceLastUp = 0;

	protected:
		bool checkActive(unsigned int dt);

	public:
		ClickTrigger(unsigned int pin);
		ClickTrigger(unsigned int pin, ClickTriggerEdge edge);
	};

	class RandomTrigger : public Trigger {
		float freq;
	
	protected:
		bool checkActive(unsigned int dt);

	public:
		RandomTrigger(float freq);
	};

	// Utilities
	class Animation {
		unsigned int duration;

	public:
		Animation(unsigned int duration);

		float progress();
	};
}

#endif