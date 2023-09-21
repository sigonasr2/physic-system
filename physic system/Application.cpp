#include "Application.h"

void Application::Setup()
{
	//Particle* smallBall = new Particle(50, 100, 1.0f);
	//smallBall->radius = 4;
	//particles.push_back(smallBall);
	//
	//Particle* bigBall = new Particle(200, 100, 3.0f);
	//bigBall->radius = 12;
	//particles.push_back(bigBall);
}

void Application::Input(olc::PixelGameEngine* ptr)
{
	if (ptr->GetKey(olc::UP).bHeld) pushForce.y = -50 * PIXELS_PER_METER;
    if (ptr->GetKey(olc::DOWN).bHeld) pushForce.y = +50 * PIXELS_PER_METER;
    if (ptr->GetKey(olc::LEFT).bHeld) pushForce.x = -50 * PIXELS_PER_METER;
	if (ptr->GetKey(olc::RIGHT).bHeld) pushForce.x = +50 * PIXELS_PER_METER;

	if (ptr->GetKey(olc::UP).bReleased) pushForce.y = 0;
	if (ptr->GetKey(olc::DOWN).bReleased) pushForce.y = 0;
	if (ptr->GetKey(olc::LEFT).bReleased) pushForce.x = 0;
	if (ptr->GetKey(olc::RIGHT).bReleased) pushForce.x = 0;

	if (ptr->GetMouse(1).bPressed)
	{
		Particle* particle = new Particle(ptr->GetMouseX(), ptr->GetMouseY(), 1.0f);
		particle->radius = 5;
		particles.push_back(particle);
	}
	
}

void Application::Update(float deltatime,olc::PixelGameEngine* ptr)
{
	if (deltatime > 0.017)
	{
		deltatime = 0.017f;
	}


	for (auto particle : particles)
	{
		Vec2 wind = Vec2(0.2f * PIXELS_PER_METER, 0.0f);
		Vec2 weight = Vec2(0.0f, particle->mass * 9.8f * PIXELS_PER_METER);
		//particle->AddForce(wind); //wind
		particle->AddForce(weight); //gravity
		particle->AddForce(pushForce);

		//drag force
		if (particle->position.y >= (ptr->ScreenHeight() / 2))
		{
			Vec2 drag = Force::GenerateDragForce(*particle, 0.04);
			particle->AddForce(drag);
		}

	}

	for (auto particle : particles)
	{
		particle->integrate(deltatime);
	}

	for (auto particle : particles)
	{
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
}

void Application::Render(olc::PixelGameEngine* ptr)
{
	ptr->FillRect(0, ptr->ScreenHeight() / 2, ptr->ScreenWidth(), ptr->ScreenHeight() / 2, olc::BLUE);

	for(auto particle : particles )
	ptr->FillCircle(particle->position.x, particle->position.y, particle->radius);


}

void Application::Destroy()
{
	for (auto particle : particles)
	{
		delete particle;
	}
}
