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
		bool rebuildRequested = true;
	
	protected:
		void addChild(Component* child);

		void requestRebuild();

	public:
		const Utils::List<Component*>& getChildren() const;

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
		Component* root;
		unsigned int lastRender = 0;

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

		unsigned int getWidth();
	
		unsigned int getHeight();

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

	template<typename T>
	class Prop {
		T* ptr;
		bool ownership;

	public:
		Prop(const T& val) : ptr(malloc(sizeof(T))), ownership(true) {
			*ptr = val;
		}

		Prop(T* ptr) : ptr(ptr), ownership(false) {}

		~Prop() {
			if (ownership) free(ptr);
		}

		T& value() {
			return *ptr;
		}

		const T& value() const {
			return *ptr;
		}
	};
}

#include "AGE_library.h"

#endif