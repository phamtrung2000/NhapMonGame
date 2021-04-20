#pragma once
#include "Enemy.h"
#include "BoomerangWeapon.h" 
#include "BoomerangWeapon.h"

#define BOOMERANGENEMY_STATE_THROWING_RIGHT	9
#define BOOMERANGENEMY_STATE_THROWING_LEFT	10

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

