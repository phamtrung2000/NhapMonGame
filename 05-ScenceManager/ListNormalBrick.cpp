#include "ListNormalBrick.h"
#include "PlayScence.h"

ListNormalBrick::ListNormalBrick(int number, vector<int> list, float startx, float starty)
{
	NumberBrick = number;
	Width = NumberBrick * OBJECT_BBOX_WIDTH_HEIGHT;
	Height = OBJECT_BBOX_WIDTH_HEIGHT;
	ObjType = OBJECT_TYPE_LISTNORMALBRICK;
	StartX = startx;
	StartY = starty;
	for (int i = 0; i < NumberBrick; i++)
	{
		int type = list.at(i);
		Brick* brick = new Brick(type, StartX + i * OBJECT_BBOX_WIDTH_HEIGHT, StartY);
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BRICK);
		brick->SetAnimationSet(ani_set);
		Bricks.push_back(brick);
	}
	TypeListNormalBrick = list.at(0);
}


void ListNormalBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}

void ListNormalBrick::Render()
{
	for (unsigned int i = 0; i < Bricks.size(); i++)
	{
		Bricks.at(i)->Render();
	}
}

