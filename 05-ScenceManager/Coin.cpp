#include "Coin.h"
#include "PlayScence.h"

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
	
	ObjType = OBJECT_TYPE_COIN;
	vx = vy = 0;
	Category = CATEGORY::ITEM;
}

Coin::Coin(float x, float y)
{
	ObjType = OBJECT_TYPE_COIN;
	vx = vy = 0;
	Category = CATEGORY::ITEM;
	SetPosition(x, y);
}
