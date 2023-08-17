#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass)
{
	this->position = Vec2(x, y);
	this->mass = mass;
	
	std::cout << "particle constructed!" << std::endl;
}

Particle::~Particle()
{
	std::cout << "particle destroyed!" << std::endl;
}

void Particle::integrate(float deltatime)
{
	
	this->velocity += this->acceleration * deltatime;

	this->position += this->velocity * deltatime;
}
