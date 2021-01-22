#include "Brick.h"
#include "PlayScence.h"

Brick::Brick()
{
	ObjType = OBJECT_TYPE_BRICK;
	vx = vy = 0.f;
	Type = -1;
}

Brick::Brick(int type)
{
	ObjType = 1;
	vx = vy = 0.f;
	Type = type;
	Category = CATEGORY::OBJECT;
}

void Brick::Render()
{
	int ani = 0;
	if (Type == CLOUD)
		ani = 1;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void Brick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}