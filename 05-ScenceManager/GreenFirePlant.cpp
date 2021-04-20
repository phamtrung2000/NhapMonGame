#include "GreenFirePlant.h"
#include "FireBullet.h"
#include "PlayScence.h"
#include "WarpPipe.h"

GreenFirePlant::GreenFirePlant() :FirePiranhaPlant()
{
	ObjType = OBJECT_TYPE_GREENFIREPLANT;
}

// cây xuất hiện nhưng chưa chắc tấn công, phải nằm trong vùng tấn công
void GreenFirePlant::CalcAttackZone()
{
	//	DebugOut(L"x - Mario_X = %f\n", abs(x - Mario_X));
	// mario đi tới vùng tấn công
	//if (x - Mario_X <= MAX_ATTACK_ZONE_X && x - Mario_X >=0)
	if (abs(x - Mario_X) <= MAX_ATTACK_ZONE_X && abs(x - Mario_X) >= MIN_ATTACK_ZONE_X && x - Mario_X >= 0)

	{
		// cây đang trạng thái núp và số đạn = 1 thì mới xuất hiện
		// nếu không có điều kiện số đạn = 1 thì sẽ lặp vô tận 
		if (GetState() == GREENFIREPLANT_STATE_HIDE && NumberBullet == 1)
		{
			SetState(GREENFIREPLANT_STATE_APPEAR);
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

void GreenFirePlant::CalcAtkPos()
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
	VxBullet = -0.05f;
	nx = -1;
}

void GreenFirePlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	// cho cây mới vô rớt nhanh xuống, chạm đất để isInit = true
	if (isInit == false)
		vy += 0.001f * dt;
	else // cây đã được khởi tạo, bắt đầu tính toán vùng tấn công
		CalcAttackZone();


	if (canAttack && CalcAtkTime == 0)
	{
		CalcAtkTime = GetTickCount64();
	}
	if (canAttack && GetTickCount64() - CalcAtkTime >= 500)
	{
		CalcAtkPos();
		SetState(GREENFIREPLANT_STATE_ATTACK);
		canAttack = isAttacking = false;
		CalcAtkTime = 0;
	}

	if (state == FIREPIRANHAPLANT_STATE_ATTACK)
	{
		if (this->NumberBullet == 1)
		{
			FireBullet* fb = new FireBullet(this->x, this->y);
			fb->SetSpeed(this->VxBullet, this->VyBullet);
			// chiều của viên đạn
			if (this->nx == 1)
				fb->nx = 1;
			else
				fb->nx = -1;

			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
			fb->SetAnimationSet(ani_set);
			_Grid->AddMovingObject(fb, this->x, this->y);
			// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
			this->NumberBullet--;
		}
	}

	// bắn ra đạn thì cây núp xuống
	if (NumberBullet == 0)
		SetState(GREENFIREPLANT_STATE_HIDE);

	this->GetEnemyPos(_Mario->x, _Mario->y);
	if (abs(_Mario->x - this->x) <= 20 && this->Stop == true)
		this->SetState(FIREPIRANHAPLANT_STATE_STOP);
	else if (this->Stop == true)
	{
		this->SetState(FIREPIRANHAPLANT_STATE_APPEAR);
		this->Stop = false;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != GREENFIREPLANT_STATE_DIE)
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
					Startposy = e->obj->y - GREENFIREPLANT_BBOX_HEIGHT;
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

void GreenFirePlant::Render()
{
	int ani = -1;
	// cây trái , mario phải
	if (Mario_Y < y)
		ani = GREENFIREPLANT_ANI_APPEAR_HIDE_UP_LEFT;
	else
		ani = GREENFIREPLANT_ANI_APPEAR_HIDE_DOWN_LEFT;
	//if (isInit == true)
		animation_set->at(ani)->Render(x, y);
}

void GreenFirePlant::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GREENFIREPLANT_STATE_APPEAR:
		vy = -GREENFIREPLANT_SPEED_Y;
		break;
	case GREENFIREPLANT_STATE_HIDE:
		vy = GREENFIREPLANT_SPEED_Y;
		break;
	case GREENFIREPLANT_STATE_ATTACK:
		isAttacking = true;
		break;
	case GREENFIREPLANT_STATE_DIE:
		isDie = canDelete = true;
		break;
	case GREENFIREPLANT_STATE_STOP:
		vy = 0;
		break;
	}
}
