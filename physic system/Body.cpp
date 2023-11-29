#include "Body.h"
#include <iostream>

Body::Body(const Shape& shape,int index,float x, float y, float mass)
{
	this->shape = shape.Clone();
	this->bodyindex = index;
	//linear motion
	this->position = Vec2f(x, y);
	this->velocity = Vec2f(0, 0);
	this->acceleration = Vec2f(0, 0);
	//angular motion
	this->rotation = 0.0f;
	this->angularvelocity = 0.0f;
	this->angularacceleration = 0.0f;
	this->restitution = 1.0f;
	this->friction = 0.7f;
	this->sumForces = Vec2f{ 0,0 };
	this->sumTorque = 0.0f;
	this->mass = mass;
	if (mass != 0.0f)
	{
		this->invMass = 1.0f / mass; 
	}
	else
	{
		this->invMass = 0.0f;
	}

	Inertia = shape.GetMomentOfInertia() * mass;

	if (Inertia != 0.0f)
	{
		this->invInertia = 1.0f / Inertia;
	}
	else
	{
		this->invInertia = 0.0f;
	}

	std::cout << "body constructed!" << std::endl;
}

Body::~Body()
{
	delete shape;
	std::cout << "body destroyed!" << std::endl;
}

void Body::integrateLinear(float deltatime)
{
	if (movementstatic)
	{
		return;
	}

	acceleration = sumForces * invMass;
	
	velocity += acceleration * deltatime;

	position += velocity * deltatime;

	ClearForces();
}

void Body::integrateAngular(float deltatime)
{
	if (rotationstatic)
	{
		return;
	}

	angularacceleration = sumTorque * invInertia;

	angularvelocity += angularacceleration * deltatime;

	rotation += angularvelocity * deltatime;

	ClearTorgue();
}

void Body::AddForce(const Vec2f& force)
{
	sumForces += force;
}

void Body::AddTorque(float torque)
{
	sumTorque += torque;
}

void Body::ClearTorgue()
{
	sumTorque = 0;
}

void Body::ClearForces()
{
	sumForces = Vec2f(0.0f, 0.0f);
}

bool Body::IsStatic() const 
{
	const float epsilon = 0.005f;
	bool result = fabs(invMass - 0.0f) < epsilon;
	 
	return result;
}

void Body::ApplyImpulse(const Vec2f& j)
{
	if (movementstatic) return;

	velocity += j * invMass;
}

void Body::ApplyImpulse(const Vec2f& j, const Vec2f& r)
{
	if (movementstatic || rotationstatic) return;

	velocity += j * invMass;
	angularvelocity += r.Cross(j) * invInertia;
}

void Body::SetTexture(const char* textureFileName)
{
	sprite = new olc::Sprite(textureFileName);
	decal = new olc::Decal(sprite);
}

void Body::Update(float deltatime, int index)
{
	integrateLinear(deltatime);
	integrateAngular(deltatime);
	bool isPolygon = shape->GetType() == POLYGON || shape->GetType() == BOX;

	if (isPolygon)
	{
		PolygonShape* polygonshape = (PolygonShape*)shape;
		
		polygonshape->UpdateVertices(rotation, position, index);
	}
}
