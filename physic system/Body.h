#ifndef body_H
#define body_H
#include "Vec2.h"
#include "Shape.h"

struct Body
{
	//linear stuff
	bool isColliding = false;
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 sumForces;
	float mass;
	float invMass;

	//angular
	float rotation;
	float angularvelocity;
	float angularacceleration;
	float sumTorque;
	float Inertia;
	float invInertia;

	float friction;
	float restitution; 
	olc::Sprite* sprite = nullptr;
	olc::Decal* decal = nullptr;
	Shape* shape = nullptr;
	Body() = default;
	Body(const Shape& shape,float x, float y, float mass);
	~Body();
	void integrateLinear(float deltatime);
	void integrateAngular(float deltatime);
	void AddForce(const Vec2& force);
	void AddTorque(float torque);
	void ClearTorgue();
	void ClearForces();
	bool IsStatic() const;
	void ApplyImpulse(const Vec2& j);
	void ApplyImpulse(const Vec2& j, const Vec2& r);
	void SetTexture(const char* textureFileName);
	void Update(float deltatime);
};
#endif // !body_H



