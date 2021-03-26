#pragma once
#include "GameObject.h"

#define ENEMY_TYPE_GOOMBA 0
#define ENEMY_TYPE_KOOPAS 1
#define ENEMY_TYPE_PLANT	 2
#define ENEMY_TYPE_BOOMERANG_ENEMY	 3

#define ENEMY_STATE_DIE_IS_JUMPED	0
#define ENEMY_STATE_DIE_IS_ATTACKED 1
#define ENEMY_STATE_INIT			2
#define ENEMY_STATE_WALKING_RIGHT	3
#define ENEMY_STATE_WALKING_LEFT	4
#define ENEMY_STATE_JUMPING_HIGH_RIGHT	5
#define ENEMY_STATE_JUMPING_HIGH_LEFT	6
#define ENEMY_STATE_JUMPING_LOW_RIGHT	7
#define ENEMY_STATE_JUMPING_LOW_LEFT	8

#define ENEMY_TIME_ISATTACKED 100
#define ENEMY_SCORE	100

#define ENEMY_DIE_DEFLECT_SPEED	0.25f
#define ENEMY_WALKING_SPEED		0.05f
#define	ENEMY_GRAVITY			0.0007f

class Enemy : public CGameObject
{
public:
	bool canRevive;
	int EnemyType;
	int Score;
	int Health;
	ULONGLONG ReviveTime;
	bool isAttacked; // để xử lý vụ quẩy đuôi hiện quá nhiều effect hit
	ULONGLONG Time_isAttacked;
	bool OnGroud;
	Enemy();
	~Enemy();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void SetState(int state) ;
	virtual void Revival() {};
	virtual void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) ;
	virtual void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithPlayer(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};

