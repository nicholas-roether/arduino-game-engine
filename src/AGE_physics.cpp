#include <Arduino.h>
#include "AGE_physics.h"

namespace AGE {

	// PhysicsObject

	void PhysicsObject::update(unsigned int dt) {
		double dtSeconds = ((double) dt) / 1000;
		velocity += acceleration * dtSeconds;
		pos += velocity * dtSeconds;
	}

	Vector<int> PhysicsObject::getPos() const {
		return { round(pos.x), round(pos.y) };
	}

	void PhysicsObject::setPos(const Vector<int>& newPos) {
		pos = { (double) newPos.x, (double) newPos.y };
	}

	void PhysicsObject::setVelocity(const Vector<double>& newVel) {
		velocity = newVel;
	}

	void PhysicsObject::setAcceleration(const Vector<double>& newAcc) {
		acceleration = newAcc;
	}

	// CollidingPhysicsObject

	static bool checkIntersection(int pos, int start, int width) {
		return start <= pos && pos <= width;
	}

	CollidingPhysicsObject::CollidingPhysicsObject(unsigned int width, unsigned int height)
		: width(width), height(height) {}

	bool CollidingPhysicsObject::collides(Vector<int> pos) const {
		return checkIntersection(pos.x, getPos().x, width) && checkIntersection(pos.y, getPos().y, height);
	}

	bool CollidingPhysicsObject::collides(const CollidingPhysicsObject& other) const {
		bool yIntersection = checkIntersection(other.getPos().y, getPos().y, height)
			|| checkIntersection(other.getPos().y + other.height, getPos().y, height);
		if (!yIntersection) return false;
		return checkIntersection(other.getPos().x, getPos().x, width)
			|| checkIntersection(other.getPos().x + other.width, getPos().x, width);
	}
}