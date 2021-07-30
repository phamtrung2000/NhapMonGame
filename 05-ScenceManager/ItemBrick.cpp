#include "ItemBrick.h"
#include "PlayScence.h"
#include "BreakItemBrick.h"

ItemBrick::ItemBrick()
{
	isCollision = false;
	hasItem = true;
	StartY = y;
	ObjType = OBJECT_TYPE_ITEMBRICK;
	type_of_brick = BRICK_TYPE_ITEM;
	Item = 0;
	isInit = false;
	SetState(BRICK_STATE_NORMAL);
	CountMoney = 0;
	max_high = 5;
}

ItemBrick::ItemBrick(int item, float x, float y) : Brick(x, y)
{
	isCollision = false;
	hasItem = true;
	StartY = y;
	ObjType = OBJECT_TYPE_ITEMBRICK;
	type_of_brick = BRICK_TYPE_ITEM;
	Item = item;
	isInit = false;
	SetState(BRICK_STATE_NORMAL);
	if (item == MONEYX10)
		CountMoney = 10;
	else
		CountMoney = 0;
	max_high = 5;
}

void ItemBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);

	if (isDie == true)
	{
		BreakItemBrick* breakbrick1 = new BreakItemBrick(RIGHT, this->x, this->y, 0.1f, -0.3f);
		_Grid->AddMovingObject(breakbrick1, this->x, this->y);
		BreakItemBrick* breakbrick2 = new BreakItemBrick(RIGHT, this->x, this->y, 0.15f, -0.02f);
		_Grid->AddMovingObject(breakbrick2, this->x, this->y);
		BreakItemBrick* breakbrick3 = new BreakItemBrick(LEFT, this->x, this->y, 0.1f, -0.3f);
		_Grid->AddMovingObject(breakbrick3, this->x, this->y);
		BreakItemBrick* breakbrick4 = new BreakItemBrick(LEFT, this->x, this->y, 0.15f, -0.02f);
		_Grid->AddMovingObject(breakbrick4, this->x, this->y);
		canDelete = true;
	}
	else
	{
		if (isCollision == true && state != BRICK_STATE_EMPTY)
		{

			if (StartY - y >= max_high)
			{
				vy = ITEMBRICK_SPEED_Y;
			}
			y += dy;

			if (Item != MONEYX10)
			{
				if (StartY < y)
				{
					y = StartY;
					SetState(BRICK_STATE_EMPTY);
				}
			}
			else
			{
				if (StartY < y)
				{
					y = StartY;
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
		ani = ITEMBRICK_ANI_NORMAL;
	else
		ani = QUESTIONBRICK_ANI_COLLISION;
	animation_set->at(ani)->Render(x, y);
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

