#include <LiquidCrystal.h>

#define AGE_DEBUG

#include "src/AGE.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::EventManager eventManager;
const AGE::Event TEST_EVT = eventManager.newEvent();

AGE::Text text(u"ナニカ　カ゛　オコッタ。", 0, 0);

void onEvent() {
	text.draw(lcd);
}

void setup() {
	lcd.begin(16, 2);
	
	eventManager.on(TEST_EVT, onEvent);

	eventManager.dispatch(TEST_EVT);
}

void loop() {
	
}