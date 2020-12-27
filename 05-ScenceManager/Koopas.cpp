#include "Koopas.h"
#include "ItemBrick.h"
#include "FireBullet.h"
#include "PlayScence.h"
#include "Coin.h"

Koopas::Koopas()
{
	SetState(KOOPAS_STATE_WALKING_RIGHT);
	X_max = MAX;
	X_min = MIN;
	CountXmaxXmin = false;
	isHold = isShell = isShell_2 =false;
	ObjType = OBJECT_TYPE_KOOPAS;
	GoAround = Revive = false;
	Category = CATEGORY::ENEMY;
}

void Koopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	if (isShell == true || isShell_2 == true)
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
	if (isHold == true)
	{
		left = left - 1.f;
		right = right - 1.f;
	}
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//DebugOut(L"state=%i, vx=%f,vy=%f, nx=%i, y = %f\n", state, vx, vy, nx, y);
	//DebugOut(L"x max %f, x min %f, x %f, state %i\n", X_max, X_min, x, state);
	if (isShell == false && isShell_2 == false)
	{
		vy += KOOPAS_GRAVITY * dt;
	}
	else if (isShell == true && isHold != true)
	{
		vy += KOOPAS_SHELL_GRAVITY * dt;
	}
	// vẫy đuôi -> rùa lật ngửa rớt từ trên xuống
	else if (isShell_2 == true && isHold != true)
	{
		vy += KOOPAS_SHELL_2_GRAVITY * dt;
	}
	if (state == GOOMBA_STATE_DIE_2)
	{
		//vy += GOOMBA_GRAVITY * dt;
		if (y > 450)
			isDie = true;
	}

	if (GetState() == KOOPAS_STATE_WALKING_RIGHT || GetState() == KOOPAS_STATE_WALKING_LEFT)
	{
		if (x >= X_max && vx > 0)
		{
			SetState(KOOPAS_STATE_WALKING_LEFT);
		}
		else if (x <= X_min && vx < 0)
		{
			SetState(KOOPAS_STATE_WALKING_RIGHT);
		}

	}

	if ((GetState() == KOOPAS_STATE_SHELL || GetState() == KOOPAS_STATE_SHELL_2 || GetState() == KOOPAS_STATE_SHELL_HOLD)
		&& GetTickCount64() - ReviveTime > KOOPAS_START_REVIVE_TIME)
	{
		Revive = true;
		if (GetTickCount64() - ReviveTime > KOOPAS_REVIVE_TIME)
		{
			ReviveTime = 0;
			Revive = false;
			y = (INT16)(y - (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) - 1);
			SetState(KOOPAS_STATE_WALKING_RIGHT);
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != KOOPAS_STATE_DIE)
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

			if (dynamic_cast<WarpPipe*>(e->obj))
			{
				if (e->nx != 0)
				{
					if (isShell == false && isShell_2 == false)
					{
						if (GetState() == KOOPAS_STATE_WALKING_RIGHT)
							SetState(KOOPAS_STATE_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_WALKING_LEFT)
							SetState(KOOPAS_STATE_WALKING_RIGHT);
					}
					else
					{
						if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
					//this->y += min_ty * dy + ny * 0.4f;
				}
			}
			else if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
				if (e->nx != 0)
				{
					if (isShell == false && isShell_2 == false)
					{
						if (GetState() == KOOPAS_STATE_WALKING_RIGHT)
							SetState(KOOPAS_STATE_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_WALKING_LEFT)
							SetState(KOOPAS_STATE_WALKING_RIGHT);
					}
					else
					{
						if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
					if (brick->GetState() == BRICK_STATE_NORMAL && brick->Item < 10 && (isShell == true || isShell_2 == true))
						brick->SetState(BRICK_STATE_COLLISION);
				}
				else if (e->ny < 0)
				{
					this->x += min_tx * dx + nx * 0.4f;
				}
			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				if (e->ny < 0 && CountXmaxXmin == false)
				{
					Block* block = dynamic_cast<Block*>(e->obj);
					X_min = block->x;
					X_max = X_min + (block->Width - 16);
					CountXmaxXmin = true;
				}
				if (GetState() != KOOPAS_STATE_SHELL || GetState() != KOOPAS_STATE_SHELL_2)
				{
					x += dx;
				}
			}
			else if (dynamic_cast<ItemBrick*>(e->obj))
			{
				ItemBrick* brick = dynamic_cast<ItemBrick*>(e->obj);
				if (e->ny < 0)
				{
					this->x += min_tx * dx + nx * 0.4f;
					if (CountXmaxXmin == false)
					{
						X_min = brick->x - ITEMBRICK_WIDTH / 2 - 2;
						X_max = X_min + ITEMBRICK_WIDTH - 2;
						CountXmaxXmin = true;
					}
				}
				else if (e->nx != 0)
				{
					if (isShell == false && isShell_2 == false)
					{
						if (GetState() == KOOPAS_STATE_WALKING_RIGHT)
							SetState(KOOPAS_STATE_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_WALKING_LEFT)
							SetState(KOOPAS_STATE_WALKING_RIGHT);
					}
					else
					{
						brick->isDie = true;
						if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
				}
				
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				X_min = MIN;
				X_max = MAX;
				Ground* ground = dynamic_cast<Ground*>(e->obj);
				if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
				}
				else if (e->nx != 0)
				{
					y += min_ty * dy + ny * 0.4f;
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj))
			{
				x += dx;
				//this->y += min_ty * dy + ny * 0.4f;
			}
			else if (dynamic_cast<Koopas*>(e->obj))
			{
				x += dx;
				
			}
			else if (dynamic_cast<FireBullet*>(e->obj))
			{
				FireBullet* fb = dynamic_cast<FireBullet*>(e->obj);
				if (fb->FireMario == false)
				{
					x += dx;
				}
			}
			else if (dynamic_cast<Coin*>(e->obj))
			{
				if (e->nx != 0)
				{
					x += dx;
				}
				else if (e->ny < 0)
				{
					y += dy;
				}
				
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Koopas::Render()
{
	int ani = 0;

	if (isShell == true)
	{
		ani = KOOPAS_ANI_SHELL;
		if (Revive == true)
			ani = KOOPAS_ANI_SHELL_REVIVE;
	}
	else if (isShell_2 == true)
	{
		ani = KOOPAS_ANI_SHELL_2;
	}
	if (vx > 0)
	{
		ani = KOOPAS_ANI_WALKING_RIGHT;
		if (isShell == true)
		{
			ani = KOOPAS_ANI_SHELL_WALKING_RIGHT;
		}
		else if (isShell_2 == true)
		{
			ani = KOOPAS_ANI_SHELL_2_WALKING_RIGHT;
		}
	}
	else if (vx < 0)
	{
		ani = KOOPAS_ANI_WALKING_LEFT;
		if (isShell == true)
		{
			ani = KOOPAS_ANI_SHELL_WALKING_LEFT;
		}
		else if (isShell_2 == true)
		{
			ani = KOOPAS_ANI_SHELL_2_WALKING_LEFT;
		}
	}
	
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Koopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
	{
		// nếu k phải mai rùa thì mới tính lại y, vì có trường hợp mai rùa di chuyển bị đạp xuống thì dừng lại
		if (isShell == false && isShell_2 == false)
			y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
		isShell = true;
		isShell_2 = false;
		vx = 0;
	}
	break;

	case KOOPAS_STATE_SHELL_2:
	{
		if (isShell_2 == false && isShell == false)
		{
			y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
			vy = -0.2f;
		}
		isShell = false;
		isShell_2 = true;
		vx = 0;
	}
	break;

	case KOOPAS_STATE_SHELL_HOLD:
	{
		isHold =  true;
		y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
		vx = 0;
		vy = 0;
	}
	break;

	case KOOPAS_STATE_DIE:
	{
		isShell = false;
		isShell_2 = true;
		y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
		vx = 0;
		vy = -0.2f;
	}
	break;

	case KOOPAS_STATE_WALKING_RIGHT:
	{
		isShell = isShell_2 = false;
		vx = KOOPAS_WALKING_SPEED;
		vy = 0;
		nx = 1;
	}break;

	case KOOPAS_STATE_WALKING_LEFT:
	{
		isShell = isShell_2 = false;
		vx = -KOOPAS_WALKING_SPEED;
		nx = -1;
	}break;


	case KOOPAS_STATE_SHELL_WALKING_RIGHT:
	{
		vx = KOOPAS_SHELL_SPEED;
		nx = 1;
		isHold = false;
	}break;

	case KOOPAS_STATE_SHELL_WALKING_LEFT:
	{
		vx = -KOOPAS_SHELL_SPEED;
		nx = -1;
		isHold = false;
	}break;

	}

}

// x start: 512
// x max: 592
// width : 96

