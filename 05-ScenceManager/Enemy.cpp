#include "Enemy.h"
#include "FireBullet.h"
#include "PlayScence.h"

void Enemy::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	x += dx;
	if (e->ny < 0)
		y += dy;
	else
		this->y += min_ty * dy + ny * 0.1f - 0.5f;
}

void Enemy::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj->ObjType == OBJECT_TYPE_FIREBULLET)
	{
		if (dynamic_cast<FireBullet*>(e->obj))
		{
			FireBullet* firebullet = dynamic_cast<FireBullet*>(e->obj);
			if (firebullet->FireMario == false)
			{
				x += dx;
				this->y += min_ty * dy + ny * 0.1f - 0.5f;
			}
			
		}
	}
	else
	{
		x += dx;
		if (e->ny < 0)
			y += dy;
		else
			this->y += min_ty * dy + ny * 0.1f - 0.5f;
	}
}

Enemy::Enemy() : CGameObject()
{
	Category = CATEGORY::ENEMY;
	TypeEnemy = -1;
	canRevive = isAttacked = false;
	Score = 0;
	ReviveTime = Time_isAttacked = 0;
	Health = 1;
	StartX = StartY = 0;
}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// đầu màn hình
	float cam_x = _Camera->cam_x;
	// chiều dài màn hình
	int cam_w = CGame::GetInstance()->GetScreenWidth();

	DebugOut(L"x = %f, cam_x = %i ,cam_w = %i\n", x, cam_x, cam_w);
	// ra khỏi camera -> delete
	if (x > cam_x + static_cast<float>(cam_w) || x < cam_x || y > _Map->GetHeight())
	{
		isDisappear = true;
		return;
	}
	else
	{
		if (isDisappear == true)
		{
			SetPosition(StartX, StartY);
			isDisappear = false;
		}
	}
	if (isAttacked == true)
	{
		if (Time_isAttacked != 0 && GetTickCount64() - Time_isAttacked > ENEMY_TIME_ISATTACKED)
		{
			isAttacked = false;
			Time_isAttacked = 0;
		}
	}
	if (isDisappear == true)
		DebugOut(L"isDisappear == true\n");
	else
		DebugOut(L"isDisappear == false\n");
}