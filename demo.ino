#include <LiquidCrystal.h>
#include "src/AGE.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::Text text1(u"moin", 0, 0);
AGE::Text text2(u"jungs", 0, 1);
AGE::Group group(2);

void setup() {
	lcd.begin(16, 2);

	group.add(&text1);
	group.add(&text2);

	group.draw(lcd);
}

void loop() {

}