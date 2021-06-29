#pragma once
#include "Goomba.h"

#define FLYGOOMBA_BBOX_WIDTH			20
#define FLYGOOMBA_WINGUP_BBOX_HEIGHT	24
#define FLYGOOMBA_WINGDOWN_BBOX_HEIGHT	19

#define FLYGOOMBA_FLY_SPEED			0.2f

#define FLYGOOMBA_ANI_WALKING		3
#define FLYGOOMBA_ANI_READYTOFLY	4
#define FLYGOOMBA_ANI_FLY			5

#define FLYGOOMBA_STATE_FLYING		9

#define FLYGOOMBA_JUMPING_LOW_SPEED		0.1f
#define FLYGOOMBA_JUMPING_HIGH_SPEED	0.15f
#define FLYGOOMBA_MOVINGTIME			1500
#define FLYGOOMBA_STARTTOFLYTIME		500
#define FLYGOOMBA_FLYINGTIME			4500
#define	FLYGOOMBA_FLYING_GRAVITY		0.0001f

class FlyGoomba : public Goomba
{
public:
	ULONGLONG TimeMoving; // thời gian đi bộ : đi bộ 1 lúc r mới nhảy
	ULONGLONG TimeFlying; // thời gian đi bộ : đi bộ 1 lúc r mới nhảy
	bool StartFlying;
	bool StopFlying;
	bool Up, Down;
	ULONGLONG DelayUpdatePosition; // thời gian đi bộ : đi bộ 1 lúc r mới nhảy

	int NumberSmallGoomba; // số lượng 
	int TotalNumberSmallGoomba; // tổng số lượng nấm nhỏ có thể thả : 6

	FlyGoomba();
	~FlyGoomba() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	void UpdatePosition();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);

};

