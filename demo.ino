#include "src/AGE.h"

AGE::Process process({
	20, 4, 					// 20 x 4 LCD display
	20, 					// 20 ticks per second
	9,						// Audio output is on pin 9
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

// Texture creation
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

// Sound effects
AGE::SoundEffect startupSound = {
	3,
	AGE::Tone{ 440, 200 },
	AGE::Tone{ 555, 70 },
	AGE::Tone{ 880, 100 }
};

AGE::SoundEffect shootingSound = {
	3,
	AGE::Tone{ 700, 10 },
	AGE::Tone{ 650, 10 },
	AGE::Tone{ 625, 10 }
};

AGE::SoundEffect obstacleHitSound {
	1,
	AGE::Tone{ 300, 100 }
};

AGE::SoundEffect enemyHitSound {
	2,
	AGE::Tone{ 440, 70 },
	AGE::Tone{ 659, 100 }
};

AGE::SoundEffect deathSound {
	3,
	AGE::Tone{ 440, 100 },
	AGE::Tone{ 330, 100 },
	AGE::Tone{ 220, 200 }
};

unsigned int score = 0; // The player's current score

AGE::SaveData<unsigned int> highScore; // The player's high score, saved between games

/**
 * @brief A class for stuff flying across the screen
 * 
 * @details A Base class for Bullet, Enemy, and Obstacle - Projectiles move across the
 * 		Screen horizontally at a fixed y position, with a fixed x velocity.
 */
class Projectile : public AGE::SpawnableComponent, public AGE::Collider {
protected:
	float xPos; // xPos is a float, since it is involved in physics calculations.
	uint8_t yPos;
	AGE::Velocity xVel;

public:
	virtual ~Projectile() = default;

	Projectile(uint8_t xPos, uint8_t yPos, float xVel, uint8_t colliderType)
		: AGE::Collider(collisionSystem, colliderType), xPos(xPos), yPos(yPos), xVel(xVel) {}

	void update(unsigned int dt) {
		xVel.update(dt, xPos);
		if (xPos < 0 || xPos >= process.getWidth()) die(); // die when the projectile leaves the screen
	}

	AGE::Position getPos() {
		return { xPos, yPos };
	}
};

/**
 * @brief The bullets the player shoots
 */
class Bullet : public Projectile {
public:
	Bullet(uint8_t yPos)
		: Projectile(2, yPos, 18, BULLET_COLLIDER) {}

	void draw(AGE::CharacterBuffer& charBuffer) {
		// xPos is a float since it is used for physics stuff, so it has to be rounded.
		charBuffer.put(TEX_BULLET, round(xPos), yPos);
	}

	void update(unsigned int dt) {
		Projectile::update(dt); // base class needs to be updated explicitly
		// Bullets disappear when hitting anything
		if (collides(OBSTACLE_COLLIDER) || collides(ENEMY_COLLIDER)) die();
	}
};

/**
 * @brief The component responsible for spawning the bullets the player shoots
 */
class BulletSpawner : public AGE::Component {
	uint8_t* yPos; // Points to the yPos member of the Player component
	AGE::Spawner spawner = { 3 }; // Max. 3 Bullets at a time

public:
	BulletSpawner(uint8_t* yPos) : yPos(yPos) {}

	void build() {
		addChild(&spawner);
	}

	void update(unsigned int dt) {
		if (shootTrigger.fired()) {
			spawner.spawn(Bullet{ *yPos });
			process.playSound(shootingSound);
		}
	}
};

/**
 * @brief The flashing fire part of the player sprite
 */
class PlayerFire : public AGE::Component {
	uint8_t* yPos;
	AGE::Animation animation = 200;

public:
	PlayerFire(uint8_t* yPos) : yPos(yPos) {}

	void draw(AGE::CharacterBuffer& charBuffer) {
		if (animation.progress() < 0.5) charBuffer.put(TEX_PLAYER_FIRE, 0, *yPos);
	}
};

/**
 * @brief The Player sprite.
 * 
 * @details The Player component includes the spaceship texture, the PlayerFire and the BulletSpawner,
 * 		and is responsible for initiating a game over and managing the high score.
 */
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
		if (upTrigger.fired() && yPos != 0) yPos--; // Move up
		if (downTrigger.fired() && yPos != 3) yPos++; // Move down
		// Game over when colliding with an obstacle or enemy
		if (collides(OBSTACLE_COLLIDER) || collides(ENEMY_COLLIDER)) {
			if (score > highScore.get()) highScore.set(score); // Update high score
			process.setScene(GAME_OVER_SCENE);
			process.playSound(deathSound);
		}
	}

	AGE::Position getPos() {
		return { 1, yPos };
	}
};

/**
 * @brief The indestructable obstacles flying towards the player
 */
class Obstacle : public Projectile {
public:
	Obstacle(uint8_t yPos)
		: Projectile(process.getWidth() - 1, yPos, -8, OBSTACLE_COLLIDER) {} // Speed of -8 characters/second

	void draw(AGE::CharacterBuffer& charBuffer) {
		// xPos is a float since it is used for physics stuff, so it has to be rounded.
		charBuffer.put(TEX_OBSTACLE, round(xPos), yPos);
	}

	void update(unsigned int dt) {
		Projectile::update(dt);
		if (collides(BULLET_COLLIDER))
			process.playSound(obstacleHitSound);
	}
};

/**
 * @brief The killable enemies flying towards the player.
 * 
 * @details Enemy components are also responsible for updating the player's score
 *		when they are killed.
 */
class Enemy : public Projectile {
	AGE::Animation animation = { 1000 }; // Animation period of 1000ms

public:
	Enemy(uint8_t yPos)
		: Projectile(process.getWidth() - 1, yPos, -8, ENEMY_COLLIDER) {}

	void draw(AGE::CharacterBuffer& charBuffer) {
		// Use different textures depending on the animation progress
		// xPos is a float since it is used for physics stuff, so it has to be rounded.
		charBuffer.put(animation.progress() < 0.5 ? TEX_ENEMY_1 : TEX_ENEMY_2, round(xPos), yPos);
	}

	void update(unsigned int dt) {
		Projectile::update(dt);
		// Enemies die when hit by a bullet from the player.
		if (collides(BULLET_COLLIDER)) {
			score += 100; // 100 score points!
			process.playSound(enemyHitSound);
			die();
		}
	}
};

// Fires on average 0.4 times per second.
AGE::RandomTrigger obstacleSpawnTrigger = { 0.4 };

/**
 * @brief The component responsible for spawning obstacles and enemies.
 */
class ObstacleSpawner : public AGE::Component {
	AGE::Spawner spawner = { 8 }; // Max. 8 obstacles and/or enemies at a time.
	
public:
	void build() {
		addChild(&spawner);
	}

	void update(unsigned int dt) {
		if (obstacleSpawnTrigger.fired()) {
			// Different spawn patterns
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

/**
 * @brief A component displaying the player's current score in the top right corner (used in GameScene)
 */
class ScoreDisplay : public AGE::Component {
public:
	void draw(AGE::CharacterBuffer& charBuffer) {
		String scoreStr = String(score);
		charBuffer.write(scoreStr.c_str(), process.getWidth() - scoreStr.length(), 0);
	}
};


/**
 * @brief The scene in which gameplay occurs.
 */
class GameScene : public AGE::Component {
	Player player;
	ObstacleSpawner obstacleSpawner;
	ScoreDisplay scoreDisplay;

public:
	GameScene() {
		score = 0; // reset the score when the scene starts
	}

	void build() {
		addChild(&obstacleSpawner);
		addChild(&player);
		addChild(&scoreDisplay);
	}
};

/**
 * @brief The menu scene visible on game startup
 */
class MenuScene : public AGE::Component {
public:
	void draw(AGE::CharacterBuffer& charBuffer) {
		// TODO proper text alignment api
		charBuffer.write("SPACE DEMO", process.getWidth() / 2 - 5, 1);
		charBuffer.write("shoot to start", process.getWidth() / 2 - 7, 2);
	}

	void update(unsigned int dt) {
		// Start gameplay when shoot button is pressed
		if (shootTrigger.fired()) {
			process.setScene(GAME_SCENE);
			process.playSound(startupSound); // play startup sound
		}
	}
};

/**
 * @brief A component displaying the player's score, blinking, in the center of the 3rd row
 * 		(Used in GameOverScene).
 */
class LastScore : public AGE::Component {
	AGE::Animation blinking = { 1000 }; // blinking animation period: 1000ms

public:
	void draw(AGE::CharacterBuffer& charBuffer) {
		if (blinking.progress() > 0.5) return; // Don't display anything 50% of the time
		String scoreStr = String(score);
		// TODO proper text alignment api
		charBuffer.write(scoreStr.c_str(), process.getWidth() / 2 - scoreStr.length() / 2, 1);
	}
};

/**
 * @brief A component displaying the player's saved high score, or "new highscore!" when applicable.
 */
class HighScore : public AGE::Component {
public:
	void draw(AGE::CharacterBuffer& charBuffer) {
		String scoreStr;
		// when the current score equals the saved highscore, a new highscore was achieved.
		if (score == highScore.get()) scoreStr = "new highscore!";
		else scoreStr = "HI: " + String(highScore.get());
		// TODO proper text alignment api
		charBuffer.write(scoreStr.c_str(), process.getWidth() / 2 - scoreStr.length() / 2, 2);
	}
};

/**
 * @brief The scene that shows when the player dies
 * 
 * @details GameOverScene shows a bunch of text, the player's achieved score, and the player's highscore.
 * 		It also allows the player to retry by pressing the shoot button.
 */
class GameOverScene : public AGE::Component {
	LastScore lastScore;
	HighScore highScore;

public:
	void build() {
		addChild(&lastScore);
		addChild(&highScore);
	}

	void draw(AGE::CharacterBuffer& charBuffer) {
		charBuffer.write("GAME  OVER", process.getWidth() / 2 - 5, 0);
		charBuffer.write("shoot to retry", process.getWidth() / 2 - 7, 3);
	}

	void update(unsigned int dt) {
		// Retry when pressing shoot
		if (shootTrigger.fired()) {
			process.setScene(GAME_SCENE);
			process.playSound(startupSound);
		}
	}
};

// The game class is responsible for choosing the correct scene to show.
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

Game game; // Game instanciation

void setup() {
	DEBUG_START; // Debug mode init; does nothing unless debug mode is enabled.
	
	// Register all triggers
	process.registerTrigger(&shootTrigger);
	process.registerTrigger(&upTrigger);
	process.registerTrigger(&downTrigger);
	process.registerTrigger(&obstacleSpawnTrigger);

	// Start the game and set startup scene
	process.start(&game);
	process.setScene(MENU_SCENE);
}

void loop() {
	process.loop();
}