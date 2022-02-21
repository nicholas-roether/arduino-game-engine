#include <LiquidCrystal.h>

#include "src/AGE.h"
#include "src/AGE_structures.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::Renderer renderer(16, 2);

class TestComponent : public AGE::Component {
	unsigned int time = 0;
	AGE::Text text;

public:
	void build() {
		text.setText(String(time));
		addChild(&text);
	}

	void update(unsigned int dt) {
		time += dt;
		setState();
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