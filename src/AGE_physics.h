namespace AGE {
	struct Vector {
		float x;
		float y;

		Vector operator+(const Vector& other);

		Vector operator-(const Vector& other);

		Vector operator*(float factor);

		Vector operator+=(const Vector& other);

		Vector operator-=(const Vector& other);

		Vector operator*=(float factor);
	};

	class PhysicsObject {
	public:
		Vector pos;
		Vector velocity;
		Vector acceleration;
		unsigned int width;
		unsigned int height;

		PhysicsObject(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		void update(unsigned int dt);

		void setWidth(unsigned int width);

		void setHeight(unsigned int height);

		bool collidesWith(unsigned int x, unsigned int y);
	};
}