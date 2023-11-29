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

void ManipulatedSprite::Render(olc::PixelGameEngine* pge, Body* body)
{
	BoxShape* boxShape = (BoxShape*)body->shape;
	std::array<Vec2f, 4> points;
	for (int i = 0;i < boxShape->worldvertices.size(); i++)
	{
		points[i] = boxShape->worldvertices[i];
	}
	//Vec2f centerpt = GetQuadCenterpoint(points);
	//
	//DrawRotatedWarpedSprite(pge,points,body->rotation,centerpt);
	DrawWarpedSprite(pge, points);
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
				for (int i = 0; i < boxShape->worldvertices.size();i++)
				{
					if ((boxShape->worldvertices[i] - mouse).Magnitude() < 5)
					{
						pSelected = &boxShape->worldvertices[i];
						index = i;
					}


				}
			}

			if (pge->GetMouse(1).bReleased)
			{
				pSelected = nullptr;
			}

			if (pSelected != nullptr)
			{
				*pSelected = mouse;
				pge->DrawString(30, 30, "index: " + std::to_string(index));
			}
		}
	}
}

void ManipulatedSprite::GetQuadBoundingBoxD(std::array<Vec2d, 4>& vertices, Vec2i& UpLeft, Vec2i& LwRight)
{
	UpLeft = { INT_MAX, INT_MAX };
	LwRight = { INT_MIN,INT_MIN };
	
	for (int i = 0; i < vertices.size(); i++)
	{
		UpLeft = UpLeft.min(vertices[i]);
		LwRight = LwRight.max(vertices[i]);
	}

}

void ManipulatedSprite::GetQuadBoundingBoxF(std::array<Vec2f, 4>& vertices, Vec2i& UpLeft, Vec2i& LwRight)
{
	UpLeft = { INT_MAX, INT_MAX };
	LwRight = { INT_MIN,INT_MIN };

	for (int i = 0; i < vertices.size(); i++)
	{
		UpLeft = UpLeft.min(vertices[i]);
		LwRight = LwRight.max(vertices[i]);
	}
}

void ManipulatedSprite::DrawWarpedSprite(olc::PixelGameEngine* pge, std::array<Vec2f, 4> points)
{
	auto Get_q = [=](const  std::array<Vec2d, 4>& cPts, const Vec2d& curVert) -> Vec2d { return curVert - cPts[0];                     };
	auto Get_b1 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[1] - cPts[0];                     };
	auto Get_b2 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[2] - cPts[0];                     };
	auto Get_b3 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d 
		{ 
			return cPts[0] - cPts[1] - cPts[2] + cPts[3]; 
		};

	// note that the corner points are passed in order: ul, ll, lr, ur, but the WarpedSample() algorithm
	// assumes the order ll, lr, ul, ur. This rearrangement is done here
	std::array<Vec2d, 4> localCornerPoints;

	localCornerPoints[0] = points[1];
	localCornerPoints[1] = points[2];
	localCornerPoints[2] = points[0];
	localCornerPoints[3] = points[3];
	
	// get b1-b3 values from the quad corner points
	// NOTE: the q value is associated per pixel and is obtained in the nested loop below
	Vec2d b1 = Get_b1(localCornerPoints);
	Vec2d b2 = Get_b2(localCornerPoints);
	Vec2d b3 = Get_b3(localCornerPoints);

	// determine the bounding box around the quad
	Vec2i UpperLeft, LowerRight;
	GetQuadBoundingBoxD(localCornerPoints, UpperLeft, LowerRight);

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

Vec2i ManipulatedSprite::GetQuadCenterpoint(std::array<Vec2f, 4> points)
{
	Vec2i UpperLeft, LowerRight;
	GetQuadBoundingBoxF(points, UpperLeft, LowerRight);
	// then return the points where the diagonals intersect
	return UpperLeft + (LowerRight - UpperLeft) / 2.0;
}

void ManipulatedSprite::DrawRotatedWarpedSprite(olc::PixelGameEngine* pge, const std::array<Vec2f, 4>& cornerPoints, float fAngle, Vec2f centerPoint)
{
	std::array<Vec2d, 4> rotatedPoints;
	for (int i = 0; i < 4; i++) {
		rotatedPoints[i] = cornerPoints[i];
	}
	// convert float parameters to doubles
	double dAngle = double(fAngle);
	Vec2d dCenterPoint(centerPoint);

	// rotate them around center point
	RotateQuadPoints(rotatedPoints, dAngle, dCenterPoint);
	// convert back to correct type
	std::array<Vec2f, 4> localPoints;
	for (int i = 0; i < 4; i++) {
		localPoints[i] = rotatedPoints[i];
	}
	// draw sprite using rotated cornerpoints
	DrawWarpedSprite(pge, localPoints);

}

void ManipulatedSprite::RotateQuadPoints(std::array<Vec2d, 4> points, float rotation, Vec2f centerpt)
{
	double dCosTheta = cos(rotation);
	double dSinTheta = sin(rotation);
	// iterate quad points
	for (int i = 0; i < 4; i++) {
		// first translate point so that center point aligns with origin
		points[i] -= centerpt;
		// rotate point - because x component is altered and used for calculating y component
		// a cache is applied
		Vec2d cachePoint = points[i];
		points[i].x = cachePoint.x * dCosTheta - cachePoint.y * dSinTheta;
		points[i].y = cachePoint.x * dSinTheta + cachePoint.y * dCosTheta;
		// translate back
		points[i] += centerpt;
	}
}

