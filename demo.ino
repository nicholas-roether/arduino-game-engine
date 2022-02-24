#include "src/AGE.h"

AGE::Process process({
	20, 4,
	20,
	{ 12, 11, 5, 4, 3, 2 }
});

AGE::TextureID TEX_PLAYER = process.createTexture({
	B00000,
	B11100,
	B00110,
	B01111,
	B01111,
	B00110,
	B11100,
	B00000
});

class Player : public AGE::Component {
	AGE::Texture texture = { TEX_PLAYER };

	void build() {
		addChild(&texture);
	}
};

Player player;

void setup() {
	process.start(&player);
}

void loop() {
	process.loop();
}