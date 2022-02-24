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

class Player : public AGE::Component {
	uint8_t yPos = 0;
	AGE::Texture fire = { TEX_PLAYER_FIRE, 0, yPos };
	AGE::Texture spaceship = { TEX_PLAYER_SPACESHIP, 1, yPos };

	AGE::ClickTrigger upTrigger = { 7 };
	AGE::ClickTrigger downTrigger = { 8 };

	void build() {
		addChild(&fire);
		addChild(&spaceship);
	}

	void update(unsigned int dt) {
		upTrigger.update(dt);
		downTrigger.update(dt);

		if (upTrigger.fired() && yPos != 0) yPos--;
		if (downTrigger.fired() && yPos != 3) yPos++;

		fire.setY(yPos);
		spaceship.setY(yPos);
	}
};

Player player;

void setup() {
	process.start(&player);
}

void loop() {
	process.loop();
}