#ifndef _AGE_H_
#define _AGE_H_

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"
#include "AGE_utils.h"
#include "AGE_physics.h"

namespace AGE {
	class CharacterBuffer;

		class Component {
		Utils::List<Component*> children;
		bool rebuildRequested = false;
	
	protected:
		void addChild(Component* child);

		void requestRebuild();

	public:
		const Utils::List<Component*>& getChildren();

		void rebuild();

		virtual void update(unsigned int dt);

		virtual void draw(CharacterBuffer& buffer);

		virtual void build();

		bool shouldRebuild();

		void didRebuild();
	};

	class CharacterBuffer {
		size_t width;
		size_t height;
		char* characters;

	public:
		CharacterBuffer() = delete;
		CharacterBuffer(size_t width, size_t height);
		CharacterBuffer(const CharacterBuffer& other);

		~CharacterBuffer();

		CharacterBuffer& operator=(const CharacterBuffer& other);

		char get(unsigned int x, unsigned int y);

		void put(char character, unsigned int x, unsigned int y);

		void write(const char* characters, unsigned int x, unsigned int y);

		void clear();

		size_t getWidth();

		size_t getHeight();
	};

	class Renderer {
		CharacterBuffer buffer1;
		CharacterBuffer buffer2;
		CharacterBuffer* frontBuffer = &buffer1;
		CharacterBuffer* backBuffer = &buffer2;
		bool buffersSwapped = false;
		Component* root;
		unsigned int lastRender = 0;
		bool firstBuild = true;

		void swapCharBuffers();

		void build(Component* component);

		void update(Component* component, unsigned int dt);

		void render(Component* component);

	public:
		Renderer() = delete;
		Renderer(size_t width, size_t height);

		void setRoot(Component* root);

		void render(LiquidCrystal& lcd);
	};

	typedef uint8_t TextureID;

	class TextureRegistry {
		static constexpr unsigned int maxTextures = 16;
		static unsigned int numTextures;
		LiquidCrystal& lcd;

	public:
		TextureRegistry(LiquidCrystal& lcd);

		TextureID create(Utils::Array<byte, 8> textureData);
	};

	struct LCDConfig {
		uint8_t rs;
		uint8_t enable;
		uint8_t d0;
		uint8_t d1;
		uint8_t d2;
		uint8_t d3;
	};
	
	struct ProcessConfig {
		unsigned int width;
		unsigned int height;
		unsigned int loopsPerSecond;
		LCDConfig lcdConfig;
	};

	class Process {
		unsigned int width;
		unsigned int height;
		unsigned int loopDelay;
		bool running = false;
		LiquidCrystal lcd;
		Renderer renderer;
		TextureRegistry textureRegistry;
		CollisionSystem collisionSystem;

	public:
		Process(const ProcessConfig& cfg);

		void start(Component* root);

		void loop();

		CollisionSystem& getCollisionSystem();

		const CollisionSystem& getCollisionSystem() const;

		TextureID createTexture(Utils::Array<byte, 8> textureData);
	};
	
	class Trigger {
		bool isActive;
		bool didFire = true;

	protected:
		Trigger(bool initial);

		void active();

		virtual bool checkActive(unsigned int dt) = 0;

	public:
		bool state();

		bool fired();

		void update(unsigned int dt);
	};

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

#include "AGE_components.h"

#endif