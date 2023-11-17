#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "olcPixelGameEngine.h"
#include "Vec2.h"
#include <vector>

struct Graphics
{
    static void DrawLine(olc::PixelGameEngine* pge,int x0, int y0, int x1, int y1, uint32_t color);
    static void DrawCircle(olc::PixelGameEngine* pge, int x, int y, int radius, float angle, uint32_t color);
    static void DrawFillCircle(olc::PixelGameEngine* pge, int x, int y, int radius, uint32_t color);
    static void DrawRect(olc::PixelGameEngine* pge, int x, int y, int width, int height, uint32_t color);
    static void DrawFillRect(olc::PixelGameEngine* pge, int x, int y, int width, int height, uint32_t color);
    static void DrawPolygon(olc::PixelGameEngine* pge, int x, int y, const std::vector<Vec2>& vertices, uint32_t color);
    static void DrawFillPolygon(olc::PixelGameEngine* pge, int x, int y, const std::vector<Vec2>& vertices, uint32_t color);

   static void DrawWrapedRotatedDecal(olc::PixelGameEngine* pge, olc::Decal* decal, std::array<olc::vf2d, 4>& pos, const float fangle,
        const olc::vf2d& center = { 0.0f,0.0f }, const olc::vf2d& scale = { 0.0f,0.0f }, const olc::Pixel& tint = olc::WHITE);
};


#endif // !GRAPHICS_H



