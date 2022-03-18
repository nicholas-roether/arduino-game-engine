#include "AGE_debug.h"
#include <stdarg.h>
#include <stdio.h>

#define VARARGS_START(start) va_list args; va_start(args, start)

#define VARARGS_END va_end(args)

#define LOG_DELAY delayMicroseconds(10000)

static void serialPrintF(const String& str, va_list args) {
	char* buffer = new char[32]; // can't really afford long messages
	int ret = vsprintf(buffer, str.c_str(), args);
	if (ret < 0) {
		Serial.println();
		Serial.print("[ERROR] Error while formatting serial message string (errcode: ");
		Serial.print(ret);
		Serial.println(")");
	}
	Serial.print(buffer);
	delete[] buffer;
}

void __debugLog(const String& message) {
	Serial.print("[DEBUG] ");
	Serial.println(message);
	LOG_DELAY;
}

void __debugLogF(const String& message, ...) {
	VARARGS_START(message);
	Serial.print("[DEBUG] ");
	serialPrintF(message, args);
	Serial.println();
	VARARGS_END;
	LOG_DELAY;
}

void __error(const String& message) {
	Serial.print("[ERROR] ");
	Serial.println(message);
	LOG_DELAY;
	abort();
}

void __errorF(const String& message, ...) {
	VARARGS_START(message);
	Serial.print("[ERROR] ");
	serialPrintF(message, args);
	Serial.println();
	VARARGS_END;
	LOG_DELAY;
	abort();
}

void __assert(bool cond, const String& message) {
	if (cond) return;
	Serial.print("[ERROR] ");
	Serial.println(message);
	LOG_DELAY;
	abort();
}

void __assertF(bool cond, const String& message, ...) {
	if (cond) return;
	VARARGS_START(message);
	Serial.print("[ERROR] ");
	serialPrintF(message, args);
	Serial.println();
	VARARGS_END;
	LOG_DELAY;
	abort();
}

void __debugLog(const char* message) {
	Serial.print("[DEBUG] ");
	Serial.println(message);
	LOG_DELAY;
}

void __debugLogF(const char* message, ...) {
	VARARGS_START(message);
	Serial.print("[DEBUG] ");
	serialPrintF(message, args);
	Serial.println();
	VARARGS_END;
	LOG_DELAY;
}

void __error(const char* message) {
	Serial.print("[ERROR] ");
	Serial.println(message);
	LOG_DELAY;
	abort();
}

void __errorF(const char* message, ...) {
	VARARGS_START(message);
	Serial.print("[ERROR] ");
	serialPrintF(message, args);
	Serial.println();
	VARARGS_END;
	LOG_DELAY;
	abort();
}

void __assert(bool cond, const char* message) {
	if (cond) return;
	Serial.print("[ERROR] ");
	Serial.println(message);
	LOG_DELAY;
	abort();
}

void __assertF(bool cond, const char* message, ...) {
	if (cond) return;
	VARARGS_START(message);
	Serial.print("[ERROR] ");
	serialPrintF(message, args);
	Serial.println();
	VARARGS_END;
	LOG_DELAY;
	abort();
}