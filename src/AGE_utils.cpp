#include <Arduino.h>
#include "AGE_utils.h"

namespace AGE::Utils {
	UUID uuid() {
		return random();
	}
}