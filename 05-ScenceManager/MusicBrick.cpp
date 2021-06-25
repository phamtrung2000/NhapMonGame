#include "MusicBrick.h"
#include "PlayScence.h"

MusicBrick::MusicBrick(float x, float y) : CGameObject()
{
	isCollision = false;
	Start_Y = y; // đúng
	this->x = x;
	this->y = y;
	ObjType = OBJECT_TYPE_MUSICBRICK;
	isInit = false;
	SetState(MUSICBRICK_STATE_NORMAL);
	Category = CATEGORY::OBJECT;
}

void MusicBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);

	if (isCollision == true)
	{
		y += dy;
		_Mario->y = this->y - _Mario->Height;
		
		if (Start_Y + MAX_HIGH < y)
		{
			vy = -BRICK_SPEED_Y;
		}

		if (int(y) == int(Start_Y) && vy == -BRICK_SPEED_Y)
		{
			SetState(MUSICBRICK_STATE_NORMAL);
			if(_Mario->pressS == false)
				_Mario->vy -= 0.3f;
			else
				_Mario->vy -= 0.35f;
		}
	}
}

void MusicBrick::Render()
{
	animation_set->at(ANI_BRICK_NORMAL)->Render(x, y);
}

void MusicBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case MUSICBRICK_STATE_NORMAL:
		{
			vx = vy = 0;
			isCollision = false;
		}
		break;

		case MUSICBRICK_STATE_COLLISION:
		{
			// viên gạch nảy xuong'
			vy = BRICK_SPEED_Y;
			isCollision = true;
		}break;
	}
}

void MusicBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + OBJECT_BBOX_WIDTH_HEIGHT;
	bottom = y + OBJECT_BBOX_WIDTH_HEIGHT;
}


