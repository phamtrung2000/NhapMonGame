#include "Ground.h"

void Ground::Render()
{
	RenderBoundingBox();
}

void Ground::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}
