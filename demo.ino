#include "src/AGE.h"

AGE::Process process({
	20, 4,
	20,
	{ 12, 11, 5, 4, 3, 2 }
});

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

class Bullet : public AGE::SpawnableComponent {
	static constexpr float X_VELOCITY = 18;

	AGE::Texture texture = { TEX_BULLET };
	uint8_t xPos = 2;
	uint8_t time = 0;

public:
	Bullet() = default;

	Bullet(uint8_t yPos) {
		texture.setY(yPos);
	}

	void build() {
		addChild(&texture);
	}

	void update(unsigned int dt) {
		time += dt;
		if (time > 1000 / X_VELOCITY) {
			xPos++;
			time = 0;
		}
		if (xPos >= process.getWidth()) die();
		texture.setX(xPos);

	}
};

class BulletSpawner : public AGE::Component {
	const uint8_t* yPos;

	AGE::Spawner spawner;

	AGE::ClickTrigger shootTrigger = { 6 };

public:
	BulletSpawner(const uint8_t* yPos) : yPos(yPos) {}

	void build() {
		addChild(&spawner);
	}

	void update(unsigned int dt) {
		shootTrigger.update(dt);
		if (shootTrigger.fired()) spawner.spawn(Bullet{ *yPos });
	}
};

class PlayerFire : public AGE::Component {
	static constexpr unsigned int ANIM_STEP_DURATION = 100;
	const uint8_t* yPos;
	unsigned int animationTime = 0;

	AGE::Texture texture = { TEX_PLAYER_FIRE, 0, *yPos };
	AGE::Toggled toggled = &texture;

public:
	PlayerFire(const uint8_t* yPos) : yPos(yPos) {}

	void build() {
		addChild(&toggled);
	}

	void update(unsigned int dt) {
		animationTime += dt;
		if (animationTime >= ANIM_STEP_DURATION) {
			toggled.toggle();
			animationTime = 0;
		}
		texture.setY(*yPos);
	}
};

class Player : public AGE::Component {
	uint8_t yPos = 0;

	PlayerFire fire = { &yPos };
	AGE::Texture spaceship = { TEX_PLAYER_SPACESHIP, 1, yPos };
	BulletSpawner bulletSpawner = { &yPos };

	AGE::ClickTrigger upTrigger = { 7 };
	AGE::ClickTrigger downTrigger = { 8 };

public:
	void build() {
		addChild(&fire);
		addChild(&spaceship);
		addChild(&bulletSpawner);
	}

	void update(unsigned int dt) {
		upTrigger.update(dt);
		downTrigger.update(dt);

		if (upTrigger.fired() && yPos != 0) yPos--;
		if (downTrigger.fired() && yPos != 3) yPos++;

		spaceship.setY(yPos);
	}
};

Player player;

void setup() {
	Serial.begin(115200);
	Serial.println("Program started");

	process.start(&player);
}

void loop() {
	process.loop();
}