#include <LiquidCrystal.h>

#define AGE_DEBUG

#include "src/AGE.h"
#include "src/AGE_structures.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::EventManager eventManager;
const AGE::Event TEST_EVT = eventManager.newEvent();

AGE::Group group;

void onEvent() {
	group.draw(lcd);
}

void setup() {
	lcd.begin(16, 2);

	Serial.begin(115200);
	AGE::Utils::Buffer buffer;
	// group.add(AGE::Text(u"ナニカ　カ゛　オコッタ。", 0, 0));
	// group.add(AGE::Text(u"コレ、イガイト　ムス゛カシイ。", 0, 1));

	// AGE::Text* text2 = (AGE::Text*) malloc(sizeof(AGE::Text));
	AGE::Text text = AGE::Text(u"ナニカ　カ゛　オコッタ。", 0, 0);
	AGE::Text* textPtr = (AGE::Text*) malloc(sizeof(text));
	// *textPtr = text;
	memcpy(textPtr, &text, sizeof(text));
	delay(100);
	textPtr->draw(lcd);
	
	// eventManager.on(TEST_EVT, onEvent);

	// eventManager.dispatch(TEST_EVT);
}

void loop() {
	
}