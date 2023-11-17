#ifndef SHAPE_H
#define SHAPE_H
#include "Vec2.h"
#include <vector>

enum ShapeType
{
	CIRCLE,
	POLYGON,
	BOX
};

struct Shape
{
	
	virtual  ~Shape() = default;
	virtual ShapeType GetType() const = 0;
	virtual Shape* Clone() const = 0;
	virtual float GetMomentOfInertia() const = 0;

};

struct CircleShape : public Shape
{
	float radius;
	int vertexCount;
	std::vector<Vec2> vertices;
	CircleShape(const float radius,const int VerticesCount);
	virtual ~CircleShape();
	ShapeType GetType() const override; 
	Shape* Clone() const override;
	float GetMomentOfInertia() const override;
};

struct PolygonShape : public Shape
{
	std::vector<Vec2> localvertices;
	std::vector<Vec2> worldvertices;
	PolygonShape() = default;
	PolygonShape(const std::vector<Vec2> vertices);
	virtual ~PolygonShape();
	Vec2 EdgeAt(int index) const;
	float FindMinSeparation(PolygonShape* other, Vec2& axis,Vec2& point);
	ShapeType GetType() const override;
	Shape* Clone() const override;
	float GetMomentOfInertia() const override;
	void UpdateVertices(float angle, const Vec2& position);
};

struct BoxShape : public PolygonShape
{
	float width;
	float height;

	BoxShape(float width, float height);
	virtual ~BoxShape();
	ShapeType GetType() const override;
	Shape* Clone() const override;
	float GetMomentOfInertia() const override;
};
#endif // !SHAPE_H


