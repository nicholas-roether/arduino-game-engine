#include "AGE_utils.h"

namespace AGE {
	template<typename T>
	struct Vector {
		T x;
		T y;

		Vector<T> operator+(const Vector<T>& other) {
			return { x + other.x, y + other.y };
		}

		Vector<T> operator-(const Vector<T>& other) {
			return { x - other.x, y - other.y };
		}

		Vector<T> operator*(T factor) {
			return { x * factor, y * factor };
		}

		Vector<T>& operator+=(const Vector<T>& other) {
			x += other.x;
			y += other.y;
			return *this;
		}

		Vector<T>& operator-=(const Vector<T>& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vector<T>& operator*=(T factor) {
			x *= factor;
			y *= factor;
			return *this;
		}
	};

	class PhysicsObject {
		Vector<float> pos;
		Vector<float> velocity;
		Vector<float> acceleration;

	public:
		virtual void update(unsigned int dt);

		Vector<float> getPos() const;

		unsigned int getX() const;

		unsigned int getY() const;

		void setPos(const Vector<int>& newPos);

		void setVelocity(const Vector<float>& newVel);

		void setAcceleration(const Vector<float>& newAcc);
	};

	class CollidingPhysicsObject : public PhysicsObject {
		Utils::UUID id;
		unsigned int objType;
		unsigned int width;
		unsigned int height;

	public:
		CollidingPhysicsObject() = delete;
		CollidingPhysicsObject(unsigned int objType, unsigned int width, unsigned int height);

		bool collides(Vector<int> pos) const;

		bool collides(const CollidingPhysicsObject& other) const;

		unsigned int getObjectType();

		bool operator==(const CollidingPhysicsObject& other);
	};

	class CollisionSystem {
		Utils::List<CollidingPhysicsObject*> objects;

	public:
		void add(CollidingPhysicsObject* obj);

		Utils::List<unsigned int> getCollisionList(const CollidingPhysicsObject& obj) const;
	};
}