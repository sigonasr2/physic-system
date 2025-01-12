#ifndef body_H
#define body_H
#include "Vec2.h"
#include "Shape.h"

struct Body
{
	//linear stuff
	bool isColliding = false;
	Vec2f position;
	Vec2f velocity;
	Vec2f acceleration;
	Vec2f sumForces;
	float mass;
	float invMass;
	bool IsSet;
	Vec2f mousecoord = { 0.0f,0.0f };
	//angular
	float rotation;
	Vec2f offset{ 0,0 };
	int bodyindex;
	float angularvelocity;
	float angularacceleration;
	float sumTorque;
	float Inertia;
	float invInertia;
	bool rotationstatic = false;
	bool movementstatic = false;
	float friction;
	float restitution; 
	olc::Sprite* sprite = nullptr;
	olc::Decal* decal = nullptr;
	Shape* shape = nullptr;
	Body() = default;
	Body(const Shape& shape,int index,float x, float y, float mass);
	~Body();
	void integrateLinear(float deltatime);
	void integrateAngular(float deltatime);
	void AddForce(const Vec2f& force);
	void AddTorque(float torque);
	void ClearTorgue();
	void ClearForces();
	bool IsStatic() const;
	void ApplyImpulse(const Vec2f& j);
	void ApplyImpulse(const Vec2f& j, const Vec2f& r);
	void SetTexture(const char* textureFileName);
	void Update(float deltatime,int index);
};
#endif // !body_H



