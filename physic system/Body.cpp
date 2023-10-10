#include "Body.h"
#include <iostream>

Body::Body(const Shape& shape,float x, float y, float mass)
{
	this->shape = shape.Clone();
	this->position = Vec2(x, y);
	this->mass = mass;
	if (mass != 0.0f)
	{
		this->invMass = 1.0f / mass; 
	}
	else
	{
		this->invMass = 0.0f;
	}

	std::cout << "body constructed!" << std::endl;
}

Body::~Body()
{
	delete shape;
	std::cout << "body destroyed!" << std::endl;
}

void Body::integrate(float deltatime)
{
	acceleration = sumForces * invMass;
	
	velocity += acceleration * deltatime;

	position += velocity * deltatime;

	ClearForces();
}

void Body::AddForce(const Vec2& force)
{
	sumForces += force;
}

void Body::ClearForces()
{
	sumForces = Vec2(0.0f, 0.0f);
}
