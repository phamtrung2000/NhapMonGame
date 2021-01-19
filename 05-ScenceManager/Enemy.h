#pragma once
#include "GameObject.h"

#define ENEMYTYPE_GOOMBA 0
#define ENEMYTYPE_KOOPAS 1
#define ENEMYTYPE_PLANT	 2

#define ENEMY_STATE_DIE 0
#define ENEMY_STATE_DIE_2 1
#define ENEMY_STATE_WALKING_RIGHT 2
#define ENEMY_STATE_WALKING_LEFT 3

#define ENEMY_TIME_ISATTACKED 100

class Enemy : public CGameObject
{
public:
	bool canRevive;
	int TypeEnemy;
	int Score;
	int Health;
	ULONGLONG ReviveTime;
	bool isAttacked; // để xử lý vụ quẩy đuôi hiện quá nhiều effect hit
	ULONGLONG Time_isAttacked;
	Enemy();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void SetState(int state) {};
	virtual void Revival() {};
	virtual void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};

