#include "Contact.h"

void Contact::ResolvePentration()
{
	if (a->IsStatic() && b->IsStatic()) return;

	float da = depth / (a->invMass + b->invMass) * a->invMass;
	float db = depth / (a->invMass + b->invMass) * b->invMass;

	a->position -= normal * da;
	b->position += normal * db;
 }

void Contact::ResolveCollision()
{
	ResolvePentration();

	float e = std::min(a->restitution, b->restitution);
	float f = std::min(a->friction, b->friction);

	//const Vec2 vrel = (a->velocity - b->velocity);
	Vec2 ra = end - a->position;
	Vec2 rb = start - b->position;
	Vec2 va = a->velocity + Vec2(-a->angularvelocity * ra.y, a->angularvelocity * ra.x);
	Vec2 vb = b->velocity + Vec2(-b->angularvelocity * rb.y, a->angularvelocity * rb.x);
	const Vec2 vrel = va - vb;

	float vrelDotNormal = vrel.Dot(normal);
	const Vec2 impulseDirectionN = normal;
    const float impulseMagnitudeN = -(1 + e) * vrelDotNormal / ((a->invMass + b->invMass) + ra.Cross(normal) * ra.Cross(normal) * a->invInertia
		+ rb.Cross(normal) * rb.Cross(normal) * b->invInertia); 
    Vec2 jn = impulseDirectionN * impulseMagnitudeN;


	Vec2 tangent = normal.Normal();
	float vrelDotTangent = vrel.Dot(tangent);
	const Vec2 impulseDirectionT = tangent;
	const float impulseMagnitudeT = f * -(1 + e) * vrelDotTangent / ((a->invMass + b->invMass) + ra.Cross(tangent) * ra.Cross(tangent) * a->invInertia
		+ rb.Cross(tangent) * rb.Cross(tangent) * b->invInertia);
	Vec2 jt = impulseDirectionT * impulseMagnitudeT;

	Vec2 J = jn + jt;

	a->ApplyImpulse(J,ra);
	b->ApplyImpulse(-J,rb);

}
