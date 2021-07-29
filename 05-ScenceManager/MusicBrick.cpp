#include "MusicBrick.h"
#include "PlayScence.h"

MusicBrick::MusicBrick(float x, float y) : Brick(x, y)
{
	isCollision = isCollisionMario = false;
	StartY = y; 
	ObjType = OBJECT_TYPE_MUSICBRICK;
	type_of_brick = BRICK_TYPE_MUSIC;
	SetState(MUSICBRICK_STATE_NORMAL);
	direction = -1;
	max_high = 8;
}

void MusicBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);

	if (isCollision == true && isCollisionMario == false)
	{
		if (direction == TOP) // tác động từ trên xuống
		{
			y += dy;
			if (StartY + max_high < y)
			{
				vy = -BRICK_SPEED_Y;
			}

			if (int(y) == int(StartY) && vy == -BRICK_SPEED_Y)
			{
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
		else if (direction == BOTTOM) // tác động từ dưới lên
		{
			y += dy;
			if (StartY - y >= max_high)
			{
				vy = BRICK_SPEED_Y;
			}

			if (StartY < y)
			{
				y = StartY;
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
	}
	else if(isCollisionMario == true)
	{
		if (direction == TOP) // tác động từ trên xuống
		{
			y += dy;
			_Mario->y = this->y - _Mario->Height;
			if (StartY + max_high < y)
			{
				vy = -BRICK_SPEED_Y;
			}

			if (int(y) == int(StartY) && vy == -BRICK_SPEED_Y)
			{
				SetState(MUSICBRICK_STATE_NORMAL);
				if (_Mario->pressS == false)
					_Mario->vy -= 0.3f;
				else
					_Mario->vy -= 0.35f;
			}
		}
		else if (direction == BOTTOM) // tác động từ dưới lên
		{
			y += dy;
			if (StartY - y >= max_high)
			{
				vy = BRICK_SPEED_Y;
			}

			if (StartY < y)
			{
				y = StartY;
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
	}
}

void MusicBrick::Render()
{
	animation_set->at(MUSICBRICK_ANI_NORMAL)->Render(x, y);
}

void MusicBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case MUSICBRICK_STATE_NORMAL:
		{
			vx = vy = 0;
			isCollision = isCollisionMario = false;
		}
		break;

		case MUSICBRICK_STATE_COLLISION:
		{
			switch (direction)
			{
				case TOP: // tren -> duoi
				{
					vy = BRICK_SPEED_Y;
					vx = 0;
				}
				break;

				case BOTTOM: // duoi -> tren
				{
					vy = -BRICK_SPEED_Y;
					vx = 0;
				}
				break;
			}
			isCollision = true;
		}break;
	}
}

void MusicBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + Width;
	bottom = y + Height;
}


