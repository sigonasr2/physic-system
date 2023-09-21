#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vec2.h"


struct Particle
{
	int radius;
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 sumForces;
	float mass;
	float invMass;
	Particle(float x, float y, float mass);
	~Particle();
	void integrate(float deltatime);
	void AddForce(const Vec2& force);
	void ClearForces();
};
#endif // !PARTICLE_H



