#include <LiquidCrystal.h>

#define AGE_DEBUG

#include "src/AGE.h"
#include "src/AGE_text.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
AGE::EventManager eventManager;
static const unsigned int TEST_EVT = eventManager.newEvent();

void onEvent() {
	lcd.write(AGE::Utils::LCDString(u"イヘ゛ント　カ゛　オコッタ。").c_str());
}

void setup() {
	// TODO export this to an init function or so
	#ifdef AGE_DEBUG
	Serial.begin(115200);
	#endif

	lcd.begin(16, 2);
	
	eventManager.on(TEST_EVT, onEvent);

	eventManager.dispatch(TEST_EVT);
}

void loop() {
	
}