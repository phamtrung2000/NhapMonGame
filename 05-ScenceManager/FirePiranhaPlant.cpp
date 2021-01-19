#include "FirePiranhaPlant.h"
#include "PlayScence.h"
#include "WarpPipe.h"

FirePiranhaPlant::FirePiranhaPlant() : Enemy()
{
	ObjType = OBJECT_TYPE_FIREPIRANHAPLANT;
	Stop = isInit = canAttack = isAttacking = false;
	NumberBullet = 1;
	CalcAtkTime = 0;
	SetState(FIREPIRANHAPLANT_STATE_HIDE);
	Score = FIREPIRANHAPLANT_SCORE;
	TypeEnemy = ENEMYTYPE_PLANT;
}

// cây xuất hiện nhưng chưa chắc tấn công, phải nằm trong vùng tấn công
void FirePiranhaPlant::CalcAttackZone()
{
	float a = abs(x - Mario_X);
	// mario đi tới vùng tấn công
	if (abs(x - Mario_X) <= MAX_ATTACK_ZONE_X && abs(x - Mario_X) >= MIN_ATTACK_ZONE_X)
	{
		// cây đang trạng thái núp và số đạn = 1 thì mới xuất hiện
		// nếu không có điều kiện số đạn = 1 thì sẽ lặp vô tận 
		if (GetState() == FIREPIRANHAPLANT_STATE_HIDE  && NumberBullet == 1)
		{
			SetState(FIREPIRANHAPLANT_STATE_APPEAR);
		}
		if (y - Startposy <= -WarpPipeHeight)
		{
			vy = 0;
			canAttack = true;
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

void FirePiranhaPlant::CalcAtkPos()
{
	// mario và cây đứng gần như ngang nhau
	if (abs(y - Mario_Y) <= 20)
	{
		// mario trên, cây dưới -> đạn bay lên trên
		if (Mario_Y < y)
			VyBullet = -0.01f;
		// mario dưới, cây trên -> đạn bay xuống
		else
			VyBullet = 0.01f;
	}
	// mario và cây cách xa 1 đoạn
	else
	{
		if (Mario_Y < y)
			VyBullet = -0.03f;
		else
			VyBullet = 0.05f;

	}
	if (Mario_X > x)
	{
		VxBullet = 0.05f;
		nx = 1;
	}
	else
	{
		VxBullet = -0.05f;
		nx = -1;
	}
}

void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	// cho cây mới vô rớt nhanh xuống, chạm đất để isInit = true
	if (isInit == false)
		vy += 0.005 * dt;
	else // cây đã được khởi tạo, bắt đầu tính toán vùng tấn công
		CalcAttackZone();
	

	if (canAttack && CalcAtkTime == 0)
	{
		CalcAtkTime = GetTickCount64();
	}
	if (canAttack && GetTickCount64() - CalcAtkTime >= 500)
	{
		CalcAtkPos();
		SetState(FIREPIRANHAPLANT_STATE_ATTACK);
		canAttack = isAttacking = false;
		CalcAtkTime = 0;
	}
	// bắn ra đạn thì cây núp xuống
	if (NumberBullet == 0)
		SetState(FIREPIRANHAPLANT_STATE_HIDE);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != FIREPIRANHAPLANT_STATE_DIE)
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
				}
				y += dy;
				
			}
			else if (e->obj->ObjType == OBJECT_TYPE_GROUND)
			{
				if (isInit == false)
				{
					isInit = true;
					Startposy = e->obj->y - FIREPIRANHAPLANT_BBOX_HEIGHT;
				}
				NumberBullet = 1;
			}
			//DebugOut(L"type = %i\n", e->obj->ObjType);
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	/*if (Stop == true)
		DebugOut(L"Stop = true\t");
	else
		DebugOut(L"Stop = false\t");
	DebugOut(L"state = %i,vx = %f, vy %f, y %f, start y %f\n", state, vx, vy, y, Startposy);*/
	
}

void FirePiranhaPlant::Render()
{
	int ani = -1;
	// cây trái , mario phải
	if (Mario_X > x)
	{
		nx = 1;
		// mario ở phía trên
		if (Mario_Y < y)
			ani = FIREPIRANHAPLANT_ANI_APPEAR_HIDE_UP_RIGHT;
		// mario ở phía dưới
		else
			ani = FIREPIRANHAPLANT_ANI_APPEAR_HIDE_DOWN_RIGHT;
	}
	// mario trái, cây phải  
	else
	{
		nx = -1;
		if (Mario_Y < y)
			ani = FIREPIRANHAPLANT_ANI_APPEAR_HIDE_UP_LEFT;
		else
			ani = FIREPIRANHAPLANT_ANI_APPEAR_HIDE_DOWN_LEFT;
	}
	if (isInit == true)
		animation_set->at(ani)->Render(x, y);
	//DebugOut(L"5.Render	Ani: %i, State=%i\n", ani, state );
	RenderBoundingBox();
}

void FirePiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FIREPIRANHAPLANT_STATE_APPEAR:
		vy = -FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRANHAPLANT_STATE_HIDE:
		vy = FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRANHAPLANT_STATE_ATTACK:
		isAttacking = true;
		break;
	case FIREPIRANHAPLANT_STATE_DIE:
		isDie = canDelete = true;
		break;
	case FIREPIRANHAPLANT_STATE_STOP:
		vy = 0;
		break;
	}
}
