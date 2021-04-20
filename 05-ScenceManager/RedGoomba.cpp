#include "RedGoomba.h"
#include "Camera.h"
#include "Mario.h"


RedGoomba::RedGoomba() : Goomba()
{
	ObjType = OBJECT_TYPE_REDGOOMBA;
	Health = 2;
	SetState(ENEMY_STATE_WALKING_LEFT);
	JumpCount = 0;
	TimeMoving = GetTickCount64();
}

void RedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (GetState() != ENEMY_STATE_DIE_IS_ATTACKED)
	{
		if (Health == 2)
		{
			left = x;
			top = y;
			right = x + REDGOOMBA_BBOX_WIDTH;
			if (GetState() == ENEMY_STATE_WALKING_LEFT || GetState() == ENEMY_STATE_WALKING_LEFT)
				bottom = y + REDGOOMBA_WINGUP_BBOX_HEIGHT;
			else
				bottom = y + REDGOOMBA_WINGUP_BBOX_HEIGHT;
		}
		else
		{
			Goomba::GetBoundingBox(left, top, right, bottom);
		}
	}
}

void RedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Health == 2)
	{
		CGameObject::Update(dt, coObjects);

		if (isAttacked == true)
		{
			if (Time_isAttacked != 0 && GetTickCount64() - Time_isAttacked > ENEMY_TIME_ISATTACKED)
			{
				isAttacked = false;
				Time_isAttacked = 0;
			}
		}

		UpdatePosition(dt);
		vy += ENEMY_GRAVITY * dt;
		
		
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (e->obj)
				{
					switch (e->obj->Category)
					{
						case CATEGORY::OBJECT:
							CollisionWithObject(e, min_tx, min_ty, nx, ny);
							break;

						case CATEGORY::ENEMY:
							CollisionWithEnemy(e, min_tx, min_ty, nx, ny);
							break;

						case CATEGORY::ITEM:
							CollisionWithItem(e, min_tx, min_ty, nx, ny);
							break;

						case CATEGORY::WEAPON:
							CollisionWithWeapon(e, min_tx, min_ty, nx, ny);
							break;
					}
				}
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	else
	{
		if (GetState() == ENEMY_STATE_JUMPING_LOW_RIGHT || state == ENEMY_STATE_JUMPING_HIGH_RIGHT)
			SetState(ENEMY_STATE_WALKING_RIGHT);
		else if (GetState() == ENEMY_STATE_JUMPING_LOW_LEFT || state == ENEMY_STATE_JUMPING_HIGH_LEFT)
			SetState(ENEMY_STATE_WALKING_LEFT);
		Goomba::Update(dt, coObjects);
	}
	DebugOut(L"State = %i\n", state);
}

void RedGoomba::Render()
{
	int ani = 0;
	if (Health == 2)
	{
		if (GetState() == ENEMY_STATE_WALKING_LEFT || GetState() == ENEMY_STATE_WALKING_RIGHT)
		{
			ani = REDGOOMBA_ANI_WALKING;
		}
		else if (GetState() == ENEMY_STATE_JUMPING_LOW_RIGHT || GetState() == ENEMY_STATE_JUMPING_LOW_LEFT)
		{
			ani = REDGOOMBA_ANI_JUMPING_LOW;
		}
		else if(GetState() == ENEMY_STATE_JUMPING_HIGH_RIGHT || GetState() == ENEMY_STATE_JUMPING_HIGH_LEFT)
		{
			ani = REDGOOMBA_ANI_JUMPING_HIGH;
		}
		animation_set->at(ani)->Render(x, y);
	}
	else
	{
		Goomba::Render();
	}
}

void RedGoomba::SetState(int state)
{
	if (Health == 2)
	{
		CGameObject::SetState(state);
		switch (state)
		{
			case ENEMY_STATE_DIE_IS_JUMPED:
			{
				vx = vy = 0;
				isDie = true;
			}
			break;

			case ENEMY_STATE_DIE_IS_ATTACKED:
			{
				Health = 0;
				vy = -ENEMY_DIE_DEFLECT_SPEED;
				vx = nx * abs(vx);
				isDie = true;
			}
			break;

			case ENEMY_STATE_INIT:
			{
				vx = 0.0f;
				vy = 0.1f;
			}
			break;

			case ENEMY_STATE_WALKING_RIGHT:
			{
				nx = RIGHT;
				vx = nx * ENEMY_WALKING_SPEED;
				vy = 0;
			}
			break;

			case ENEMY_STATE_WALKING_LEFT:
			{
				nx = LEFT;
				vx = nx * ENEMY_WALKING_SPEED;
				vy = 0;
			}
			break;

			case ENEMY_STATE_JUMPING_HIGH_RIGHT:
			{
				nx = RIGHT;
				vx = nx * ENEMY_WALKING_SPEED;
				vy = -REDGOOMBA_JUMPING_HIGH_SPEED;
			}
			break;

			case ENEMY_STATE_JUMPING_HIGH_LEFT:
			{
				nx = LEFT;
				vx = nx * ENEMY_WALKING_SPEED;
				vy = -REDGOOMBA_JUMPING_HIGH_SPEED;
			}
			break;

			case ENEMY_STATE_JUMPING_LOW_RIGHT:
			{
				nx = RIGHT;
				vx = nx * ENEMY_WALKING_SPEED;
				vy = -REDGOOMBA_JUMPING_LOW_SPEED;
			}
			break;

			case ENEMY_STATE_JUMPING_LOW_LEFT:
			{
				nx = LEFT;
				vx = nx * ENEMY_WALKING_SPEED;
				vy = -REDGOOMBA_JUMPING_LOW_SPEED;
			}
			break;
		}
	}
	else
		Goomba::SetState(state);
}

void RedGoomba::UpdatePosition(DWORD dt)
{
	/*if (this->x < _Mario->x)
	{
		if (GetState() == ENEMY_STATE_WALKING_LEFT)
			SetState(ENEMY_STATE_WALKING_RIGHT);
		else if (GetState() == ENEMY_STATE_JUMPING_HIGH_LEFT)
			SetState(ENEMY_STATE_JUMPING_HIGH_RIGHT);
		else if (GetState() == ENEMY_STATE_JUMPING_LOW_LEFT)
			SetState(ENEMY_STATE_JUMPING_LOW_RIGHT);
	}
	else
	{
		if (GetState() == ENEMY_STATE_WALKING_RIGHT)
			SetState(ENEMY_STATE_WALKING_LEFT);
		else if (GetState() == ENEMY_STATE_JUMPING_HIGH_RIGHT)
			SetState(ENEMY_STATE_JUMPING_HIGH_LEFT);
		else if (GetState() == ENEMY_STATE_JUMPING_LOW_RIGHT)
			SetState(ENEMY_STATE_JUMPING_LOW_LEFT);
	}*/
}

void RedGoomba::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (Health == 2)
	{
		if (e->ny < 0)
		{
			OnGroud = true;
			if (GetState() == ENEMY_STATE_INIT)
				SetState(ENEMY_STATE_WALKING_LEFT);
		}

		if (e->obj != NULL)
		{
			if (e->obj->ObjType == OBJECT_TYPE_GROUND)
			{
				if (ny != 0) vy = 0;
				if (e->ny < 0 && Health == 2)
				{
					if (TimeMoving != 0 && GetTickCount64() - TimeMoving > REDGOOMBA_MOVINGTIME) // hết thời gian đi bộ thì chuyển state nhảy
					{
						TimeMoving = 0;
						if (this->nx == LEFT)
							SetState(ENEMY_STATE_JUMPING_LOW_LEFT);
						else
							SetState(ENEMY_STATE_JUMPING_LOW_RIGHT);
						JumpCount++;
					}
					else
					{
						if (JumpCount < 3 && JumpCount > 0)
						{
							if (this->nx == LEFT)
								SetState(ENEMY_STATE_JUMPING_LOW_LEFT);
							else
								SetState(ENEMY_STATE_JUMPING_LOW_RIGHT);
							JumpCount++;
						}
						else if (JumpCount == 3)
						{
							if (this->nx == LEFT)
								SetState(ENEMY_STATE_JUMPING_HIGH_LEFT);
							else
								SetState(ENEMY_STATE_JUMPING_HIGH_RIGHT);
							JumpCount = 0;
						}
						else if (JumpCount == 0 && TimeMoving == 0)
						{
							TimeMoving = GetTickCount64();
							if (this->nx == LEFT)
								SetState(ENEMY_STATE_WALKING_LEFT);
							else
								SetState(ENEMY_STATE_WALKING_RIGHT);
						}
					}
				}
				if (dynamic_cast<Ground*>(e->obj))
				{
					if (e->ny < 0)
					{
						x += min_tx * dx + nx * 0.4f;
					}
					else if (e->nx != 0)
					{
						y += min_ty * dy + ny * 0.1f - 0.3f;
						if (GetState() == ENEMY_STATE_WALKING_RIGHT)
							SetState(ENEMY_STATE_WALKING_LEFT);
						else if (GetState() == ENEMY_STATE_WALKING_LEFT)
							SetState(ENEMY_STATE_WALKING_RIGHT);
						else if (GetState() == ENEMY_STATE_JUMPING_HIGH_RIGHT)
							SetState(ENEMY_STATE_JUMPING_HIGH_LEFT);
						else if (GetState() == ENEMY_STATE_JUMPING_HIGH_LEFT)
							SetState(ENEMY_STATE_JUMPING_HIGH_RIGHT);
						else if (GetState() == ENEMY_STATE_JUMPING_LOW_RIGHT)
							SetState(ENEMY_STATE_JUMPING_LOW_LEFT);
						else if (GetState() == ENEMY_STATE_JUMPING_LOW_LEFT)
							SetState(ENEMY_STATE_JUMPING_LOW_RIGHT);
					}
				}
			}
			else if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
			{
				if (e->nx != 0)
				{
					x += dx;
				}
				else if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
				}
				if (ny != 0) vy = 0;
			}
			else
			{
				if (e->nx != 0)
				{
					int state = GetState();
					if (GetState() == ENEMY_STATE_WALKING_RIGHT)
						SetState(ENEMY_STATE_WALKING_LEFT);
					else if (GetState() == ENEMY_STATE_WALKING_LEFT)
						SetState(ENEMY_STATE_WALKING_RIGHT);
					else if (GetState() == ENEMY_STATE_JUMPING_HIGH_RIGHT)
						SetState(ENEMY_STATE_JUMPING_HIGH_LEFT);
					else if (GetState() == ENEMY_STATE_JUMPING_HIGH_LEFT)
						SetState(ENEMY_STATE_JUMPING_HIGH_RIGHT);
					else if (GetState() == ENEMY_STATE_JUMPING_LOW_RIGHT)
						SetState(ENEMY_STATE_JUMPING_LOW_LEFT);
					else if (GetState() == ENEMY_STATE_JUMPING_LOW_LEFT)
						SetState(ENEMY_STATE_JUMPING_LOW_RIGHT);
				}
				else
				{
					x += min_tx * dx + nx * 0.4f;
				}
				if (ny != 0) vy = 0;
			}
		}
	}
	else
	{
		Enemy::CollisionWithObject(e, min_tx, min_ty, nx, ny);
	}
}
