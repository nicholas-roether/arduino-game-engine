#include <Arduino.h>
#include <math.h>
#include "AGE_physics.h"

namespace AGE {
	Velocity::Velocity(uint8_t* pos, int8_t velocity)
		: pos(pos), velocity(velocity) {}

	void Velocity::update(unsigned int dt) {
		time += dt;
		if (time > 1000 / abs(velocity)) {
			(*pos) += (velocity > 0 ? 1 : -1);
			time = 0;
		}
	}
}