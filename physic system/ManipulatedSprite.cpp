#include "ManipulatedSprite.h"

void ManipulatedSprite::ScaleSprite(olc::Sprite* orgSprite, olc::Sprite** activeSpr, int nScale)
{
	if (*activeSpr != nullptr)
		delete* activeSpr;

	*activeSpr = new olc::Sprite(orgSprite->width * nScale, orgSprite->height * nScale);

    for (int y = 0; y < orgSprite->height; y++) {
        for (int x = 0; x < orgSprite->width; x++) {
            olc::Pixel p = orgSprite->GetPixel(x, y);
            for (int i = 0; i < nScale; i++) {
                for (int j = 0; j < nScale; j++) {
                    (*activeSpr)->SetPixel(x * nScale + j, y * nScale + i, p);
                }
            }
        }
    }
}

void ManipulatedSprite::Setup(Body* body)
{
    bodies.push_back(body);

	sprOrg = new olc::Sprite("crate.png");
	nScaleSprite = 1;
	scaleFactor = { 1.0f,1.0f };

	ScaleSprite(sprOrg, &activeSpr, nScaleSprite);

}

void ManipulatedSprite::Render(olc::PixelGameEngine* pge, BoxShape* box)
{
	DrawWarpedSprite(pge, box);
}

void ManipulatedSprite::mousecontrol(olc::PixelGameEngine* pge)
{
	for (auto body : bodies)
	{
		if (body->shape->GetType() == BOX)
		{

			BoxShape* boxShape = (BoxShape*)body->shape;





			Vec2d mouse = { double(pge->GetMouseX()), double(pge->GetMouseY()) };

			if (pge->GetMouse(1).bPressed)
			{
				pSelected = nullptr;
				for (auto& p : boxShape->worldvertices)
				{
					if ((p - mouse).Magnitude() < 5)
						pSelected = &p;
				}
			}

			if (pge->GetMouse(1).bReleased)
			{
				pSelected = nullptr;
			}

			if (pSelected != nullptr)
			{
				*pSelected = mouse;
			}
		}
	}
}

void ManipulatedSprite::GetQuadBoundingBox(std::array<Vec2d, 4>& vertices, Vec2i& UpLeft, Vec2i& LwRight)
{
	UpLeft = { INT_MAX, INT_MAX };
	LwRight = { INT_MIN,INT_MIN };
	
	for (int i = 0; i < vertices.size(); i++)
	{
		UpLeft = UpLeft.min(vertices[i]);
		LwRight = LwRight.max(vertices[i]);
	}

}

void ManipulatedSprite::DrawWarpedSprite(olc::PixelGameEngine* pge, BoxShape* box)
{
	auto Get_q = [=](const  std::array<Vec2d, 4>& cPts, const Vec2d& curVert) -> Vec2d { return curVert - cPts[0];                     };
	auto Get_b1 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[1] - cPts[0];                     };
	auto Get_b2 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[2] - cPts[0];                     };
	auto Get_b3 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[0] - cPts[1] - cPts[2] + cPts[3]; };

	// note that the corner points are passed in order: ul, ll, lr, ur, but the WarpedSample() algorithm
	// assumes the order ll, lr, ul, ur. This rearrangement is done here
	std::array<Vec2d, 4> localCornerPoints;

	for (int i = 0; i < box->worldvertices.size(); i++)
	{
		localCornerPoints[i] = box->worldvertices[i];
	}
	
	// get b1-b3 values from the quad corner points
	// NOTE: the q value is associated per pixel and is obtained in the nested loop below
	Vec2d b1 = Get_b1(localCornerPoints);
	Vec2d b2 = Get_b2(localCornerPoints);
	Vec2d b3 = Get_b3(localCornerPoints);

	// determine the bounding box around the quad
	Vec2i UpperLeft, LowerRight;
	GetQuadBoundingBox(localCornerPoints, UpperLeft, LowerRight);

	// iterate all pixels within the bounding box of the quad...
	for (int y = UpperLeft.y; y <= LowerRight.y; y++) {
		for (int x = UpperLeft.x; x <= LowerRight.x; x++) {
			// ... and render them if sampling produces valid pixel
			olc::Pixel pix2render;
			Vec2d q = Get_q(localCornerPoints, { (double)x, (double)y });

			if (WarpedSample(q, b1, b2, b3, activeSpr, pix2render)) {
				pge->Draw(x, y, pix2render);
			}
		}
	}
}

bool ManipulatedSprite::WarpedSample(Vec2d q, Vec2d b1, Vec2d b2, Vec2d b3, olc::Sprite* psprite, olc::Pixel& color)
{
	auto wedge_2d = [=](Vec2d v, Vec2d w) {
		return v.x * w.y - v.y * w.x;
		};

	// Set up quadratic formula
	double A = wedge_2d(b2, b3);
	double B = wedge_2d(b3, q) - wedge_2d(b1, b2);
	double C = wedge_2d(b1, q);

	// Solve for v
	Vec2d uv = { 0.0, 0.0 };
	if (fabs(A) < NEAR_ZERO) {
		// Linear form
		if (fabs(B) < NEAR_ZERO) {
			return false;
		}
		uv.y = -C / B;
	}
	else {
		// Quadratic form: Take positive root for CCW winding with V-up
		double D = B * B - 4 * A * C;
		if (D <= 0.0) {         // if discriminant <= 0, then the point is not inside the quad
			return false;
		}
		uv.y = 0.5 * (-B + sqrt(D)) / A;
	}
	// Solve for u, using largest magnitude component
	Vec2d denom = b1 + b3 * uv.y;
	if (fabs(denom.x) > fabs(denom.y)) {
		if (fabs(denom.x) < NEAR_ZERO) {
			return false;
		}
		uv.x = (q.x - b2.x * uv.y) / denom.x;
	}
	else {
		if (fabs(denom.y) < NEAR_ZERO) {
			return false;
		}
		uv.x = (q.y - b2.y * uv.y) / denom.y;
	}
	// Note that vertical texel coord is mirrored because the algorithm assumes positive y to go up
	color = psprite->Sample(uv.x, 1.0 - uv.y);

	// return whether sampling produced a valid pixel
	return (uv.x >= 0.0 && uv.x <= 1.0 &&
		uv.y >= 0.0 && uv.y <= 1.0);
}

