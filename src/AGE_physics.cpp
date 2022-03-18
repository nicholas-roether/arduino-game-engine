#include <Arduino.h>
#include <math.h>
#include "AGE_physics.h"

namespace AGE {
	Velocity::Velocity(int8_t velocity)
		: velocity(velocity) {}

	void Velocity::update(unsigned int dt, uint8_t& pos) {
		time += dt;
		if (time > 1000 / abs(velocity)) {
			pos += (velocity > 0 ? 1 : -1);
			time = 0;
		}
	}
}