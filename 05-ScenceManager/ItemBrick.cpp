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
}

void ItemBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);

	if (isDie == true)
	{
		BreakItemBrick* breakbrick1 = new BreakItemBrick(RIGHT, this->x, this->y, 0.05f, -0.1f);
		_PlayScene->objects.push_back(breakbrick1);
		BreakItemBrick* breakbrick2 = new BreakItemBrick(RIGHT, this->x, this->y, 0.05f, -0.02f);
		_PlayScene->objects.push_back(breakbrick2);
		BreakItemBrick* breakbrick3 = new BreakItemBrick(LEFT, this->x, this->y, 0.05f, -0.1f);
		_PlayScene->objects.push_back(breakbrick3);
		BreakItemBrick* breakbrick4 = new BreakItemBrick(LEFT, this->x, this->y, 0.05f, -0.02f);
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

			//
			if (Start_Y < y)
			{
				y = Start_Y;
				SetState(BRICK_STATE_EMPTY);
			}
		}
	}
	
}

void ItemBrick::Render()
{
	int ani = 0;
	if (vy == 0)
	{
		ani = ANI_BRICK_NORMAL;
		if (isCollision == true)
			ani = ANI_BRICK_COLLISION;
	}
	else
	{
		ani = ANI_BRICK_COLLISION;
	}
	/*if(isCollision==true)
		DebugOut(L"isCollision==true\n");
	else
		DebugOut(L"isCollision==false\n");
	DebugOut(L"State = %i, Ani = %i\n", state, ani);*/
	
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
		isCollision = true;
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

