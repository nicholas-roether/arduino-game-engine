#include <Arduino.h>
#include <math.h>
#include "AGE_physics.h"

namespace AGE {
	Velocity::Velocity(uint8_t* pos, uint8_t velocity)
		: pos(pos), velocity(velocity) {}

	void Velocity::update(unsigned int dt) {
		time += dt;
		if (time > 1000 / velocity) {
			(*pos)++;
			time = 0;
		}
	}
}