#include "GreenFlyKoopas.h"

GreenFlyKoopas::GreenFlyKoopas()
{
	SetState(GREENKOOPAS_STATE_WALKING_LEFT);
	Revive = isHold = isShell = isShell_2 = false;
	ObjType = OBJECT_TYPE_GREENFLYKOOPAS;
	Health = 2;
}

GreenFlyKoopas::~GreenFlyKoopas()
{
}

void GreenFlyKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GREENKOOPAS_BBOX_WIDTH;
	if (isShell == true || isShell_2 == true)
		bottom = y + GREENKOOPAS_BBOX_HEIGHT_SHELL;
	else
		bottom = y + GREENKOOPAS_BBOX_HEIGHT;
	if (isHold == true)
	{
		left = left - 1.f;
		right = right - 1.f;
	}
}

void GreenFlyKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (isShell == false && isShell_2 == false)
	{
		vy += GREENKOOPAS_GRAVITY * dt;
	}
	else if (isShell == true && isHold != true)
	{
		vy += GREENKOOPAS_SHELL_GRAVITY * dt;
	}
	// vẫy đuôi -> rùa lật ngửa rớt từ trên xuống
	else if (isShell_2 == true && isHold != true)
	{
		vy += GREENKOOPAS_SHELL_2_GRAVITY * dt;
	}
	if (state == GOOMBA_STATE_DIE_2)
	{
		if (y > 450)
			isDie = true;
	}
	
	if ((GetState() == GREENKOOPAS_STATE_SHELL || GetState() == GREENKOOPAS_STATE_SHELL_2 || GetState() == GREENKOOPAS_STATE_SHELL_HOLD)
		&& GetTickCount64() - ReviveTime > GREENKOOPAS_START_REVIVE_TIME)
	{
		Revive = true;
		if (GetTickCount64() - ReviveTime > GREENKOOPAS_REVIVE_TIME)
		{
			ReviveTime = 0;
			Revive = false;
			y = (INT16)(y - (GREENKOOPAS_BBOX_HEIGHT - GREENKOOPAS_BBOX_HEIGHT_SHELL) - 1);
			if (nx == LEFT)
				SetState(GREENKOOPAS_STATE_WALKING_RIGHT);
			else
				SetState(GREENKOOPAS_STATE_WALKING_LEFT);
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != GREENKOOPAS_STATE_DIE)
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

		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->ny < 0 && Health == 2)
				vy = -0.2;
			if (dynamic_cast<WarpPipe*>(e->obj))
			{
				if (e->nx != 0)
				{
					if (isShell == false && isShell_2 == false)
					{
						if (GetState() == GREENKOOPAS_STATE_WALKING_RIGHT)
							SetState(GREENKOOPAS_STATE_WALKING_LEFT);
						else if (GetState() == GREENKOOPAS_STATE_WALKING_LEFT)
							SetState(GREENKOOPAS_STATE_WALKING_RIGHT);
					}
					else
					{
						if (GetState() == GREENKOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(GREENKOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == GREENKOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(GREENKOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
				}
			}
			else if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
				if (e->nx != 0)
				{
					if (isShell == false && isShell_2 == false)
					{
						if (GetState() == GREENKOOPAS_STATE_WALKING_RIGHT)
							SetState(GREENKOOPAS_STATE_WALKING_LEFT);
						else if (GetState() == GREENKOOPAS_STATE_WALKING_LEFT)
							SetState(GREENKOOPAS_STATE_WALKING_RIGHT);
					}
					else
					{
						if (GetState() == GREENKOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(GREENKOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == GREENKOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(GREENKOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
					if (brick->GetState() == BRICK_STATE_NORMAL && brick->Item < 10 && (isShell == true || isShell_2 == true))
						brick->SetState(BRICK_STATE_COLLISION);
				}
			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				if (e->ny < 0)
				{
					this->x += min_tx * dx + nx * 0.4f;
				}
				else if (e->nx != 0)
				{
					if (GetState() != GREENKOOPAS_STATE_SHELL || GetState() != GREENKOOPAS_STATE_SHELL_2)
						x += dx;
				}
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny < 0)
					this->x += min_tx * dx + nx * 0.4f;
				else
					this->y += min_ty * dy + ny * 0.4f;
			}
			else if (dynamic_cast<CGoomba*>(e->obj))
			{
				x += dx;
			}
			else if (dynamic_cast<GreenFlyKoopas*>(e->obj))
			{
				x += dx;
			}
			else if (dynamic_cast<FireBullet*>(e->obj))
			{
				FireBullet* fb = dynamic_cast<FireBullet*>(e->obj);
				if (fb->FireMario == false)
					x += dx;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void GreenFlyKoopas::Render()
{
	int ani = 0;
	if (Health == 2)
	{
		if (nx == LEFT)
			ani = GREENFLYKOOPAS_ANI_WALKING_LEFT;
		else
			ani = GREENFLYKOOPAS_ANI_WALKING_RIGHT;
	}
	else
	{
		if (isShell == true)
		{
			ani = GREENKOOPAS_ANI_SHELL;
			if (Revive == true)
				ani = GREENKOOPAS_ANI_SHELL_REVIVE;
		}
		else if (isShell_2 == true)
		{
			ani = GREENKOOPAS_ANI_SHELL_2;
		}
		if (vx > 0)
		{
			ani = GREENKOOPAS_ANI_WALKING_RIGHT;
			if (isShell == true)
			{
				ani = GREENKOOPAS_ANI_SHELL_WALKING_RIGHT;
			}
			else if (isShell_2 == true)
			{
				ani = GREENKOOPAS_ANI_SHELL_2_WALKING_RIGHT;
			}
		}
		else if (vx < 0)
		{
			ani = GREENKOOPAS_ANI_WALKING_LEFT;
			if (isShell == true)
			{
				ani = GREENKOOPAS_ANI_SHELL_WALKING_LEFT;
			}
			else if (isShell_2 == true)
			{
				ani = GREENKOOPAS_ANI_SHELL_2_WALKING_LEFT;
			}
		}
	}
	

	//DebugOut(L"state=%i, ani = %i, vx=%f,vy=%f, nx=%i, y = %f, type %i\n", state, ani, vx, vy, nx, y,ObjType);
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void GreenFlyKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREENKOOPAS_STATE_SHELL:
	{
		// nếu k phải mai rùa thì mới tính lại y, vì có trường hợp mai rùa di chuyển bị đạp xuống thì dừng lại
		if (isShell == false && isShell_2 == false)
			y = (INT16)(y + GREENKOOPAS_BBOX_HEIGHT - GREENKOOPAS_BBOX_HEIGHT_SHELL - 1);
		isShell = true;
		isShell_2 = false;
		vx = 0;
	}
	break;

	case GREENKOOPAS_STATE_SHELL_2:
	{
		if (isShell_2 == false && isShell == false)
		{
			y = (INT16)(y + GREENKOOPAS_BBOX_HEIGHT - GREENKOOPAS_BBOX_HEIGHT_SHELL - 1);
			vy = -0.2f;
		}
		isShell = false;
		isShell_2 = true;
		vx = 0;
	}
	break;

	case GREENKOOPAS_STATE_SHELL_HOLD:
	{
		isHold = true;
		y = (INT16)(y + GREENKOOPAS_BBOX_HEIGHT - GREENKOOPAS_BBOX_HEIGHT_SHELL - 1);
		vx = 0;
		vy = 0;
	}
	break;

	case GREENKOOPAS_STATE_DIE:
	{
		isShell = false;
		isShell_2 = true;
		y = (INT16)(y + GREENKOOPAS_BBOX_HEIGHT - GREENKOOPAS_BBOX_HEIGHT_SHELL - 1);
		vx = 0;
		vy = -0.2f;
	}
	break;

	case GREENKOOPAS_STATE_WALKING_RIGHT:
	{
		isShell = isShell_2 = false;
		vx = GREENKOOPAS_WALKING_SPEED;
		vy = 0;
		nx = 1;
	}break;

	case GREENKOOPAS_STATE_WALKING_LEFT:
	{
		isShell = isShell_2 = false;
		vx = -GREENKOOPAS_WALKING_SPEED;
		nx = -1;
	}break;


	case GREENKOOPAS_STATE_SHELL_WALKING_RIGHT:
	{
		vx = GREENKOOPAS_SHELL_SPEED;
		nx = 1;
		isHold = false;
	}break;

	case GREENKOOPAS_STATE_SHELL_WALKING_LEFT:
	{
		vx = -GREENKOOPAS_SHELL_SPEED;
		nx = -1;
		isHold = false;
	}break;
	}
}