#pragma once
#include <raylib.h>
#include <cstdint>


class DrawAction
{
public:
DrawAction(Vector2 startPoint, Vector2 endPoint, uint32_t strokeWidth, Color color);
public:
	Vector2 startPoint{};
	Vector2 endPoint{};
	uint32_t strokeWidth;
	Color color;
};

