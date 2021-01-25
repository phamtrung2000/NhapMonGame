#include "Enemy.h"
#include "FireBullet.h"
#include "PlayScence.h"


Enemy::Enemy() : CGameObject()
{
	Category = CATEGORY::ENEMY;
	TypeEnemy = -1;
	canRevive = isAttacked = false;
	Score = 100;
	ReviveTime = Time_isAttacked = 0;
	Health = 1;
	OnGroud = false;
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT;
}

Enemy::~Enemy()
{
}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// đầu màn hình
	float cam_x = _Camera->cam_x;
	float camxx = _Game->GetCamX();
	float camy = _Game->GetCamY();
	// chiều dài màn hình
	int cam_w = CGame::GetInstance()->GetScreenWidth();
	int cam_h = CGame::GetInstance()->GetScreenHeight();

	//DebugOut(L"x = %f, cam_x = %i ,cam_w = %i\n", x, cam_x, cam_w);
	float a = camy + static_cast<float>(cam_h) - 50;
	float b = camy - 50;
	// ra khỏi camera -> delete
	if ((StartX > camxx + static_cast<float>(cam_w) + 150 || StartX + this->Width < camxx - 150) )
		//&& ( StartY > a || StartY + this->Height < b))
	{
		isDisappear = true;
		return;
	}
	else
	{
		if (isDisappear == true)
		{
			SetPosition(StartX, StartY);
			SetState(ENEMY_STATE_WALKING_LEFT);
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
}


void Enemy::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->ny < 0)
		OnGroud = true;
}

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
