#ifndef APPLICATION_H
#define APPLICAITON_H
#include "olcPixelGameEngine.h"
#include "Particle.h"
#include "Constants.h"
#include <chrono>

class Application
{
public:
	Application() = default;
	~Application() = default;

public:
	Particle* particle;
	
public:

	void Setup();
	void Input(olc::PixelGameEngine* ptr);
	void Update(float deltatime, olc::PixelGameEngine* ptr);
	void Render(olc::PixelGameEngine* ptr);
	void Destroy();

};



#endif // !APPLICATION_H


