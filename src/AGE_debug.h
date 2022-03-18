#ifndef _AGE_DEBUG_H
#define _AGE_DEBUG_H

#include <Arduino.h>

#define SERIAL_BAUD_RATE 115200

#define DEBUG_LOG(str)
#define DEBUG_LOG_F(str, ...)

#define ERROR(message) abort()
#define ERROR_F(message, ...) abort()

#define ASSERT(cond, message) if(!(cond)) abort()
#define ASSERT_F(cond, message, ...) if(!(cond)) abort()

#define DEBUG_START

#ifdef AGE_DEBUG_MODE
	void __debugLog(const String& message);

	void __debugLogF(const String& message, ...);

	void __error(const String& message);

	void __errorF(const String& message, ...);

	void __assert(bool cond, const String& message);

	void __assertF(bool cond, const String& message, ...);

	#define DEBUG_LOG(str) __debugLog(str)
	#define DEBUG_LOG_F(str, ...) __debugLogF(str, __VA_ARGS__)

	#define ERROR(message) __error(message)
	#define ERROR_F(message, ...) __errorF(message, __VA_ARGS__)

	#define ASSERT(cond, message) __assert(cond, message)
	#define ASSERT_F(cond, message, ...) __assertF(cond, message, __VA_ARGS__)

	#define DEBUG_START Serial.begin(SERIAL_BAUD_RATE)

#endif

#endif