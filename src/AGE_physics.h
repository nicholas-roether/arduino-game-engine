#include "AGE_utils.h"

namespace AGE {
	template<typename T>
	struct Vector {
		T x;
		T y;

		Vector<T> Vector::operator+(const Vector<T>& other) {
			return { x + other.x, y + other.y };
		}

		Vector<T> Vector::operator-(const Vector<T>& other) {
			return { x - other.x, y - other.y };
		}

		Vector<T> Vector::operator*(T factor) {
			return { x * factor, y * factor };
		}

		Vector<T>& Vector::operator+=(const Vector<T>& other) {
			x += other.x;
			y += other.y;
			return *this;
		}

		Vector<T>& Vector::operator-=(const Vector<T>& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vector<T>& Vector::operator*=(T factor) {
			x *= factor;
			y *= factor;
			return *this;
		}
	};

	class PhysicsObject {
		Vector<double> pos;
		Vector<double> velocity;
		Vector<double> acceleration;

	public:
		virtual void update(unsigned int dt);

		Vector<int> getPos() const;

		void setPos(const Vector<int>& newPos);

		void setVelocity(const Vector<double>& newVel);

		void setAcceleration(const Vector<double>& newAcc);
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

		// TODO
		bool operator==(const CollidingPhysicsObject& other);
	};

	class CollisionSystem {
		Utils::Array<CollidingPhysicsObject*> objects;

	public:
		void add(CollidingPhysicsObject* obj);

		Utils::Array<unsigned int> getCollisionList(const CollidingPhysicsObject& obj);
	};
}