#pragma once
#include "Goomba.h"

#define REDGOOMBA_BBOX_WIDTH			20
#define REDGOOMBA_WINGUP_BBOX_HEIGHT	24
#define REDGOOMBA_WINGDOWN_BBOX_HEIGHT	19

#define REDGOOMBA_JUMPING_LOW_SPEED		0.1f
#define REDGOOMBA_JUMPING_HIGH_SPEED	0.2f

#define REDGOOMBA_ANI_WALKING		3
#define REDGOOMBA_ANI_JUMPING_LOW	4
#define REDGOOMBA_ANI_JUMPING_HIGH	5

#define REDGOOMBA_MOVINGTIME	1500

class RedGoomba : public Goomba
{
public:
	int JumpCount; // đếm đủ 3 lần nhảy thấp thì sẽ nhảy cao
	ULONGLONG TimeMoving; // thời gian đi bộ : đi bộ 1 lúc r mới nhảy
	RedGoomba();
	~RedGoomba() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	void UpdatePosition(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};

