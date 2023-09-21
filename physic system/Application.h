#ifndef APPLICATION_H
#define APPLICAITON_H
#include "olcPixelGameEngine.h"
#include "Particle.h"
#include "Constants.h"
#include "Force.h"

class Application
{
public:
	Application() = default;
	~Application() = default;

public:
	std::vector<Particle*> particles;
	Vec2 pushForce = Vec2(0,0);


	
public:

	void Setup();
	void Input(olc::PixelGameEngine* ptr);
	void Update(float deltatime, olc::PixelGameEngine* ptr);
	void Render(olc::PixelGameEngine* ptr);
	void Destroy();

};



#endif // !APPLICATION_H


