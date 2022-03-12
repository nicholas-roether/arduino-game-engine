#ifndef _AGE_DEBUG_H
#define _AGE_DEBUG_H

#include <Arduino.h>

#define DEBUG_INIT
#define SERIAL_BAUD_RATE 115200

#define DEBUG_LOG(str)
#define DEBUG_LOG_F(str, ...)

#define ERROR(message)
#define ERROR_F(message, ...)

#define ASSERT(cond, message)
#define ASSERT_F(cond, message, ...)

#ifdef AGE_DEBUG_MODE
	void __debugInit();

	void __debugLog(const String& message);

	void __debugLogF(const String& message, ...);

	void __error(const String& message);

	void __errorF(const String& message, ...);

	void __assert(bool cond, const String& message);

	void __assertF(bool cond, const String& message, ...);

	#define DEBUG_INIT __debugInit()

	#define DEBUG_LOG(str) __debugLog(str)
	#define DEBUG_LOG_F(str, ...) __debugLogF(str, __VA_ARGS__)

	#define ERROR(message) __error(message)
	#define ERROR_F(message, ...) __errorF(message, __VA_ARGS__)

	#define ASSERT(cond, message) __assert(cond, message)
	#define ASSERT_F(cond, message, ...) __assertF(cond, message, __VA_ARGS__)

#endif

#endif