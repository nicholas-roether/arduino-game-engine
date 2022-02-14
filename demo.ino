#include <LiquidCrystal.h>
#include "src/AGE.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::Text text(u"öhm", 0, 0);

void setup() {
	lcd.begin(16, 2);
	text.draw(lcd);
}

void loop() {

}