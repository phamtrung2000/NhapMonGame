#include "ListItemBrick.h"
#include "BreakItemBrick.h"
#include "BrickItem.h"
#include "EffectSmoke.h"

void ListItemBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}

void ListItemBrick::Render()
{
	for (unsigned int i = 0; i < Bricks.size(); i++)
	{
		Bricks.at(i)->Render();
	}
}

void ListItemBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Bricks.size() == 0)
	{
		canDelete = true;
		return;
	}
	ItemBrick* firstbrick = Bricks.front();
	x = firstbrick->x;
	Width = Bricks.size() * 16;
	for (unsigned int i = 0; i < Bricks.size(); i++)
	{
		Bricks.at(i)->Update(dt,coObjects);
	}
}

void ListItemBrick::DeleteBrick(int vitri)
{
	ItemBrick* brick = Bricks.at(vitri);
	if (brick->hasItem == true && brick->isCollision==false)
	{
		brick->SetState(BRICK_STATE_COLLISION);
		switch (brick->Item)
		{
			case NORMAL:
			{
				BreakItemBrick* breakbrick1 = new BreakItemBrick(RIGHT, brick->x, brick->y, 0.1f, -0.3f);
				_Grid->AddMovingObject(breakbrick1, brick->x, brick->y);
				BreakItemBrick* breakbrick2 = new BreakItemBrick(RIGHT, brick->x, brick->y, 0.15f, -0.02f);
				_Grid->AddMovingObject(breakbrick2, brick->x, brick->y);
				BreakItemBrick* breakbrick3 = new BreakItemBrick(LEFT, brick->x, brick->y, 0.1f, -0.3f);
				_Grid->AddMovingObject(breakbrick3, brick->x, brick->y);
				BreakItemBrick* breakbrick4 = new BreakItemBrick(LEFT, brick->x, brick->y, 0.15f, -0.02f);
				_Grid->AddMovingObject(breakbrick4, brick->x, brick->y);
				Bricks.erase(Bricks.begin() + vitri);
			}
			break;

			case MUSHROOM:
			{
				BrickItem* brickitem = new BrickItem(MUSHROOM, brick->x, brick->y - 3);
				_Grid->AddMovingObject(brickitem, brick->x, brick->y - 3);
			}
			break;

			case BUTTONP:
			{
				BrickItem* brickitem = new BrickItem(BUTTONP, brick->x, brick->y - 16);
				_Grid->AddStaticObject(brickitem, brick->x, brick->y - 16);
				auto effect = new EffectSmoke(brick->x, brick->y - 16);
				_Grid->AddStaticObject(effect, brick->x, brick->y - 16);
			}
			break;

			case MONEYX10:
			{
				BrickItem* brickitem = new BrickItem(MONEYX10, brick->x, brick->y - 3);
				_Grid->AddMovingObject(brickitem, brick->x, brick->y - 3);
				brick->CountMoney--;
			}
			break;

			case ITEMBRICK_LEAF:
			{
				BrickItem* brickitem = new BrickItem(ITEMBRICK_LEAF, brick->x, brick->y - 3);
				_Grid->AddMovingObject(brickitem, brick->x, brick->y - 3);
			}
			break;
		}
	}
}

int ListItemBrick::ViTriGachVaCham(float Obj_x,int width)
{
	if (Bricks.size() == 1)
	{
		return 0;
	}
	else if (Bricks.size() == 2)
	{
		int vitri = 0;
		if ((Obj_x <= Bricks.at(0)->x) || (Obj_x + width / 2 <= Bricks.at(0)->x + 16 && Obj_x + Width / 2 > Bricks.at(0)->x))// sure đụng viên đầu tiên
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if (Obj_x <= Bricks.at(0)->x) // sure đụng viên đầu tiên
		{
			return 0;
		}
		else if (Obj_x + width >= Bricks.at(Bricks.size() - 1)->x + 16) // sure đụng viên cuối
		{
			return Bricks.size() - 1;
		}
		else
		{
			int vitri = int(Obj_x + width - Bricks.at(0)->x) / 16;
			float tempx = Bricks.at(vitri)->x;
			if (Obj_x < Bricks.at(vitri)->x && tempx - Obj_x > 8)
				vitri--;
			return vitri;
		}
	}
	return 0;
}
