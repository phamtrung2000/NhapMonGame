#include "ItemBrick.h"
#include "PlayScence.h"
#include "BreakItemBrick.h"

ItemBrick::ItemBrick(int item, float x, float y) : CGameObject()
{
	isCollision = false;
	hasItem = true;
	Start_Y = y; // đúng
	this->x = x;
	this->y = y;
	ObjType = OBJECT_TYPE_ITEMBRICK;
	Item = item;
	isInit = false;
	SetState(BRICK_STATE_NORMAL);
	Category = CATEGORY::OBJECT;
	if (item == MONEYX10)
		CountMoney = 10;
	else
		CountMoney = 0;
}

void ItemBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);

	if (isDie == true)
	{
		BreakItemBrick* breakbrick1 = new BreakItemBrick(RIGHT, this->x, this->y, 0.1f, -0.3f);
		_PlayScene->objects.push_back(breakbrick1);
		BreakItemBrick* breakbrick2 = new BreakItemBrick(RIGHT, this->x, this->y, 0.15f, -0.02f);
		_PlayScene->objects.push_back(breakbrick2);
		BreakItemBrick* breakbrick3 = new BreakItemBrick(LEFT, this->x, this->y, 0.1f, -0.3f);
		_PlayScene->objects.push_back(breakbrick3);
		BreakItemBrick* breakbrick4 = new BreakItemBrick(LEFT, this->x, this->y, 0.15f, -0.02f);
		_PlayScene->objects.push_back(breakbrick4);
		canDelete = true;
	}
	else
	{
		if (isCollision == true && state != BRICK_STATE_EMPTY)
		{

			if (Start_Y - y >= MAX_HIGH)
			{
				vy = ITEMBRICK_SPEED_Y;
			}
			y += dy;

			if (Item != MONEYX10)
			{
				if (Start_Y < y)
				{
					y = Start_Y;
					SetState(BRICK_STATE_EMPTY);
				}
			}
			else
			{
				if (Start_Y < y)
				{
					y = Start_Y;
					SetState(BRICK_STATE_NORMAL);
				}
				//CountMoney--;
				if(CountMoney == 0)
					SetState(BRICK_STATE_EMPTY);
			}
		}
	}
	
}

void ItemBrick::Render()
{
	int ani = 0;
	if (hasItem == true)
		ani = ANI_BRICK_NORMAL;
	else
		ani = ANI_BRICK_COLLISION;

	/*	DebugOut(L"isCollision==true\n");
	else
		DebugOut(L"isCollision==false\n");
	DebugOut(L"State = %i, Ani = %i, count = %i\n", state, ani, CountMoney);*/
	
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void ItemBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICK_STATE_NORMAL:
	{
		vy = 0;
		isCollision = false;
	}
	break;

	case BRICK_STATE_COLLISION:
	{
		// viên gạch nảy lên
		vy = -ITEMBRICK_SPEED_Y;
		isCollision = true;
	}break;

	case BRICK_STATE_EMPTY:
	{
		vy = 0;
		hasItem = false;
	}break;

	case ITEMBRICK_STATE_DIE:
	{
		isDie = true;
	}
	break; 
	
	}

}

void ItemBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEMBRICK_WIDTH;
	bottom = y + ITEMBRICK_WIDTH;
}

