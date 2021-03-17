#pragma once
#include "GameObject.h"
#include "ItemBrick.h"
#include "Game.h"

class ListBrick : public CGameObject
{
public:
	int NumberBrick;
	vector<ItemBrick*>Bricks;
	ListBrick(int number,float startx,float starty)
	{
		NumberBrick = number;
		Width = NumberBrick * 16;
		Height = 16;
		ObjType = 24;
		Category = CATEGORY::OBJECT;
		StartX = startx;
		StartY = starty;
		for (int i = 0; i < NumberBrick; i++)
		{
			ItemBrick* brick = new ItemBrick(0, StartX + i * 16, StartY);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(13);
			brick->SetAnimationSet(ani_set);
			//_PlayScene->objects.push_back(brick);
			Bricks.push_back(brick);
		}
	}
	ListBrick(int number, vector<int>list, float startx, float starty)
	{
		NumberBrick = number;
		Width = NumberBrick * 16;
		Height = 16;
		ObjType = 24;
		Category = CATEGORY::OBJECT;
		StartX = startx;
		StartY = starty;
		for (int i = 0; i < NumberBrick; i++)
		{
			int type = list.at(i);
			ItemBrick* brick = new ItemBrick(type, StartX + i * 16, StartY);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(13);
			brick->SetAnimationSet(ani_set);
			//_PlayScene->objects.push_back(brick);
			Bricks.push_back(brick);
		}
	}
	
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void DeleteBrick(int vitri);
};

