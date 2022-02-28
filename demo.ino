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
	B11110,
	B00000
});

class Bullet : public AGE::SpawnableComponent {
	static constexpr double X_VELOCITY = 1;

	AGE::Texture texture = { TEX_BULLET };
	AGE::PhysicsObject physics;

public:
	Bullet(uint8_t yPos) {
		Serial.println("Bullet::Bullet");
		delay(100);
		physics.setPos({ 3, yPos});
		physics.setVelocity({ 0, 0 });
	}
	
	Bullet& operator=(const Bullet& other) {
		Serial.println("Bullet::operator=");
		delay(100);
		// physics = other.physics;
		// texture = other.texture;
	}

	void build() {
		addChild(&texture);
	}

	void update(unsigned int dt) {
		physics.update(dt);
		if (physics.getPos().x >= process.getWidth()) die();
		texture.setPos(physics.getX(), physics.getY());
	}
};

class BulletSpawner : public AGE::Component {
	const uint8_t* yPos;

	AGE::Spawner<Bullet> spawner;

	AGE::ClickTrigger shootTrigger = { 6 };

public:
	BulletSpawner(const uint8_t* yPos) : yPos(yPos) {}

	void build() {
		addChild(&spawner);
	}

	void draw(AGE::CharacterBuffer& buffer) {
		buffer.write(String(spawner.getSpawnedComponents().size()).c_str(), 19, 0);
	}

	void update(unsigned int dt) {
		shootTrigger.update(dt);
		if (shootTrigger.fired()) spawner.spawn({ *yPos });
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
	process.start(&player);
}

void loop() {
	process.loop();
}