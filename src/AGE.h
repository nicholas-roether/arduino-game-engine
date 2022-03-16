#ifndef _AGE_H_
#define _AGE_H_

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "AGE_text.h"
#include "AGE_utils.h"
#include "AGE_physics.h"
#include "AGE_debug.h"

namespace AGE {
	class CharacterBuffer;

	class Component {
		Utils::List<Component*> children;
		bool rebuildRequested = true;
	
	protected:
		void addChild(Component* child);

		void requestRebuild();

	public:
		virtual ~Component() = default;

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

		void swapCharBuffers();

		void build(Component* component);

		void update(Component* component, unsigned int dt);

		void render(Component* component);

	public:
		Renderer() = delete;
		Renderer(size_t width, size_t height);

		void setRoot(Component* root);

		void render(LiquidCrystal& lcd, unsigned int dt);
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
		unsigned long lastLoop;
		bool running = false;
		LiquidCrystal lcd;
		Renderer renderer;
		TextureRegistry textureRegistry;
		CollisionSystem collisionSystem;
		Utils::List<Trigger*> triggers;

	public:
		Process(const ProcessConfig& cfg);

		void start(Component* root);

		void loop();

		unsigned int getWidth();
	
		unsigned int getHeight();

		CollisionSystem& getCollisionSystem();

		const CollisionSystem& getCollisionSystem() const;

		TextureID createTexture(Utils::Array<byte, 8> textureData);

		void registerTrigger(Trigger* trigger);
	};
	
	class Trigger {
		bool isActive;
		bool didFire = true;

	protected:
		Trigger(bool initial);

		virtual ~Trigger() = default;

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
		bool isOwned;

	public:
		Prop(const T& val) : isOwned(true) {
			ptr = new T(val);
		}

		Prop(T* ptr) : ptr(ptr), isOwned(false) {}

		~Prop() {
			if (isOwned) delete ptr;
		}

		Prop(const Prop<T>& other) : isOwned(other.isOwned) {
			if (isOwned) ptr = new T(*other.ptr);
			else ptr = other.ptr;
		}

		Prop<T>& operator=(const Prop<T>& other) {
			if (isOwned) delete ptr;
			isOwned = other.isOwned;
			if (isOwned) ptr = new T(*other);
			else ptr = other.ptr;
		}

		T& operator*() {
			return value();
		}

		const T& operator*() const {
			return value();
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