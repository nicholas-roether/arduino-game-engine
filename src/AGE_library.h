#ifndef _AGE_COMPONENTS_H_
#define _AGE_COMPONENTS_H_

#include <Arduino.h>
#include "AGE.h"

namespace AGE {
	// Components

	class Text : public Component {
		Utils::LCDString text;
		uint8_t x;
		uint8_t y;
	
	public:
		Text();

		Text(const Utils::LCDString& text);

		Text(const Utils::LCDString& text, uint8_t x, uint8_t y);

		Text(const Text& text);

		void draw(CharacterBuffer& buffer);

		void setText(const Utils::LCDString& str);

		void setX(uint8_t x);

		void setY(uint8_t y);

		void setPos(uint8_t x, uint8_t y);
	};

	class Texture : public Component {
		uint8_t textureId;
		uint8_t x;
		uint8_t y;

	public:
		Texture(uint8_t textureId);
		Texture(uint8_t textureId, uint8_t x, uint8_t y);

		void draw(CharacterBuffer& buffer);

		void setTexture(uint8_t texutreId);

		void setX(uint8_t x);

		void setY(uint8_t y);

		void setPos(uint8_t x, uint8_t y);
	};

	class Toggled : public Component {
		Component* child;
		bool showing;
		bool visible;

	public:
		Toggled(Component* child);
		Toggled(Component* child, bool visible);

		void setVisible(bool visible);

		void toggle();

		bool isVisible();

		void build();

		void update(unsigned int dt);
	};

	class SpawnableComponent : public Component {
		bool deathFlag = false;

	protected:
		void die();

	public:
		bool shouldDie();
	};

	template<typename C>
	class Spawner : public Component {
		Utils::List<C> spawnedComponents;

	public:

		void spawn(const C& component) {
			spawnedComponents.push(component);
			requestRebuild();
		}


		void build() {
			for (SpawnableComponent& spc : spawnedComponents)
				addChild(&spc);
		}

		void update(unsigned int dt) {
			for (unsigned int i = 0; i < spawnedComponents.size(); i++) {
				if (!spawnedComponents[i].shouldDie()) continue;
				spawnedComponents.remove(i);
				requestRebuild();
			}
		}
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

	class CollisionTrigger : public Trigger {
		const CollidingPhysicsObject* collider;
		unsigned int objType;
		const Process* process;

	protected:
		bool checkActive(unsigned int dt);

	public:
		CollisionTrigger(
			const CollidingPhysicsObject* collider,
			unsigned int objType,
			const Process* process
		);
	};
}

#endif