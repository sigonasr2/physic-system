#include "Application.h"

void Application::Setup()
{
	particle = new Particle(50, 100, 1.0f);
	particle->radius = 4;
	
}

void Application::Input(olc::PixelGameEngine* ptr)
{
}

void Application::Update(float deltatime,olc::PixelGameEngine* ptr)
{
	if (deltatime > 0.017)
	{
		deltatime = 0.017f;
	}

	

	particle->acceleration = Vec2(2.0f, 9.8f ) * PIXELS_PER_METER;
	particle->integrate(deltatime);

	if (particle->position.x - particle->radius <= 0)
	{
		particle->position.x = particle->radius;
		particle->velocity.x *= -0.9f;
	}
	else if (particle->position.x + particle->radius >= ptr->ScreenWidth())
	{
		particle->position.x = ptr->ScreenWidth() - particle->radius;
		particle->velocity.x *= -0.9f;
	}

	if (particle->position.y - particle->radius <= 0)
	{
		particle->position.y = particle->radius;
		particle->velocity.y *= -0.9f;
	}
	else if (particle->position.y + particle->radius >= ptr->ScreenHeight())
	{
		particle->position.y = ptr->ScreenHeight() - particle->radius;
		particle->velocity.y *= -.9f;

	}

	
}

void Application::Render(olc::PixelGameEngine* ptr)
{
	ptr->FillCircle(particle->position.x, particle->position.y, particle->radius);
}

void Application::Destroy()
{
	delete particle;
}
