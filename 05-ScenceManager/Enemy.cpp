#include "Enemy.h"
#include "FireBullet.h"
#include "PlayScence.h"


Enemy::Enemy() : CGameObject()
{
	Category = CATEGORY::ENEMY;
	EnemyType = -1;
	canRevive = isAttacked = false;
	Score = ENEMY_SCORE;
	ReviveTime = Time_isAttacked = 0;
	Health = 1;
	OnGroud = false;
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT;
	IsMovingObject = true;
	SetState(ENEMY_STATE_INIT);
}

Enemy::~Enemy()
{

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

void Enemy::SetState(int state)
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
			vy = -ENEMY_DIE_DEFLECT_SPEED;
			vx = nx * abs(vx);
			isDie = true;
		}
		break;

		case ENEMY_STATE_INIT:
		{
			vy = ENEMY_INIT_GRAVITY;
		}
		break;

		case ENEMY_STATE_WALKING_RIGHT:
		{
			nx = RIGHT;
			vx = nx * ENEMY_WALKING_SPEED;
		}
		break;

		case ENEMY_STATE_WALKING_LEFT:
		{
			nx = LEFT;
			vx = nx * ENEMY_WALKING_SPEED;
		}
		break;
	}
}


void Enemy::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	x += dx;
	if (OnGroud == true)
		this->y += min_ty * dy + ny * 0.2f;
	else
		y += dy;
}

void Enemy::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj != NULL)
	{
		if (e->ny < 0)
		{
			OnGroud = true;
			if (GetState() == ENEMY_STATE_INIT)
				SetState(ENEMY_STATE_WALKING_LEFT);
		}
			
		if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
		{
			if (ny != 0) vy = 0;
			if (e->nx != 0)
			{
				x += dx;
			}
			else if (e->ny < 0)
			{
				x += min_tx * dx + nx * 0.4f;
			}
			else if (e->ny > 0)
			{
				y += dy;
			}
		}
		else
		{
			if (e->nx != 0)
			{

				if (GetState() == ENEMY_STATE_WALKING_RIGHT)
					SetState(ENEMY_STATE_WALKING_LEFT);
				else if (GetState() == ENEMY_STATE_WALKING_LEFT)
					SetState(ENEMY_STATE_WALKING_RIGHT);
				y += min_ty * dy + ny * 0.2f;
			}
			else
			{
				x += min_tx * dx + nx * 0.4f;
			}
			if (ny != 0) vy = 0;
		}
	}
}

void Enemy::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	x += dx;
	if (e->ny < 0)
		y += dy;
	else
		this->y += min_ty * dy + ny * 0.2f;
	if (ny != 0) vy = 0;
}

void Enemy::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj->ObjType == OBJECT_TYPE_FIREBULLET)
	{
		FireBullet* firebullet = dynamic_cast<FireBullet*>(e->obj);
		if (firebullet->FireMario == false)
		{
			x += dx;
			this->y += min_ty * dy + ny * 0.1f - 0.5f;
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

void Enemy::CollisionWithPlayer(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Mario* mario = dynamic_cast<Mario*>(e->obj);
	if (e->nx != 0)
	{
		if (mario->untouchable == true)
		{
			x += dx;
		}
		else
		{
			mario->DownLevel();
		}
		if (ny != 0) vy = 0;
	}
}
