#ifndef APPLICATION_H
#define APPLICAITON_H
#include "olcPixelGameEngine.h"
#include "Body.h"
#include "Constants.h"
#include "Force.h"
#include "Graphics.h"
#include "CollisionDetection.h"
#include "Contact.h"

class Application
{
public:
	Application() = default;
	~Application() = default;

public:
	std::vector<Body*> Bodies;
	Vec2 pushForce = Vec2(0,0);
	
	float k = 500;
	float restLength = 200.0f;
	const int NUM_PARTICLES = 4;
	bool debug = false;
	Vec2 mousepressedlocation;
	Vec2 mousereleasedlocation;
	olc::Sprite* sprite;
	olc::Decal* Dsprite;
	
public:

	void Setup();
	void Input(olc::PixelGameEngine* ptr);
	void Update(float deltatime, olc::PixelGameEngine* ptr);
	void Render(olc::PixelGameEngine* ptr);
	void Destroy();
	void DraweWireFrameModel(olc::PixelGameEngine* pge,
		const std::vector<Vec2> vecmodelcoordinates,
		float x, float y,
		float r = 0.0f, float s = 1.0f,
		olc::Pixel p = olc::WHITE);
};



#endif // !APPLICATION_H


