#include "Graphics.h"

void Graphics::DrawLine(olc::PixelGameEngine* pge, int x0, int y0, int x1, int y1, uint32_t color)
{
	int delta_x(x1 - x0);
	int delta_y(y1 - y0);

	int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / float(longest_side_length);
	float y_inc = delta_y / float(longest_side_length);

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= longest_side_length; i++)
	{
		pge->Draw(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;

	}
}

void Graphics::DrawCircle(olc::PixelGameEngine* pge, int x, int y, int radius, float angle, uint32_t color)
{

}

void Graphics::DrawFillCircle(olc::PixelGameEngine* pge, int x, int y, int radius, uint32_t color)
{
}

void Graphics::DrawRect(olc::PixelGameEngine* pge, int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int current_x = x + i;
			int current_y = y + j;
			pge->Draw(current_x, current_y, color);
		}
	}
}

void Graphics::DrawFillRect(olc::PixelGameEngine* pge, int x, int y, int width, int height, uint32_t color)
{
}

void Graphics::DrawPolygon(olc::PixelGameEngine* pge, int x, int y, const std::vector<Vec2>& vertices, uint32_t color)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		int currIndex = i;
		int nextIndex = (i + 1) % vertices.size();
		DrawLine(pge, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color);
	}

	pge->DrawCircle(x, y, 1, color);
}

void Graphics::DrawFillPolygon(olc::PixelGameEngine* pge, int x, int y, const std::vector<Vec2>& vertices, uint32_t color)
{
}

void Graphics::DrawWrapedRotatedDecal(olc::PixelGameEngine* pge, olc::Decal* decal, std::array<olc::vf2d, 4>& pos, const float fangle, const olc::vf2d& center, const olc::vf2d& scale, const olc::Pixel& tint)
{
	std::array<olc::vf2d, 4> newPos = pos;
	olc::vf2d normPos[4];

	for (int i = 0; i < 4; i++)
	{
		normPos[i] = (pos[i] - center) * scale;
	}

	float sinVal = sin(fangle);
	float cosVal = cos(fangle);

	for (int i = 0; i < 4; i++) {
		newPos[i].x = center.x + normPos[i].x * cosVal - normPos[i].y * sinVal;
		newPos[i].y = center.y + normPos[i].y * cosVal + normPos[i].x * sinVal;
		// feedback to caller that warp pos has changed due to rotation
		pos = newPos;
	}

	// draw the decal using the rotated and scaled warp points
	pge->DrawWarpedDecal(decal, newPos, tint);
	// draw aux line to identify boundaries of warped decal - remove after testing
	for (int i = 0; i < 4; i++) {
		pge->DrawLine(newPos[i], newPos[(i + 1) % 4], olc::DARK_YELLOW);
	}
}
