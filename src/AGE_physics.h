#include "AGE_utils.h"

namespace AGE {
	class Velocity {
		uint8_t* pos;
		uint8_t velocity;
		unsigned int time;

	public:
		Velocity(uint8_t* pos, uint8_t velocity);

		void update(unsigned int dt);
	};
}