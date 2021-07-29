#include "Block.h"
#include "PlayScence.h"

void Block::Render()
{
	
}

Block::Block(int W, int H)
{
	ObjType = OBJECT_TYPE_BLOCK;
	Width =  W;
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
