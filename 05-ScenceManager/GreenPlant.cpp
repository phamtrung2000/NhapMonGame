#include "GreenPlant.h"
#include "PlayScence.h"
#include "WarpPipe.h"
#include "FirePiranhaPlant.h"

GreenPlant::GreenPlant() : Enemy()
{
	ObjType = OBJECT_TYPE_GREENPLANT;
	isBlocked = Stop = isInit = false;
	WarpPipeHeight = WarpPipeWidth = 0;
	AppearTime = 0;
	SetState(GREENPLANT_STATE_HIDE);

	Score = FIREPIRANHAPLANT_SCORE;
	EnemyType = ENEMY_TYPE_PLANT;
}

GreenPlant::~GreenPlant()
{
}

// cây xuất hiện nhưng chưa chắc tấn công, phải nằm trong vùng tấn công
void GreenPlant::CalcAttackZone()
{
	if (abs(x - Mario_X) <= MAX_ATTACK_ZONE_X && abs(x - Mario_X) >= MIN_ATTACK_ZONE_X)
	{
		ChangeStateHideAppear();
	}
	else
	{
		// TH cây đang trồi lên mà Mario nhảy tới thì cây k bị thụt xuống liền
		if (GetState() == FIREPIRANHAPLANT_STATE_APPEAR)
		{
			if (y - StartY <= -WarpPipeHeight && Stop == false)
			{
				vy = 0;
				Stop = true;
				AppearTime = GetTickCount64();
			}

			if (GetTickCount64() - AppearTime >= APPEAR_TIME && AppearTime != 0) // hết thời gian xuất hiện
			{
				SetState(GREENPLANT_STATE_HIDE);
				Stop = false;
			}
		}
		else
			SetState(FIREPIRANHAPLANT_STATE_HIDE);
	}
}

void GreenPlant::ChangeStateHideAppear()
{
	if (GetState() == GREENPLANT_STATE_HIDE )
	{
		// appear time sẽ reset = 0 khi chạm đất , có đk appeartime ở đây để tránh appear mãi mãi
		if (Stop == false && AppearTime == 0)
		{
			SetState(GREENPLANT_STATE_APPEAR);
		}
	}
	else
	{
		if (y - StartY <= -WarpPipeHeight && Stop == false)
		{
			vy = 0;
			Stop = true;
			AppearTime = GetTickCount64();
		}

		if (GetTickCount64() - AppearTime >= APPEAR_TIME && AppearTime != 0) // hết thời gian xuất hiện
		{
			SetState(GREENPLANT_STATE_HIDE);
			Stop = false;
		}
	}
	
}

void GreenPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	// cho cây mới vô rớt nhanh xuống, chạm đất để isInit = true
	if (isInit == false)
		vy += 0.001f * dt;
	else // cây đã được khởi tạo, bắt đầu tính toán vùng tấn công
		CalcAttackZone();

	this->GetEnemyPos(_Mario->x, _Mario->y);
	if (abs(_Mario->x - this->x) <= 20 && this->isBlocked == true)
		this->SetState(GREENPLANT_STATE_STOP);
	else if (this->isBlocked == true)
	{
		this->SetState(GREENPLANT_STATE_APPEAR);
		this->isBlocked = false;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != GREENPLANT_STATE_DIE)
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		if (nx != 0) vx = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->ObjType == OBJECT_TYPE_WARPPIPE)
			{
				WarpPipe* wp = dynamic_cast<WarpPipe*>(e->obj);
				if (isInit == false)
				{
					WarpPipeHeight = wp->Height;
					WarpPipeWidth = wp->Width;
				}
				y += dy;
			}
			else if (e->obj->ObjType == OBJECT_TYPE_GROUND)
			{
				if (isInit == false)
				{
					isInit = true;
					StartY = e->obj->y - GREENPLANT_BBOX_HEIGHT;
				}
				AppearTime = 0;
				Stop = false;
			}
			//DebugOut(L"type = %i\n", e->obj->ObjType);
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	/*if (isBlocked == true)
		DebugOut(L"isBlocked = true\n");
	else
		DebugOut(L"isBlocked = false\n");
	DebugOut(L"state = %i,vx = %f, vy %f, y %f, start y %f, distance %i\n", state, vx, vy, y, StartY, INT(StartY - y));
	DebugOut(L"height %f\n", WarpPipeHeight);*/
}

void GreenPlant::Render()
{
	int ani = -1;
	// cây trái , mario phải
	ani = GREENPLANT_ANI_APPEAR_HIDE;
	//if (isInit == true)
		animation_set->at(ani)->Render(x, y);
}

void GreenPlant::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GREENPLANT_STATE_APPEAR:
		vy = -GREENPLANT_SPEED_Y;
		break;
	case GREENPLANT_STATE_HIDE:
		vy = GREENPLANT_SPEED_Y;
		break;
	case GREENPLANT_STATE_DIE:
		isDie = canDelete = true;
		break;
	case GREENPLANT_STATE_STOP:
		vy = 0;
		break;
	}
}
