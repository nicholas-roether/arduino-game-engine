#include <Arduino.h>
#include <math.h>
#include "AGE_physics.h"

namespace AGE {
	// Velocity
	Velocity::Velocity(float velocity)
		: velocity(velocity) {}

	void Velocity::update(uint8_t dt, float& pos) const {
		pos += dt * velocity / 1000;
	}

	void Velocity::update(uint8_t dt, uint8_t& pos) const {
		pos += dt * velocity / 1000;
	}

	// Position
	bool Position::operator==(const Position& other) {
		return x == other.x && y == other.y;
	}

	Position Position::operator+(const Position& other) const {
		return { x + other.x, y + other.y };
	}

	Position Position::operator-(const Position& other) const {
		return *this + other * (-1);
	}

	Position Position::operator*(float num) const {
		return { x * num, y * num };
	}

	// Collider
	Collider::Collider(CollisionSystem& collisionSystem, uint8_t type)
		: type(type), colSys(collisionSystem)
	{
		colSys.add(this);
	}

	Collider::Collider(const Collider& other)
		: type(other.type), colSys(other.colSys)
	{
		colSys.add(this);
	}

	Collider::~Collider() {
		colSys.remove(this);
	}

	bool Collider::collides(uint8_t type) {
		return colSys.collides(this, type);
	}

	uint8_t Collider::getType() {
		return type;
	}

	// CollisionSystem
	uint8_t CollisionSystem::numTypes = 0;

	void CollisionSystem::add(Collider* collider) {
		colliders.push(collider);
	}

	void CollisionSystem::remove(Collider* collider) {
		colliders.removeElement(collider);
	}

	bool CollisionSystem::collides(Collider* collider, uint8_t type) {
		for (Collider* other : colliders) {
			if (other->getType() != type) continue;
			Position diff = collider->getPos() - other->getPos();
			if (abs(diff.x) < 1 && abs(diff.y) < 1) return true;
		}
		return false;
	}
}