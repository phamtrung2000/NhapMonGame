#include "Brick.h"
#include "PlayScence.h"


Brick::Brick()
{
	ObjType = OBJECT_TYPE_BRICK;
	Type = max_high = 0;
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT;
	type_of_brick = BRICK_TYPE_NORMAL;
}

Brick::Brick(float x, float y)
{
	this->x = x;
	this->y = y;
	ObjType = OBJECT_TYPE_BRICK;
	Type = 0;
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT;
	type_of_brick = BRICK_TYPE_NORMAL;
}

Brick::Brick(int type, float x, float y)
{
	this->x = x;
	this->y = y;

	ObjType = OBJECT_TYPE_BRICK;
	Type = type;
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT;
	type_of_brick = BRICK_TYPE_NORMAL;
}

void Brick::Render()
{
	int ani = NORMALBRICK_ANI_NORMAL;
	if (Type == BRICK_CLOUD)
		ani = NORMALBRICK_ANI_CLOUD;
	animation_set->at(ani)->Render(x, y);
}

void Brick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + Height;
}