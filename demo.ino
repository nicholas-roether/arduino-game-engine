#include <LiquidCrystal.h>

#include "src/AGE.h"
#include "src/AGE_structures.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::Renderer renderer(16, 2);

class TestComponent : public AGE::Component {
	unsigned int count = 0;
	AGE::ClickTrigger clickTrigger = { 8, AGE::BTN_UP };
	AGE::Text text;

public:
	void build() {
		text.setText(String(count));
		addChild(&text);
	}

	void update(unsigned int dt) {
		clickTrigger.update(dt);
		if (clickTrigger.fired()) {
			count++;
			setState();
		}
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