#include "HiddenMusicBrick.h"
#include "PlayScence.h"

HiddenMusicBrick::HiddenMusicBrick(float x, float y) : MusicBrick(x,y)
{
	isHidden = true;
	ObjType = OBJECT_TYPE_HIDDENMUSICBRICK;
	SetState(MUSICBRICK_STATE_HIDDEN);
	StartX = x;
}

void HiddenMusicBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	if (GetState() == MUSICBRICK_STATE_APPEAR)
	{
		if (StartY - y >= max_high)
		{
			vy = BRICK_SPEED_Y;
		}
		y += dy;

		if (StartY < y)
		{
			y = StartY;
			SetState(MUSICBRICK_STATE_NORMAL);
		}
	}
	else if (isCollision == true)
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
				{
					_Mario->vy -= 0.5f;
					_Mario->GoHiddenWorld = true;
					_Mario->StartGoHiddenWorld = GetTickCount64();
					_Mario->Scene = 13;
					_Mario->time_switch_scene = 400;
				}
			}
		}
		else if (direction == BOTTOM) // tác động từ dưới lên
		{
			if (StartY - y >= max_high)
			{
				vy = BRICK_SPEED_Y;
			}
			y += dy;

			if (StartY < y)
			{
				y = StartY;
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
		else if (direction == LEFT)
		{
			if (abs(StartX - x) >= max_high)
			{
				vx = -BRICK_SPEED_Y;
			}
			x += dx;

			if (x < StartX)
			{
				x = StartX;
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
		else if (direction == RIGHT) // phai -> trai
		{
			if (abs(StartX - x) >= max_high)
			{
				vx = BRICK_SPEED_Y;
			}
			x += dx;

			if (x > StartX)
			{
				x = StartX;
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
	}
}

void HiddenMusicBrick::Render()
{
	if (isHidden == false)
		animation_set->at(HIDDENMUSICBRICK_ANI_NORMAL)->Render(x, y);
}

void HiddenMusicBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case MUSICBRICK_STATE_NORMAL:
		{
			vx = vy = 0;
			isCollision = false;
			isHidden = false;
			direction = -1;
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

				case LEFT: // trai -> phai
				{
					vx = BRICK_SPEED_Y;
					vy = 0;
				}
				break;

				case RIGHT: // phai -> trai
				{
					vx = -BRICK_SPEED_Y;
					vy = 0;
				}
				break;
			}
			
			isCollision = true;
		}break;

		case MUSICBRICK_STATE_HIDDEN:
		{
			vx = vy = 0;
			isCollision = false;
			isHidden = true;
		}
		break;

		case MUSICBRICK_STATE_APPEAR:
		{
			vy = -BRICK_SPEED_Y;
			isHidden = false;
		}
		break;
		
	}
}

void HiddenMusicBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + OBJECT_BBOX_WIDTH_HEIGHT;
	bottom = y + OBJECT_BBOX_WIDTH_HEIGHT;
}


