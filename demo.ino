#include <LiquidCrystal.h>

#include "src/AGE.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::Renderer renderer(16, 2);

class TestComponent : public AGE::Component {
	unsigned int count = 0;
	AGE::ClickTrigger clickTrigger = { 8, AGE::BTN_UP };
	AGE::Text text = { "0", 0, 0 };

public:
	void build() {
		addChild(&text);
	}

	void update(unsigned int dt) {
		clickTrigger.update(dt);
		if (clickTrigger.fired()) {
			count++;
			text.setText(String(count));
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