#include <LiquidCrystal.h>
#include "src/AGE.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

AGE::Text text1(u"öhm", 0, 0);
AGE::Group group(2);

void setup() {
	Serial.begin(115200);
	lcd.begin(16, 2);

	// AGE::Array<AGE::Text> test;

	// test.push(text1);

	// Serial.print("text1 hex: ");
	// printHexValue(&text1);
	// Serial.println();

	// Serial.print("array value hex: ");
	// printHexValue(&test[0]);
	// Serial.println();

	AGE::Text text2 = text1;
	AGE::Text text3 = text2;
	AGE::Text text4 = text3;
	text4.draw(lcd);
	printHexValue(&text1);
	Serial.println();
	printHexValue(&text2);
	Serial.println();
	printHexValue(&text3);
	Serial.println();
	printHexValue(&text4);
	Serial.println();
}

void loop() {

}