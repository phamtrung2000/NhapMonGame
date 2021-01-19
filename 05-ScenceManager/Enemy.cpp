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
}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
}