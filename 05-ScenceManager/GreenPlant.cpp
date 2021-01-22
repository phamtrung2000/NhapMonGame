#include "GreenPlant.h"
#include "PlayScence.h"
#include "WarpPipe.h"
#include "FirePiranhaPlant.h"

GreenPlant::GreenPlant() : FirePiranhaPlant()
{
	ObjType = OBJECT_TYPE_GREENPLANT;
	isBlocked = Stop = isInit = false;
	WarpPipeHeight = WarpPipeWidth = AppearTime = 0;
	SetState(GREENPLANT_STATE_HIDE);
}

GreenPlant::~GreenPlant()
{
}

// cây xuất hiện nhưng chưa chắc tấn công, phải nằm trong vùng tấn công
void GreenPlant::CalcAttackZone()
{
	if (abs(x - Mario_X) <= MAX_ATTACK_ZONE_X && abs(x - Mario_X) >= MIN_ATTACK_ZONE_X && x - Mario_X >= 0)
	{
		
		// cây đang trạng thái núp và số đạn = 1 thì mới xuất hiện
		// nếu không có điều kiện số đạn = 1 thì sẽ lặp vô tận 
		if (GetState() == GREENPLANT_STATE_HIDE && Stop == false)
		{
			SetState(GREENPLANT_STATE_APPEAR);
		}
		if (y - Startposy <= -WarpPipeHeight && Stop == false)
		{
			vy = 0;
			if (GetState() == GREENPLANT_STATE_APPEAR && Stop == false)
				Stop = true;
		}
	}
	else
	{
		if (GetState() == FIREPIRANHAPLANT_STATE_APPEAR && NumberBullet == 1)
		{
			if (y - Startposy <= -WarpPipeHeight)
			{
				vy = 0;
				canAttack = true;
			}
			if (NumberBullet == 0)
				SetState(FIREPIRANHAPLANT_STATE_HIDE);
		}
		else
			SetState(FIREPIRANHAPLANT_STATE_HIDE);
	}
}

void GreenPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	// cho cây mới vô rớt nhanh xuống, chạm đất để isInit = true
	if (isInit == false)
		vy += 0.005 * dt;
	else // cây đã được khởi tạo, bắt đầu tính toán vùng tấn công
		CalcAttackZone();
	// bắt đầu đếm thời gian xuất hiện
	if (Stop==true && AppearTime == 0)
	{
		AppearTime = GetTickCount64();
	}
	else
	{
		if (GetTickCount64() - AppearTime >= APPEAR_TIME) // hết thời gian xuất hiện
		{
			AppearTime = 0;
		}
		if (AppearTime == 0 && Stop == true)
		{
			SetState(GREENPLANT_STATE_HIDE);
		}
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
					Startposy = e->obj->y - GREENPLANT_BBOX_HEIGHT;
				}
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
	DebugOut(L"state = %i,vx = %f, vy %f, y %f, start y %f, distance %i\n", state, vx, vy, y, Startposy, INT(Startposy - y));
	DebugOut(L"height %f\n", WarpPipeHeight);*/
}

void GreenPlant::Render()
{
	int ani = -1;
	// cây trái , mario phải
	ani = GREENPLANT_ANI_APPEAR_HIDE;
	if (isInit == true)
		animation_set->at(ani)->Render(x, y);
	//DebugOut(L"5.Render	Ani: %i, State=%i\n", ani, state );
	//RenderBoundingBox();
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
