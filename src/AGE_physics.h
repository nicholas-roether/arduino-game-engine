#include "AGE_utils.h"

namespace AGE {
	class Velocity {
		int8_t velocity;
		unsigned int time;

	public:
		Velocity(int8_t velocity);

		void update(unsigned int dt, uint8_t& pos);
	};
}