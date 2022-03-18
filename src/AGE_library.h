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

		Text(const Prop<Utils::LCDString>& text);

		Text(
			const Prop<Utils::LCDString>& text,
			const Prop<uint8_t>& x,
			const Prop<uint8_t>& y
		);

		void draw(CharacterBuffer& buffer);
	};

	class Texture : public Component {
		Prop<uint8_t> textureId;
		Prop<uint8_t> x;
		Prop<uint8_t> y;

	public:
		Texture(const Prop<uint8_t>& textureId);
		Texture(
			const Prop<uint8_t>& textureId,
			const Prop<uint8_t>& x,
			const Prop<uint8_t>& y
		);

		void draw(CharacterBuffer& buffer);
	};

	class Toggled : public Component {
		Component* child;
		bool showing;
		Prop<bool> visible;

	public:
		Toggled(Component* child);
		Toggled(Component* child, const Prop<bool>& visible);

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
}

#endif