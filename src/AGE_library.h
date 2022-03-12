#ifndef _AGE_COMPONENTS_H_
#define _AGE_COMPONENTS_H_

#include <Arduino.h>
#include "AGE.h"
#include "AGE_utils.h"

namespace AGE {
	// Components

	class Text : public Component {
		Prop<Utils::LCDString> text;
		Prop<uint8_t> x;
		Prop<uint8_t> y;
	
	public:
		Text();

		Text(Prop<Utils::LCDString> text);

		Text(Prop<Utils::LCDString> text, Prop<uint8_t> x, Prop<uint8_t> y);

		void draw(CharacterBuffer& buffer);
	};

	class Texture : public Component {
		Prop<uint8_t> textureId;
		Prop<uint8_t> x;
		Prop<uint8_t> y;

	public:
		Texture(Prop<uint8_t> textureId);
		Texture(Prop<uint8_t> textureId, Prop<uint8_t> x, Prop<uint8_t> y);

		void draw(CharacterBuffer& buffer);
	};

	class Toggled : public Component {
		Component* child;
		bool showing;
		Prop<bool> visible;

	public:
		Toggled(Component* child);
		Toggled(Component* child, Prop<bool> visible);

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

	class Spawner : public Component {
		Utils::List<SpawnableComponent*> spawnedComponents;

	public:
		virtual ~Spawner();

		template<typename C>
		void spawn(const C& component) {
			spawnedComponents.push(new C(component));
			requestRebuild();
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