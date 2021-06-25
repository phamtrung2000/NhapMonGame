#include "FlyGoomba.h"
#include "ListNormalBrick.h"

FlyGoomba::FlyGoomba() : Goomba()
{
	ObjType = OBJECT_TYPE_FLYGOOMBA;
	Health = 2;
	SetState(ENEMY_STATE_INIT);
	TimeMoving = TimeFlying = DelayUpdatePosition = 0;
	StartFlying = StopFlying = false;
	Up = Down = false;

}

void FlyGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (GetState() != ENEMY_STATE_DIE_IS_ATTACKED)
	{
		if (Health == 2)
		{
			left = x;
			top = y;
			right = x + FLYGOOMBA_BBOX_WIDTH;
			if (GetState() == ENEMY_STATE_WALKING_RIGHT || GetState() == ENEMY_STATE_WALKING_LEFT)
			{
				if(StartFlying == false)
					bottom = y + FLYGOOMBA_WINGDOWN_BBOX_HEIGHT;
				else
					bottom = y + FLYGOOMBA_WINGUP_BBOX_HEIGHT;
			}
			else
				bottom = y + FLYGOOMBA_WINGUP_BBOX_HEIGHT;
		}
		else
		{
			Goomba::GetBoundingBox(left, top, right, bottom);
		}
	}
}

void FlyGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

		if (GetState() == FLYGOOMBA_STATE_FLYING && StopFlying == false)
		{
			if (Down == true)
			{
				vy += FLYGOOMBA_FLYING_GRAVITY * dt;
			}
			else if (Up == true)
			{
				vy -= FLYGOOMBA_FLYING_GRAVITY * dt;
			}
			else
				vy += FLYGOOMBA_FLYING_GRAVITY * dt;

			ULONGLONG a = GetTickCount64() - TimeFlying;
			int b = a / 1000;
			if (a > 1500) // den' luc bay len lai
			{
				if (b % 2 == 0)
				{
					Up = true;
					Down = false;
				}
				else
				{
					Down = true;
					Up = false;
				}

				if (a > 5000)
				{
					StopFlying = true;
				}
			}
			//DebugOut(L"vy = %f, a = %u, b = %i\n", vy, a, b);
		}
		else
		{
			vy += ENEMY_GRAVITY * dt;
		}
		

		if (OnGroud == true)
		{
			if (TimeMoving != 0 && GetTickCount64() - TimeMoving > FLYGOOMBA_MOVINGTIME) // hết thời gian đi bộ thì chuyển state nhảy
			{
				TimeMoving = 0;
				StartFlying = true;
				y -= FLYGOOMBA_WINGUP_BBOX_HEIGHT - FLYGOOMBA_WINGDOWN_BBOX_HEIGHT;
				TimeFlying = GetTickCount64();
			}
			else if (StartFlying == true)
			{
				ULONGLONG a = GetTickCount64() - TimeFlying;
				if (TimeFlying != 0 && GetTickCount64() - TimeFlying > FLYGOOMBA_STARTTOFLYTIME) // hết thời gian đi bộ thì chuyển state nhảy
				{
					SetState(FLYGOOMBA_STATE_FLYING);
					TimeFlying = GetTickCount64();
					DelayUpdatePosition = GetTickCount64();
				}
			}
		}
		else if (GetState() == FLYGOOMBA_STATE_FLYING)
		{
			ULONGLONG a = GetTickCount64() - TimeFlying;
			int b = a / 1000;
			if (a > 1500) // den' luc bay len lai
			{
				if (b % 2 == 0)
				{
					if (DelayUpdatePosition != 0 && GetTickCount64() - DelayUpdatePosition > 1000)
					{
						DebugOut(L"UpdatePosition\n");
						UpdatePosition();
						DelayUpdatePosition = GetTickCount64();

					}
				}
			}
		}

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
}

void FlyGoomba::Render()
{
	int ani = 0;
	if (Health == 2)
	{
		if (GetState() == ENEMY_STATE_WALKING_LEFT || GetState() == ENEMY_STATE_WALKING_RIGHT)
		{
			ani = FLYGOOMBA_ANI_WALKING;
			if (StartFlying == true)
			{
				ani = FLYGOOMBA_ANI_READYTOFLY;
			}
		}
		else if (GetState() == FLYGOOMBA_STATE_FLYING)
		{
			ani = FLYGOOMBA_ANI_FLY;
		}
		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
		/*if (TimeMoving != 0)
			DebugOut(L"TimeMoving = %u\t", GetTickCount64() - TimeMoving);
		else
			DebugOut(L"TimeMoving = 0\t");*/
		if (TimeFlying != 0)
			DebugOut(L"TimeFlying = %u\t", GetTickCount64() - TimeFlying);
		else
			DebugOut(L"TimeFlying = 0\t");

		/*if (StartFlying == true)
			DebugOut(L"StartFlying == true\t");
		else
			DebugOut(L"StartFlying == false\t");
		if (OnGroud == true)
			DebugOut(L"OnGroud == true\t");
		else
			DebugOut(L"OnGroud == false\t");*/
		if (Up == true)
			DebugOut(L"Up == true\t");
		else
			DebugOut(L"Up == false\t");
		if (Down == true)
			DebugOut(L"Down == true\t");
		else
			DebugOut(L"Down == false\t");
		DebugOut(L"vx = %f ,vy = %f, state = %i, ani = %i\n",vx, vy, state, ani);
	}
	else
	{
		Goomba::Render();
	}
}

void FlyGoomba::SetState(int state)
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

		case FLYGOOMBA_STATE_FLYING:
		{
			vy = -FLYGOOMBA_JUMPING_HIGH_SPEED;
			OnGroud = false;
			StartFlying = false;
		}
		break;
		}
	}
	else
		Goomba::SetState(state);
}

void FlyGoomba::UpdatePosition()
{
	if (StopFlying == true)
	{
		if (this->x < _Mario->x)
		{
			SetState(ENEMY_STATE_WALKING_RIGHT);
		}
		else
		{
			SetState(ENEMY_STATE_WALKING_LEFT);
		}
	}
	else
	{
		if (this->x < _Mario->x)
		{
			if (GetState() == ENEMY_STATE_WALKING_LEFT)
				SetState(ENEMY_STATE_WALKING_RIGHT);
			else if (GetState() == FLYGOOMBA_STATE_FLYING)
			{
				vx = ENEMY_WALKING_SPEED;
				//vy = -vy;
			}
				
		}
		else
		{
			if (GetState() == ENEMY_STATE_WALKING_RIGHT)
				SetState(ENEMY_STATE_WALKING_LEFT);
			else if (GetState() == FLYGOOMBA_STATE_FLYING)
			{
				vx = -ENEMY_WALKING_SPEED;
				//vy = -vy;
			}
				
		}
	}

	
}

void FlyGoomba::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (Health == 2)
	{
		if (e->ny < 0)
		{
			if (state != FLYGOOMBA_STATE_FLYING)
				OnGroud = true; // xử lý chạm đất
			if (GetState() == ENEMY_STATE_INIT)
			{
				SetState(ENEMY_STATE_WALKING_LEFT);
				TimeMoving = GetTickCount64();
			}
		}

		if (e->obj != NULL)
		{
			if (e->obj->ObjType == OBJECT_TYPE_GROUND)
			{
				if (dynamic_cast<Ground*>(e->obj))
				{
					if (e->ny < 0)
					{
						// có đk state là vì khi setstate fly vy = -0.1 thì tới đây set vy = 0 lại k bay lên được
						if (ny != 0 && state != FLYGOOMBA_STATE_FLYING) vy = 0;
						x += min_tx * dx + nx * 0.2f;
						y += min_ty * dy + ny * 0.2f; // tiếp đất mượt hơn
						if (StopFlying == true)
						{
							UpdatePosition();
							TimeMoving = GetTickCount64();
							StopFlying = false;
						}
					}
					else if (e->nx != 0)
					{
						y += min_ty * dy + ny * 0.2f;
					}
				}
			}
			else if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
			{
				
				if (e->nx != 0)
				{
					if (ny != 0) vy = 0;
					x += dx;
				}
				else if (e->ny < 0)
				{
					if (ny != 0) vy = 0;
					x += min_tx * dx + nx * 0.4f;
				}
				else if (e->ny > 0)
				{
					y += dy;
				}
			}
			else if (e->obj->ObjType == OBJECT_TYPE_LISTNORMALBRICK)
			{
				if (ny != 0) vy = 0;
				ListNormalBrick* brick = dynamic_cast<ListNormalBrick*>(e->obj);
				if (e->nx != 0)
				{
					y += min_ty * dy + ny * 0.2f;
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
				else if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
				}
			}
			else
			{
				if (ny != 0) vy = 0;
				if (e->nx != 0)
				{
					y += min_ty * dy + ny * 0.1f - 0.4f;
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
				else if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
				}
			}
		}
	}
	else
	{
		Enemy::CollisionWithObject(e, min_tx, min_ty, nx, ny);
	}
}
