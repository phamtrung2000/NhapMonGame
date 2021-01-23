#include "Brick.h"
#include "PlayScence.h"

#define BRICK_NORMAL_ANI 0
#define BRICK_CLOUD_ANI 1

Brick::Brick()
{
	ObjType = OBJECT_TYPE_BRICK;
	vx = vy = 0.f;
	Type = -1;
	Width  = Height = OBJECT_BBOX_WIDTH_HEIGHT;
}

Brick::Brick(int type)
{
	ObjType = OBJECT_TYPE_BRICK;
	vx = vy = 0.f;
	Type = type;
	Category = CATEGORY::OBJECT;
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT;
}

void Brick::Render()
{
	int ani = BRICK_NORMAL_ANI;
	if (Type == BRICK_CLOUD)
		ani = BRICK_CLOUD_ANI;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void Brick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + Height;
}