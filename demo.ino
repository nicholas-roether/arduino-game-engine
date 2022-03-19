#include "src/AGE.h"

AGE::Process process({
	20, 4, 					// 20 x 4 LCD display
	20, 					// 20 ticks per second
	/**
	 * LCD configuration:
	 * 
	 * RS 		-> 	Pin 12
	 * EN 		->	Pin 11
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

// Collision stuff
AGE::CollisionSystem collisionSystem;

enum ColliderType {
	PLAYER_COLLIDER,
	OBSTACLE_COLLIDER,
	ENEMY_COLLIDER,
	BULLET_COLLIDER
};

// Scene stuff
enum Scene {
	MENU_SCENE,
	GAME_SCENE,
	GAME_OVER_SCENE
};

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

AGE::TextureID TEX_ENEMY_1 = process.createTexture({
	B00000,
	B00000,
	B01010,
	B11111,
	B10101,
	B11111,
	B10001,
	B00000
});

AGE::TextureID TEX_ENEMY_2 = process.createTexture({
	B00000,
	B01010,
	B11111,
	B10111,
	B11101,
	B11111,
	B10001,
	B00000
});

class Projectile : public AGE::SpawnableComponent, public AGE::Collider {
protected:
	float xPos;
	uint8_t yPos;
	AGE::Velocity xVel;

public:
	virtual ~Projectile() = default;

	Projectile(uint8_t xPos, uint8_t yPos, float xVel, uint8_t colliderType)
		: AGE::Collider(collisionSystem, colliderType), xPos(xPos), yPos(yPos), xVel(xVel) {}

	void update(unsigned int dt) {
		xVel.update(dt, xPos);
		if (xPos < 0 || xPos >= process.getWidth()) die();
	}

	AGE::Position getPos() {
		return { xPos, yPos };
	}
};

class Bullet : public Projectile {
public:
	Bullet(uint8_t yPos)
		: Projectile(2, yPos, 18, BULLET_COLLIDER) {}

	void draw(AGE::CharacterBuffer& charBuffer) {
		charBuffer.put(TEX_BULLET, xPos, yPos);
	}

	void update(unsigned int dt) {
		Projectile::update(dt);
		if (collides(OBSTACLE_COLLIDER)) die();
		if (collides(ENEMY_COLLIDER)) die();
	}
};

class BulletSpawner : public AGE::Component {
	uint8_t* yPos;
	AGE::Spawner spawner = { 3 };

public:
	BulletSpawner(uint8_t* yPos) : yPos(yPos) {}

	void build() {
		addChild(&spawner);
	}

	void update(unsigned int dt) {
		if (shootTrigger.fired()) spawner.spawn(Bullet{ *yPos });
	}
};

class PlayerFire : public AGE::Component {
	uint8_t* yPos;
	AGE::Animation animation = 200;

public:
	PlayerFire(uint8_t* yPos) : yPos(yPos) {}

	void draw(AGE::CharacterBuffer& charBuffer) {
		if (animation.progress() < 0.5) charBuffer.put(TEX_PLAYER_FIRE, 0, *yPos);
	}
};

class Player : public AGE::Component, public AGE::Collider {
	uint8_t yPos = 0;

	PlayerFire fire = { &yPos };
	BulletSpawner bulletSpawner = { &yPos };

public:
	Player() : AGE::Collider(collisionSystem, PLAYER_COLLIDER) {}

	void build() {
		addChild(&fire);
		addChild(&bulletSpawner);
	}

	void draw(AGE::CharacterBuffer& charBuffer) {
		charBuffer.put(TEX_PLAYER_SPACESHIP, 1, yPos);
	}

	void update(unsigned int dt) {
		if (upTrigger.fired() && yPos != 0) yPos--;
		if (downTrigger.fired() && yPos != 3) yPos++;
		if (collides(OBSTACLE_COLLIDER) || collides(ENEMY_COLLIDER))
			process.setScene(GAME_OVER_SCENE);
	}

	AGE::Position getPos() {
		return { 1, yPos };
	}
};

class Obstacle : public Projectile {
public:
	Obstacle(uint8_t yPos)
		: Projectile(process.getWidth() - 1, yPos, -8, OBSTACLE_COLLIDER) {}

	void draw(AGE::CharacterBuffer& charBuffer) {
		charBuffer.put(TEX_OBSTACLE, xPos, yPos);
	}
};

class Enemy : public Projectile {
	AGE::Animation animation = { 1000 };

public:
	Enemy(uint8_t yPos)
		: Projectile(process.getWidth() - 1, yPos, -8, ENEMY_COLLIDER) {}

	void draw(AGE::CharacterBuffer& charBuffer) {
		charBuffer.put(animation.progress() < 0.5 ? TEX_ENEMY_1 : TEX_ENEMY_2, xPos, yPos);
	}

	void update(unsigned int dt) {
		Projectile::update(dt);
		if (collides(BULLET_COLLIDER)) die();
	}
};

AGE::RandomTrigger obstacleSpawnTrigger = { 0.4 };

class ObstacleSpawner : public AGE::Component {
	AGE::Spawner spawner = { 8 };
	
public:
	void build() {
		addChild(&spawner);
	}

	void update(unsigned int dt) {
		if (obstacleSpawnTrigger.fired()) {
			switch(random(12)) {
				case 0:
					spawner.spawn(Obstacle(0));
					spawner.spawn(Obstacle(3));
					break;
				case 1:
					spawner.spawn(Obstacle(0));
					spawner.spawn(Obstacle(1));
					spawner.spawn(Obstacle(2));
					break;
				case 2:
					spawner.spawn(Obstacle(1));
					spawner.spawn(Obstacle(2));
					spawner.spawn(Obstacle(3));
					break;
				case 3:
					spawner.spawn(Obstacle(1));
					spawner.spawn(Obstacle(2));
					break;
				case 4:
					spawner.spawn(Obstacle(0));
					spawner.spawn(Obstacle(1));
					spawner.spawn(Obstacle(2));
					spawner.spawn(Enemy(3));
					break;
				case 5:
					spawner.spawn(Enemy(0));
					spawner.spawn(Obstacle(1));
					spawner.spawn(Obstacle(2));
					spawner.spawn(Obstacle(3));
					break;
				case 6:
					spawner.spawn(Enemy(0));
					spawner.spawn(Enemy(1));
					spawner.spawn(Enemy(2));
					spawner.spawn(Enemy(3));
					break;
				case 7:
					spawner.spawn(Enemy(0));
					spawner.spawn(Obstacle(1));
					spawner.spawn(Enemy(2));
					spawner.spawn(Enemy(3));
					break;
				case 8:
					spawner.spawn(Enemy(0));
					spawner.spawn(Enemy(1));
					spawner.spawn(Obstacle(2));
					spawner.spawn(Enemy(3));
					break;
				case 9:
					spawner.spawn(Enemy(0));
					spawner.spawn(Obstacle(1));
					spawner.spawn(Obstacle(2));
					spawner.spawn(Enemy(3));
					break;
				case 10:
					spawner.spawn(Enemy(1));
					spawner.spawn(Enemy(2));
					break;
				case 11:
					spawner.spawn(Obstacle(0));
					spawner.spawn(Enemy(1));
					spawner.spawn(Enemy(2));
					spawner.spawn(Obstacle(3));
					break;
			}
		}
	}
};

class GameScene : public AGE::Component {
	Player player;
	ObstacleSpawner obstacleSpawner;

public:
	void build() {
		addChild(&obstacleSpawner);
		addChild(&player);
	}
};

class MenuScene : public AGE::Component {
public:
	void draw(AGE::CharacterBuffer& charBuffer) {
		charBuffer.write("SPACE DEMO", process.getWidth() / 2 - 5, 1);
		charBuffer.write("shoot to start", process.getWidth() / 2 - 7, 2);
	}

	void update(unsigned int dt) {
		if (shootTrigger.fired()) process.setScene(GAME_SCENE);
	}
};

class GameOverScene : public AGE::Component {
public:
	void draw(AGE::CharacterBuffer& charBuffer) {
		charBuffer.write("GAME  OVER", process.getWidth() / 2 - 5, 1);
		charBuffer.write("shoot to retry", process.getWidth() / 2 - 7, 2);
	}

	void update(unsigned int dt) {
		if (shootTrigger.fired()) process.setScene(GAME_SCENE);
	}
};

class Game : public AGE::Game {
protected:
	AGE::Component* buildScene(AGE::SceneID id) {
		switch(id) {
			case MENU_SCENE:
				return new MenuScene();
				break;
			case GAME_SCENE:
				return new GameScene();
				break;
			case GAME_OVER_SCENE:
				return new GameOverScene();
				break;
			default:
				return nullptr;
		}
	}
};

Game game;

void setup() {
	DEBUG_START;
	
	process.registerTrigger(&shootTrigger);
	process.registerTrigger(&upTrigger);
	process.registerTrigger(&downTrigger);
	process.registerTrigger(&obstacleSpawnTrigger);

	
	process.start(&game);
	process.setScene(MENU_SCENE);
}

void loop() {
	process.loop();
}