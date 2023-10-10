#include "Force.h"

float Force::clamp(float val, float min, float max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

Vec2 Force::GenerateDragForce(const Body& particle, float k)
{
    Vec2 dragForce = Vec2(0, 0);

    if (particle.velocity.MagnitudeSquared() > 0)
    {
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0f;

        float dragMagnitude = k * particle.velocity.MagnitudeSquared();

        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Body& particle, float k)
{
    Vec2 frictionForce = Vec2(0, 0);

    Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0f;

    float frictionMagnitude = k;

    frictionForce = frictionDirection * frictionMagnitude;


    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance)
{
    Vec2 d = (b.position - a.position);

    float distanceSquared = d.MagnitudeSquared();

    distanceSquared = clamp(distanceSquared, minDistance, maxDistance);

    Vec2 attractionDirection = d.UnitVector();
    float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;

    Vec2 attractionForce = attractionDirection * attractionMagnitude;

    return attractionForce;
}

Vec2 Force::GenerateSpringForce(const Body& particle, Vec2 anchor, float restLength, float k)
{
    Vec2 d = particle.position - anchor;

    float displacement = d.Magnitude() - restLength;

    Vec2 springDirection = d.UnitVector();
    float spriteMagnitude = -k * displacement;

    Vec2 springForce = springDirection * spriteMagnitude;

    return springForce;
}

Vec2 Force::GenerateSpringForce(const Body& a, const Body& b, float restLength, float k)
{
    Vec2 d = a.position - b.position;

    float displacement = d.Magnitude() - restLength;

    Vec2 springDirection = d.UnitVector();
    float spriteMagnitude = -k * displacement;

    Vec2 springForce = springDirection * spriteMagnitude;

    return springForce;
}
