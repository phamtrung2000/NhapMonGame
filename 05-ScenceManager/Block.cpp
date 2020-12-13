#include "Block.h"

void Block::Render()
{
	RenderBoundingBox();
}

Block::Block(int W, int H)
{
	ObjType = 6;
	Width = W;
	Height = H;
	Category = CATEGORY::OBJECT;
}

void Block::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}
