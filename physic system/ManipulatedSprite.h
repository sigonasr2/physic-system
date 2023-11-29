#ifndef MANIPULATEDSPRITE_H
#define MANIPULATEDSPRITE_H
#include "olcPixelGameEngine.h"
#include "Body.h"
#define NEAR_ZERO   0.000001 
class ManipulatedSprite
{
public:
	ManipulatedSprite() = default;

	olc::Sprite* sprOrg = nullptr;
	olc::Sprite* activeSpr = nullptr;

	std::vector<Body*> bodies;

	float ftheta = 0.0f;

	int nScaleSprite = 2;
	float fDispPercentage = 0.5f;
	Vec2f scaleFactor = { 1.0f, 1.0f };
	Vec2f partSource = { 0.0f, 0.0f };
	Vec2f partSize = { 1.0f, 1.0f };
	Vec2f* pSelected = nullptr;
	int index = 0;
public:

	void ScaleSprite(olc::Sprite* orgSprite, olc::Sprite** activeSpr, int nScale);
	void Setup(Body* body);
	void Render(olc::PixelGameEngine* pge,Body* body);
	void mousecontrol(olc::PixelGameEngine* pge, Body* body, int& index);
	void GetQuadBoundingBoxD(std::array<Vec2d, 4>& vertices,Vec2i& UpLeft, Vec2i& LwRight);
	void GetQuadBoundingBoxF(std::array<Vec2f, 4>& vertices, Vec2i& UpLeft, Vec2i& LwRight);
	void DrawWarpedSprite(olc::PixelGameEngine* pge, std::array<Vec2f, 4> points);
	bool WarpedSample(Vec2d q, Vec2d b1, Vec2d b2, Vec2d b3, olc::Sprite* psprite, olc::Pixel& color);
	Vec2i GetQuadCenterpoint(std::array<Vec2f, 4> points);
	void DrawRotatedWarpedSprite(olc::PixelGameEngine* pge, const std::array<Vec2f, 4>& cornerPoints, float fAngle, Vec2f centerPoint);
	void RotateQuadPoints(std::array<Vec2d, 4> points, float rotation, Vec2f centerpt);
};

#endif // !MANIPULATEDSPRITE_H



