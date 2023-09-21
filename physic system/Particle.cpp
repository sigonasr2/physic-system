#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass)
{
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

	std::cout << "particle constructed!" << std::endl;
}

Particle::~Particle()
{
	std::cout << "particle destroyed!" << std::endl;
}

void Particle::integrate(float deltatime)
{
	acceleration = sumForces * invMass;
	
	velocity += acceleration * deltatime;

	position += velocity * deltatime;

	ClearForces();
}

void Particle::AddForce(const Vec2& force)
{
	sumForces += force;
}

void Particle::ClearForces()
{
	sumForces = Vec2(0.0f, 0.0f);
}
