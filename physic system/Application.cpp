#include "Application.h"

void Application::Setup()
{
	Body* floor = new Body(BoxShape(800 - 50, 50),0 , 400, 500, 0.0f);
	floor->restitution = 0.5f;
	floor->movementstatic = true;
	floor->rotationstatic = true;
	//floor->SetTexture("crate.png");
	Bodies.push_back(floor);

	Body* leftWall = new Body(BoxShape(50, 400), 1, 50, 270, 0.0f);
	leftWall->movementstatic = true;
	leftWall->rotationstatic = true;
	leftWall->restitution = 0.2f;
    Bodies.push_back(leftWall);

	Body* rightWall = new Body(BoxShape(50, 400), 2, 720, 270, 0.0f);
	rightWall->restitution = 0.2f;
	rightWall->movementstatic = true;
	rightWall->rotationstatic = true;
	Bodies.push_back(rightWall);


	Body* bigBox = new Body(BoxShape(100, 100),3, 400, 300, 0.0f);
	bigBox->rotation = 0.0f;
	//bigBox->restitution = 0.7f;
	bigBox->movementstatic = true;
	bigBox->SetTexture("crate.png");
	Bodies.push_back(bigBox);
	
	for (int i = 0; i < Bodies.size(); i++)
	{
		MS.Setup(Bodies[i]);
	}
	//Body* ball = new Body(CircleShape(50, 20), 400, 300, 1.0f);
	//ball->restitution = 0.1f;
	//Bodies.push_back(ball);
}

void Application::Input(olc::PixelGameEngine* ptr)
{
	if (ptr->GetKey(olc::UP).bPressed) 
		pushForce.y = -100 * PIXELS_PER_METER;
    if (ptr->GetKey(olc::DOWN).bPressed) 
		pushForce.y = +100 * PIXELS_PER_METER;
	if (ptr->GetKey(olc::LEFT).bHeld)  
		Bodies[3]->angularvelocity = -1.0;
	if (ptr->GetKey(olc::RIGHT).bHeld) 
		Bodies[3]->angularvelocity = 1.0;

	if (ptr->GetKey(olc::UP).bReleased) 
		pushForce.y = 0;
	if (ptr->GetKey(olc::DOWN).bReleased) pushForce.y = 0;
	if (ptr->GetKey(olc::LEFT).bReleased)  Bodies[3]->angularvelocity = 0;//pushForce.x = 0;
	if (ptr->GetKey(olc::RIGHT).bReleased) Bodies[3]->angularvelocity = 0;//pushForce.x = 0;
	if (ptr->GetKey(olc::D).bPressed) debug = !debug;
	if (ptr->GetMouse(0).bPressed)
	{
		int x = ptr->GetMouseX();
		int y = ptr->GetMouseY();
		std::vector<Vec2f> vertices =
		{
			Vec2f(20,60),
			Vec2f(-40,20),
			Vec2f(-20,-60),
			Vec2f(20, -60),
			Vec2f(40,20)
		};
		Body* poly = new Body(PolygonShape(vertices),1, x, y, 2.0);
		//poly->restitution = 0.1f;
		poly->friction = 0.7f;
		
		Bodies.push_back(poly);
	}
	
}

void Application::Update(float deltatime,olc::PixelGameEngine* ptr) 
{
	if (deltatime > 0.017)
	{
		deltatime = 0.017f;
	}
	
	//Bodies[1]->position = Vec2(ptr->GetMouseX(), ptr->GetMouseY());
	//Bodies[1]->AddForce(pushForce);

	for (int i = 0; i < Bodies.size(); i++)
	{
		Body* body = Bodies[i];
		if (i != 3)
		{
			Vec2f drag = Force::GenerateDragForce(*body, 0.002f);
			//body->AddForce(drag);

			Vec2f weight = Vec2f(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
			body->AddForce(weight);

			float torque = 200;
			//body->AddTorque(torque);

			Vec2f wind = Vec2f(2.0f * PIXELS_PER_METER, 0.0f);
			//body->AddForce(wind);
		}
	}

	
	
	

	for (auto body : Bodies)
	{
		MS.mousecontrol(ptr, body, index);
		
		body->Update(deltatime, index);
		
		
	}
	

	for (int i = 0; i <= Bodies.size() - 1; i++)
	{
		for (int j = i + 1; j < Bodies.size(); j++)
		{
			Body* a = Bodies[i];
			Body* b = Bodies[j];
			Contact contact;
			a->isColliding = false;
			b->isColliding = false;
			if (CollisionDetection::IsColliding(a,b, contact))
			{
				contact.ResolveCollision();
				if (debug)
				{
					ptr->DrawCircle(contact.start.x, contact.start.y, 3, olc::DARK_MAGENTA);
					ptr->DrawCircle(contact.end.x, contact.end.y, 3, olc::DARK_MAGENTA);
					ptr->DrawLine(contact.start.x, contact.start.y, contact.end.x, contact.end.y, olc::DARK_MAGENTA);
					a->isColliding = true;
					b->isColliding = true;
				}

				
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
			DraweWireFrameModel(ptr, circleshape->vertices, body->position.x, body->position.y,body->rotation, circleshape->radius, 0xff00ff00);
			//ptr->FillCircle(body->position.x, body->position.y, circleshape->radius, color);
		}
		if(body->shape->GetType() == BOX)
		{
			
			BoxShape* boxShape = (BoxShape*)body->shape;
			
			
				
			
			
			

			MS.Render(ptr,body);
				for (int i = 0; i < boxShape->worldvertices.size(); i++)
				{
					ptr->FillCircle({ int(boxShape->worldvertices[i].x),int(boxShape->worldvertices[i].y) }, 4, olc::CYAN);
				}
			
			
			Graphics::DrawPolygon(ptr, body->position.x, body->position.y, boxShape->worldvertices, 0xff00ff00);
			
			
		}
		if (body->shape->GetType() == POLYGON)
		{
			PolygonShape* polygonShape = (PolygonShape*)body->shape;
			Graphics::DrawPolygon(ptr, body->position.x, body->position.y, polygonShape->worldvertices, 0xff00ff00);
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

void Application::DraweWireFrameModel(olc::PixelGameEngine* pge, const std::vector<Vec2f> vecmodelcoordinates, float x, float y, float r, float s, olc::Pixel p)
{
	std::vector<Vec2f> vecTransformedCoordinates;
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
