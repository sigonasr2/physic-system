#ifndef body_H
#define body_H
#include "Vec2.h"
#include "Shape.h"

struct Body
{
	
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 sumForces;
	float mass;
	float invMass;

	Shape* shape = nullptr;
	Body() = default;
	Body(const Shape& shape,float x, float y, float mass);
	~Body();
	void integrate(float deltatime);
	void AddForce(const Vec2& force);
	void ClearForces();
};
#endif // !body_H


