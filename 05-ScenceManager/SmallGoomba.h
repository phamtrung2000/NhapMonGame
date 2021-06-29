#pragma once
#include "Enemy.h"

#define SMALLGOOMBA_ANI_FLYING	0
#define SMALLGOOMBA_ANI_DIE		1

#define SMALLGOOMBA_STATE_FOLLOWMARIO		9

class SmallGoomba : public Enemy
{
public:
	float Start_X;
	int TimeFollow; // tính toán thời gian bu vào mario để đi lên đi xuống
	//bool LeftToRight; // nấm bu lên người mario , đi từ trái sang phải
	int phase; // giai đoạn
	SmallGoomba(float a, float b);
	~SmallGoomba() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void Revival() {};
	virtual void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithPlayer(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);

};

