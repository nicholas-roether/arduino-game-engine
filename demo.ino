#include "src/AGE.h"

AGE::Process game({
	16, 2,
	10,
	{ 12, 11, 5, 4, 3, 1 }
});

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
	Serial.begin(115200);
	Serial.println("test");
	// game.start(&testComponent);
}

void loop() {
	// game.loop();
}