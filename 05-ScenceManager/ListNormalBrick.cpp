#include "ListNormalBrick.h"

ListNormalBrick::ListNormalBrick(int number, vector<int> list, float startx, float starty)
{
	NumberBrick = number;
	Width = NumberBrick * 16;
	Height = 16;
	ObjType = OBJECT_TYPE_LISTNORMALBRICK;
	Category = CATEGORY::OBJECT;
	StartX = startx;
	StartY = starty;
	for (int i = 0; i < NumberBrick; i++)
	{
		int type = list.at(i);
		Brick* brick = new Brick(type, StartX + i * 16, StartY);
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

