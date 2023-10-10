#include "Application.h"

void Application::Setup()
{
	std::vector<Vec2> modelcircle;
	modelcircle.push_back({ 0.0f,0.0f });
	int nPoints = 20;
	for (int i = 0; i < nPoints; i++)
	{
		modelcircle.push_back({ cosf(i / (float)(nPoints - 1) * 2.0f * 3.14159f), sinf(i / (float)(nPoints - 1) * 2.0f * 3.14159f) });

	}

	Body* body = new Body(CircleShape(50,modelcircle), 400, 300, 1.0f);
	
	Bodies.push_back(body);
}

void Application::Input(olc::PixelGameEngine* ptr)
{
	if (ptr->GetKey(olc::UP).bHeld) 
		pushForce.y = -100 * PIXELS_PER_METER;
    if (ptr->GetKey(olc::DOWN).bHeld) 
		pushForce.y = +100 * PIXELS_PER_METER;
    if (ptr->GetKey(olc::LEFT).bHeld) pushForce.x = -100 * PIXELS_PER_METER;
	if (ptr->GetKey(olc::RIGHT).bHeld) pushForce.x = +100 * PIXELS_PER_METER;

	if (ptr->GetKey(olc::UP).bReleased) pushForce.y = 0;
	if (ptr->GetKey(olc::DOWN).bReleased) pushForce.y = 0;
	if (ptr->GetKey(olc::LEFT).bReleased) pushForce.x = 0;
	if (ptr->GetKey(olc::RIGHT).bReleased) pushForce.x = 0;

	//if (ptr->GetMouse(1).bPressed)
	//{
	//	body* body = new body(ptr->GetMouseX(), ptr->GetMouseY(), 1.0f);
	//	body->radius = 5;
	//	bodys.push_back(body);
	//}
	//
	//if (ptr->GetMouse(0).bHeld)
	//{
	//	int lastbody = NUM_bodyS - 1;
	//	mousepressedlocation = Vec2(ptr->GetMouseX(), ptr->GetMouseY());
	//	ptr->DrawLine(Bodies[lastbody]->position.x, Bodies[lastbody]->position.y, mousepressedlocation.x, mousepressedlocation.y);
	//}
	//
	//if (ptr->GetMouse(0).bReleased)
	//{
	//	int lastbody = NUM_bodyS - 1;
	//	Vec2 impulseDirection = (Bodies[lastbody]->position - mousepressedlocation).UnitVector();
	//	float impulseMagnitude = (Bodies[lastbody]->position - mousepressedlocation).Magnitude() * 5.0f;
	//	Bodies[0]->velocity = impulseDirection * impulseMagnitude;
	//}
}

void Application::Update(float deltatime,olc::PixelGameEngine* ptr)
{
	if (deltatime > 0.017)
	{
		deltatime = 0.017f;
	}
	
	Bodies[0]->AddForce(pushForce);

	for (auto body : Bodies)
	{

		Vec2 drag = Force::GenerateDragForce(*body, 0.002f);
		body->AddForce(drag);

		Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
		body->AddForce(weight);

	}

	
	


	for (auto body : Bodies)
	{
		body->integrate(deltatime);
	}

	for (auto body : Bodies)
	{
		if (body->shape->GetType() == CIRCLE)
		{
			CircleShape* circleshape = (CircleShape*)body->shape;
			if (body->position.x - circleshape->radius <= 0)
			{
				body->position.x = circleshape->radius;
				body->velocity.x *= -0.9f;
			}
			else if (body->position.x + circleshape->radius >= ptr->ScreenWidth())
			{
				body->position.x = ptr->ScreenWidth() - circleshape->radius;
				body->velocity.x *= -0.9f;
			}

			if (body->position.y - circleshape->radius <= 0)
			{
				body->position.y = circleshape->radius;
				body->velocity.y *= -0.9f;
			}
			else if (body->position.y + circleshape->radius >= ptr->ScreenHeight())
			{
				body->position.y = ptr->ScreenHeight() - circleshape->radius;
				body->velocity.y *= -.9f;

			}
		}
	}
}

void Application::Render(olc::PixelGameEngine* ptr)
{
	for (auto& body : Bodies)
	{
		if (body->shape->GetType() == CIRCLE)
		{
			CircleShape* circleshape = (CircleShape*)body->shape;
			//ptr->DrawCircle(body->position.x, body->position.y, circleshape->radius, olc::CYAN);
			DraweWireFrameModel(ptr, circleshape->vertices, body->position.x, body->position.y, circleshape->radius);
		}
		else
		{

		}
	}
}

void Application::Destroy()
{
	for (auto body : Bodies)
	{
		delete body;
	}
}

void Application::DraweWireFrameModel(olc::PixelGameEngine* pge, const std::vector<Vec2> vecmodelcoordinates, float x, float y, float r, float s, olc::Pixel p)
{
	std::vector<Vec2> vecTransformedCoordinates;
	int verts = vecmodelcoordinates.size();
	vecTransformedCoordinates.resize(verts);

	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].x = vecmodelcoordinates[i].x * cosf(r) - vecmodelcoordinates[i].y * sinf(r);
		vecTransformedCoordinates[i].y = vecmodelcoordinates[i].x * sinf(r) + vecmodelcoordinates[i].y * cosf(r);
	}

	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].x = vecTransformedCoordinates[i].x * s;
		vecTransformedCoordinates[i].y = vecTransformedCoordinates[i].y * s;
 	}

	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].x = vecTransformedCoordinates[i].x + x;
		vecTransformedCoordinates[i].y = vecTransformedCoordinates[i].y + y;
	}

	for (int i = 0; i < verts + 1; i++)
	{
		int j = (i + 1);
		pge->DrawLine(vecTransformedCoordinates[i % verts].x, vecTransformedCoordinates[i % verts].y,
			vecTransformedCoordinates[j % verts].x, vecTransformedCoordinates[j % verts].y, p);
	}

}
