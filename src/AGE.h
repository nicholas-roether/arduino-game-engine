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
		uint8_t width;
		uint8_t height;
		char* characters;

	public:
		CharacterBuffer() = delete;
		CharacterBuffer(uint8_t width, uint8_t height);
		CharacterBuffer(const CharacterBuffer& other);

		~CharacterBuffer();

		CharacterBuffer& operator=(const CharacterBuffer& other);

		char get(uint8_t x, uint8_t y);

		void put(char character, uint8_t x, uint8_t y);

		void write(const char* characters, uint8_t x, uint8_t y);

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

	typedef uint8_t SceneID;

	class Game : public Component {
		SceneID sceneId = 0;
		SceneID sceneShowing = -1;
		Component* scene;

	protected:
		virtual Component* buildScene(SceneID sceneId) = 0;
		
	public:
		Game();

		Game(const Game& other) = delete;
		
		virtual ~Game();

		void build();

		void update(unsigned int dt);

		void setScene(SceneID id);
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
		Utils::List<Trigger*> triggers;
		Game* game;

	public:
		Process(const ProcessConfig& cfg);

		void start(Game* game);

		void loop();

		unsigned int getWidth();
	
		unsigned int getHeight();

		void setScene(SceneID id);

		TextureID createTexture(Utils::Array<byte, 8> textureData);

		void registerTrigger(Trigger* trigger);
	};

	template<typename T>
	class SaveData {
		T* bufferedValue = nullptr;
		Utils::HardStorage storage;

		void readToBuffered() {
			const T* ptr = (const T*) storage.read();
			bufferedValue = new T(*ptr);
		}

	public:
		SaveData() : storage(sizeof(T)) {
			readToBuffered();
		}

		const T& get() {
			return *bufferedValue;
		}

		void set(const T& val) {
			*bufferedValue = val;
			storage.write((const byte*) &val);
		}
	};
}

#include "AGE_library.h"

#endif