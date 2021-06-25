#include "HiddenMusicBrick.h"
#include "PlayScence.h"

HiddenMusicBrick::HiddenMusicBrick(float x, float y) : MusicBrick(x,y)
{
	isHidden = true;
	ObjType = OBJECT_TYPE_HIDDENMUSICBRICK;
	SetState(MUSICBRICK_STATE_HIDDEN);
	direction = -1;
	Start_X = x;
}

void HiddenMusicBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	if (GetState() == MUSICBRICK_STATE_APPEAR)
	{
		if (Start_Y - y >= MAX_HIGH)
		{
			vy = BRICK_SPEED_Y;
		}
		y += dy;

		if (Start_Y < y)
		{
			y = Start_Y;
			SetState(MUSICBRICK_STATE_NORMAL);
		}
	}
	else if (isCollision == true)
	{
		if (direction == 1)
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
				if (_Mario->pressS == false)
					_Mario->vy -= 0.3f;
				else
					_Mario->vy -= 0.35f;
			}
		}
		else if (direction == 3)
		{
			if (Start_Y - y >= MAX_HIGH)
			{
				vy = BRICK_SPEED_Y;
			}
			y += dy;

			if (Start_Y < y)
			{
				y = Start_Y;
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
		else if (direction == 0)
		{
			if (abs(Start_X - x) >= MAX_HIGH)
			{
				vx = -BRICK_SPEED_Y;
			}
			x += dx;

			if (x < Start_X)
			{
				x = Start_X;
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
		else if (direction == 2) // phai -> trai
		{
			if (abs(Start_X - x) >= MAX_HIGH)
			{
				vx = BRICK_SPEED_Y;
			}
			x += dx;

			if (x > Start_X)
			{
				x = Start_X;
				SetState(MUSICBRICK_STATE_NORMAL);
			}
		}
	}
}

void HiddenMusicBrick::Render()
{
	if (isHidden == false)
		animation_set->at(ANI_BRICK_NORMAL)->Render(x, y);
	RenderBoundingBox();
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
				case 1: // tren -> duoi
				{
					vy = BRICK_SPEED_Y;
					vx = 0;
				}
				break;

				case 3: // duoi -> tren
				{
					vy = -BRICK_SPEED_Y;
					vx = 0;
				}
				break;

				case 0: // trai -> phai
				{
					vx = BRICK_SPEED_Y;
					vy = 0;
				}
				break;

				case 2: // phai -> trai
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


