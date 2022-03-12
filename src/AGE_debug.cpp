#include "AGE_debug.h"
#include <stdarg.h>
#include <stdio.h>

#define VARARGS_START(start) va_list args; va_start(args, start)

#define VARARGS_END va_end(args)

static void serialPrintF(const String& str, va_list args) {
	char buffer[64]; // can't really afford long messages
	vsprintf(buffer, str.c_str(), args);
	Serial.print(buffer);
}

void __debugInit() {
	Serial.begin(SERIAL_BAUD_RATE);
}

void __debugLog(const String& message) {
	Serial.print("[DEBUG] ");
	Serial.println(message);
	delay(10);
}

void __debugLogF(const String& message, ...) {
	VARARGS_START(message);
	Serial.print("[DEBUG] ");
	serialPrintF(message, args);
	Serial.println();
	delay(10);
	VARARGS_END;
}

void __error(const String& message) {
	Serial.print("[ERROR] ");
	Serial.println(message);
	delay(10);
	abort();
}

void __errorF(const String& message, ...) {
	VARARGS_START(message);
	Serial.print("[ERROR] ");
	serialPrintF(message, args);
	Serial.println();
	delay(10);
	VARARGS_END;
	abort();
}

void __assert(bool cond, const String& message) {
	if (cond) return;
	Serial.print("[ERROR] ");
	Serial.println(message);
	delay(10);
	abort();
}

void __assertF(bool cond, const String& message, ...) {
	if (cond) return;
	VARARGS_START(message);
	Serial.print("[ERROR] ");
	serialPrintF(message, args);
	Serial.println();
	delay(10);
	VARARGS_END;
	abort();
}