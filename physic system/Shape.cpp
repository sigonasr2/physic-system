#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(const float radius, const std::vector<Vec2> vertices)
{
	this->radius = radius;

	for (auto vertex : vertices)
	{
		this->vertices.push_back(vertex);
	}
	std::cout << "circleshape constructor called" << std::endl;

}

CircleShape::~CircleShape()
{
	std::cout << "circleshape destructor called" << std::endl;
}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

Shape* CircleShape::Clone() const
{
	return new CircleShape(radius, vertices);
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices)
{
	//this->vertices = vertices;
	std::cout << "polygoneshape constructor called" << std::endl;
}

PolygonShape::~PolygonShape()
{
	std::cout << "polygoneshape destructor called" << std::endl;
}

ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}

Shape* PolygonShape::Clone() const
{
	return new PolygonShape(vertices);
}

BoxShape::BoxShape(float width, float height)
{
	this->width = width;
	this->height = height;

	//vertices.push_back(Vec2())
}

BoxShape::~BoxShape()
{
}

ShapeType BoxShape::GetType() const
{
	return BOX;
}

Shape* BoxShape::Clone() const
{
	return new BoxShape(width,height);
}
