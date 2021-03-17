#include "ListBrick.h"
#include "BreakItemBrick.h"
#include "BrickItem.h"
#include "EffectSmoke.h"

void ListBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}

void ListBrick::Render()
{
	for (int i = 0; i < Bricks.size(); i++)
	{
		Bricks.at(i)->Render();
	}
	RenderBoundingBox();
}

void ListBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Bricks.size() == 0)
	{
		canDelete = true;
		return;
	}
	for (int i = 0; i < Bricks.size(); i++)
	{
		Bricks.at(i)->Update(dt,coObjects);
	}
	ItemBrick* firstbrick = Bricks.front();
	x = firstbrick->x;
	y = firstbrick->y;
	Width = Bricks.size() * 16;
}

void ListBrick::DeleteBrick(int vitri)
{
	ItemBrick* brick = Bricks.at(vitri);
	if (brick->hasItem == true)
	{
		brick->SetState(BRICK_STATE_COLLISION);
		switch (brick->Item)
		{
		case BUTTONP:
		{
			BrickItem* brickitem = new BrickItem(BUTTONP, brick->x, brick->y - 16);
			_PlayScene->objects.push_back(brickitem);
			auto effect = new EffectSmoke(brick->x, brick->y - 16);
			_PlayScene->objects.push_back(effect);
		}
		break;

		case MUSHROOM:
		{
			BrickItem* brickitem = new BrickItem(MUSHROOM, brick->x, brick->y - 3);
			_PlayScene->objects.push_back(brickitem);
		}
		break;

		case NORMAL:
		{
			BreakItemBrick* breakbrick1 = new BreakItemBrick(RIGHT, brick->x, brick->y, 0.1f, -0.3f);
			_PlayScene->objects.push_back(breakbrick1);
			BreakItemBrick* breakbrick2 = new BreakItemBrick(RIGHT, brick->x, brick->y, 0.15f, -0.02f);
			_PlayScene->objects.push_back(breakbrick2);
			BreakItemBrick* breakbrick3 = new BreakItemBrick(LEFT, brick->x, brick->y, 0.1f, -0.3f);
			_PlayScene->objects.push_back(breakbrick3);
			BreakItemBrick* breakbrick4 = new BreakItemBrick(LEFT, brick->x, brick->y, 0.15f, -0.02f);
			_PlayScene->objects.push_back(breakbrick4);
		}
		break;

		case MONEYX10:
		{
			BrickItem* brickitem = new BrickItem(MONEYX10, brick->x, brick->y - 3);
			_PlayScene->objects.push_back(brickitem);
			brick->CountMoney--;
		}

		}
	}
	
}
