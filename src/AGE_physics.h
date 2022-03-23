#include "AGE_utils.h"

namespace AGE {
	class Velocity {
		float velocity;

	public:
		Velocity(float velocity);

		void update(uint8_t dt, float& pos) const;
	};

	struct Position {
		float x;
		float y;
		
		bool operator==(const Position& other);
		Position operator+(const Position& other) const;
		Position operator-(const Position& other) const;
		Position operator*(float num) const;
	};

	class CollisionSystem;

	class Collider {
		uint8_t type;
		CollisionSystem& colSys;

	public:
		Collider(CollisionSystem& collisionSystem, uint8_t type);

		Collider(const Collider& other);

		virtual ~Collider();

		bool collides(uint8_t type);

		virtual Position getPos() = 0;

		uint8_t getType();
	};

	class CollisionSystem {
		static uint8_t numTypes;
		Utils::List<Collider*> colliders;

	public:
		void add(Collider* collider);

		void remove(Collider* collider);

		bool collides(Collider* collider, uint8_t type);

		uint8_t createType();
	};
}