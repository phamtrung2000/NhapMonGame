#include "QuestionBrick.h"
#include "PlayScence.h"

QuestionBrick::QuestionBrick()
{
	type_of_brick = BRICK_TYPE_QUESTION;
	isCollision = false;
	hasItem = true;
	StartY = y;
	Item = 0;
	SetState(BRICK_STATE_NORMAL);
	max_high = 5;
}

QuestionBrick::QuestionBrick(int item,float x,float y) : Brick(x,y)
{
	type_of_brick = BRICK_TYPE_QUESTION;
	isCollision =  false;
	hasItem = true;
	StartY = y; 
	Item = item;
	SetState(BRICK_STATE_NORMAL);
	max_high = 5;
}

void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) 
{
	CGameObject::Update(dt);

	if (isCollision==true && state != BRICK_STATE_EMPTY)
	{
		if (StartY - y >= max_high)
		{
			vy = BRICK_SPEED_Y;
		}
		y += dy;

		//
		if (StartY <  y)
		{
			y = StartY;
			SetState(BRICK_STATE_EMPTY);
		}
	}
}

void QuestionBrick::Render()
{
	int ani = 0;
	if (vy == 0)
	{
		ani = QUESTIONBRICK_ANI_NORMAL;
		if(isCollision==true)
			ani = QUESTIONBRICK_ANI_COLLISION;
	}
	else
	{
		ani = QUESTIONBRICK_ANI_COLLISION;
	}
	if (Item != 10)
		animation_set->at(ani)->Render(x, y);
}

void QuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case BRICK_STATE_NORMAL:
		{
			vx = vy = 0;
		}
		break;

		case BRICK_STATE_COLLISION:
		{
			// viên gạch nảy lên
			vy = -BRICK_SPEED_Y;
			isCollision = true;
		}break;

		case BRICK_STATE_EMPTY:
		{
			vy = 0;
			isCollision = true;
		}break;
	}
}

void QuestionBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + Width;
	bottom = y + Height;
}


