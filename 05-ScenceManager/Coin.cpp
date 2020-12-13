#include "Coin.h"

void Coin::Render()
{
	animation_set->at(COIN_ANI)->Render(x, y);
	RenderBoundingBox();
}

void Coin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}

Coin::Coin()
{
	ObjType = 12;
	vx = vy = 0;
	Category = CATEGORY::ITEM;
}