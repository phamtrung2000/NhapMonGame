﻿#pragma once
#include "GameObject.h"
#include "ItemBrick.h"
#include "Game.h"

class ListItemBrick : public ItemBrick
{
public:
	int NumberBrick;
	vector<ItemBrick*>Bricks;
	ListItemBrick()
	{
		NumberBrick = 0;
		Width = NumberBrick * OBJECT_BBOX_WIDTH_HEIGHT;
		Height = OBJECT_BBOX_WIDTH_HEIGHT;
		ObjType = OBJECT_TYPE_LISTITEMBRICK;
	}
	ListItemBrick(int number, vector<int>list, float startx, float starty)
	{
		NumberBrick = number;
		Width = NumberBrick * OBJECT_BBOX_WIDTH_HEIGHT;
		Height = OBJECT_BBOX_WIDTH_HEIGHT;
		ObjType = OBJECT_TYPE_LISTITEMBRICK;
		StartX = startx;
		StartY = starty;
		for (int i = 0; i < NumberBrick; i++)
		{
			int type = list.at(i);
			ItemBrick* brick = new ItemBrick(type, StartX + i * 16, StartY);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BRICK);
			brick->SetAnimationSet(ani_set);
			Bricks.push_back(brick);
		}
	}
	ListItemBrick(vector<ItemBrick*>list, int vitridau,int vitricuoi)
	{
		NumberBrick = vitricuoi - vitridau + 1;
		Width = NumberBrick * OBJECT_BBOX_WIDTH_HEIGHT;
		Height = OBJECT_BBOX_WIDTH_HEIGHT;
		ObjType = OBJECT_TYPE_LISTITEMBRICK;
		for (int i = vitridau; i <= vitricuoi; i++)
		{
			ItemBrick* brick = list.at(i);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BRICK);
			brick->SetAnimationSet(ani_set);
			Bricks.push_back(brick);
		}
		ItemBrick* firstbrick = Bricks.front();
		x = firstbrick->x;
		y = firstbrick->y;
	}
	
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void DeleteBrick(int vitri); // xóa gạch đồng thời tạo item (nếu có)
	int ViTriGachVaCham(float x, int width);
};

