#include "src/AGE.h"

AGE::Process process({
	20, 4, 					// 20 x 4 LCD display
	20, 					// 20 ticks per second
	/**
	 * LCD configuration:
	 * 
	 * R/W 		-> 	Pin 12
	 * Reset 	->	Pin 11
	 * Data 0	-> 	Pin 5
	 * Data 1	-> 	Pin 4
	 * Data 2	->	Pin 3
	 * Data 3	->	Pin 2
	 */
	{ 12, 11, 5, 4, 3, 2 }
});

AGE::ClickTrigger shootTrigger 	= { 6 }; // Pin 6: shoot button
AGE::ClickTrigger upTrigger 	= { 7 }; // Pin 7: move up button
AGE::ClickTrigger downTrigger 	= { 8 }; // Pin 8: move down button

AGE::TextureID TEX_PLAYER_SPACESHIP = process.createTexture({
	B00000,
	B11100,
	B00110,
	B01111,
	B01111,
	B00110,
	B11100,
	B00000
});

AGE::TextureID TEX_PLAYER_FIRE = process.createTexture({
	B00000,
	B00000,
	B00001,
	B01111,
	B00111,
	B00001,
	B00000,
	B00000
});

AGE::TextureID TEX_BULLET = process.createTexture({
	B00000,
	B11110,
	B00000,
	B00000,
	B00000,
	B00000,
	B11110,
	B00000
});

AGE::TextureID TEX_OBSTACLE = process.createTexture({
	B00000,
	B00100,
	B01110,
	B11111,
	B11111,
	B01110,
	B00100,
	B00000
});

class Bullet : public AGE::SpawnableComponent {
	static constexpr int8_t X_VELOCITY = 18;

	uint8_t xPos = 2;
	AGE::Velocity xVel = { &xPos, X_VELOCITY };
	AGE::Prop<uint8_t> yPos;
	AGE::Texture texture = { TEX_BULLET, &xPos, yPos };

public:
	Bullet(const AGE::Prop<uint8_t>& yPos) : yPos(yPos) {}

	// Copy constructor is absolutely necessary to ensure the texture and velocity have the correct pointer to xPos
	Bullet(const Bullet& other)
		: xPos(other.xPos),
		  xVel(&xPos, X_VELOCITY),
		  yPos(other.yPos),
		  texture(TEX_BULLET, &xPos, yPos)
	{}

	void build() {
		addChild(&texture);
	}

	void update(unsigned int dt) {
		xVel.update(dt);
		if (xPos >= process.getWidth()) die();
	}
};

class BulletSpawner : public AGE::Component {
	AGE::Prop<uint8_t> yPos;

	AGE::Spawner spawner = { 3 };

public:
	BulletSpawner(const AGE::Prop<uint8_t>& yPos) : yPos(yPos) {}

	void build() {
		addChild(&spawner);
	}

	void update(unsigned int dt) {
		if (shootTrigger.fired()) spawner.spawn(Bullet{ *yPos });
	}
};

class PlayerFire : public AGE::Component {
	static constexpr unsigned int ANIM_STEP_DURATION = 100;
	AGE::Prop<uint8_t> yPos;
	unsigned int animationTime = 0;

	AGE::Texture texture = { TEX_PLAYER_FIRE, 0su, yPos };
	AGE::Toggled toggled = &texture;

public:
	PlayerFire(AGE::Prop<uint8_t> yPos) : yPos(yPos) {}

	void build() {
		addChild(&toggled);
	}

	void update(unsigned int dt) {
		animationTime += dt;
		if (animationTime >= ANIM_STEP_DURATION) {
			toggled.toggle();
			animationTime = 0;
		}
	}
};

class Player : public AGE::Component {
	uint8_t yPos = 0;

	PlayerFire fire = { &yPos };
	AGE::Texture spaceship = { TEX_PLAYER_SPACESHIP, 1su, &yPos };
	BulletSpawner bulletSpawner = { &yPos };

public:
	void build() {
		addChild(&fire);
		addChild(&spaceship);
		addChild(&bulletSpawner);
	}

	void update(unsigned int dt) {
		if (upTrigger.fired() && yPos != 0) yPos--;
		if (downTrigger.fired() && yPos != 3) yPos++;
	}
};

class Obstacle : public AGE::SpawnableComponent {
	static constexpr int8_t X_VELOCITY = -8;

	uint8_t xPos = process.getWidth() - 1;
	AGE::Velocity xVel = { &xPos, X_VELOCITY };
	AGE::Prop<uint8_t> yPos;
	AGE::Texture texture = { TEX_OBSTACLE, &xPos, yPos };

public:
	Obstacle(const AGE::Prop<uint8_t>& yPos) : yPos(yPos) {}

	Obstacle(const Obstacle& other)
		: xPos(other.xPos),
		  xVel(&xPos, X_VELOCITY),
		  yPos(other.yPos),
		  texture(TEX_OBSTACLE, &xPos, yPos)
	{}

	void build() {
		addChild(&texture);
	}

	void update(unsigned int dt) {
		xVel.update(dt);
		if (xPos == 0) die();
	}
};

AGE::RandomTrigger obstacleSpawnTrigger = { 0.2 };

class ObstacleSpawner : public AGE::Component {
	AGE::Spawner spawner = { 4 };

public:
	void build() {
		addChild(&spawner);
	}

	void update(unsigned int dt) {
		if (obstacleSpawnTrigger.fired()) {
			spawner.spawn(Obstacle(0su));
			// spawner.spawn(Obstacle(1su));
			spawner.spawn(Obstacle(2su));
			spawner.spawn(Obstacle(3su));
		}
	}
};

class Scene : public AGE::Component {
	Player player;
	ObstacleSpawner obstacleSpawner;

public:
	void build() {
		addChild(&player);
		addChild(&obstacleSpawner);
	}
};

Scene scene;

void setup() {
	DEBUG_START;
	DEBUG_LOG("Setup");
	process.registerTrigger(&shootTrigger);
	process.registerTrigger(&upTrigger);
	process.registerTrigger(&downTrigger);
	process.registerTrigger(&obstacleSpawnTrigger);
	process.start(&scene);
}

void loop() {
	process.loop();
}