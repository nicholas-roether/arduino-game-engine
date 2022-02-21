#include <LiquidCrystal.h>

#include "src/AGE.h"
#include "src/AGE_structures.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::Renderer renderer(16, 2);

class TestComponent : public AGE::Component {
	unsigned int time = 0;
	unsigned int lastTime = 0;
	AGE::Text text = { "", 0, 0 };

public:
	void build() {
		String numStr(time);
		text = AGE::Text(numStr.c_str(), 0, 0);
		addChild(&text);
	}

	bool shouldRebuild() {
		return time != lastTime;
	}

	void didRebuild() {
		lastTime = time;
	}

	void draw(AGE::CharacterBuffer& buffer) {
		time++;
	}
};

TestComponent testComponent;

void setup() {
	lcd.begin(16, 2);

	renderer.setRoot(&testComponent);
}

void loop() {
	renderer.render(lcd);
	delay(100);
}