#pragma once
#include "Enemy.h"
#include "BoomerangWeapon.h" 
#include "BoomerangWeapon.h"

#define BOOMERANGENEMY_STATE_DIE		0
#define BOOMERANGENEMY_STATE_DIE_2		1
#define BOOMERANGENEMY_STATE_WALKING_RIGHT	2
#define BOOMERANGENEMY_STATE_WALKING_LEFT	3
#define BOOMERANGENEMY_STATE_JUMPING_RIGHT	4
#define BOOMERANGENEMY_STATE_JUMPING_LEFT	5
#define BOOMERANGENEMY_STATE_THROWING_RIGHT	6
#define BOOMERANGENEMY_STATE_THROWING_LEFT	7

class BoomerangEnemy : public Enemy
{
	ULONGLONG TimeToMove;
	ULONGLONG TimeToJump;
	ULONGLONG TimeStartThrow; // thời điểm bắt đầu ném
	BoomerangWeapon* weapon;
	int TimeToThrow;
	bool CanThrow;
	bool isThrow;
	int countThrow;
public:
	BoomerangEnemy();
	BoomerangEnemy(float,float);
	~BoomerangEnemy() {}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CreateBoomerang();
};

