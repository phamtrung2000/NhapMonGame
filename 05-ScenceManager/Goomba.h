#pragma once
#include "Enemy.h"

//#define GOOMBA_DIE_DEFLECT_SPEED	0.1f//0.25f
//#define GOOMBA_WALKING_SPEED	0.02f
//#define	GOOMBA_GRAVITY			0.0001f//0.0007f

#define GOOMBA_BBOX_WIDTH		16
#define GOOMBA_BBOX_HEIGHT		15
#define GOOMBA_BBOX_HEIGHT_DIE	9

#define GOOMBA_ANI_WALKING	0
#define GOOMBA_ANI_DIE		1
#define GOOMBA_ANI_DIE_2	2

#define GOOMBA_SCORE	100
#define GOOMBA_TIMETODIE	20

extern class Goomba : public Enemy
{
public:
	Goomba();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void Revival() {};
	virtual void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) ;
	virtual void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithPlayer(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};