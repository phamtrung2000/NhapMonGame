#pragma once
#include "GameObject.h"

#define ITEM_TYPE_QUESTIONBRICKITEM 0
#define ITEM_TYPE_BRICKITEM			1
#define ITEM_TYPE_COIN				2

#define ITEM_STATE_INIT	0
#define	ITEM_STATE_MOVE_RIGHT	1
#define	ITEM_STATE_MOVE_LEFT	2
#define ITEM_STATE_COLLISION	3

#define ENEMY_TIME_ISATTACKED 100

class Item : public CGameObject
{
public:
	float Start_X;
	float Start_Y;
	int TypeItem, Score;
	bool OnGround;
	Item();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void SetState(int state) {};
	virtual void Revival() {};
	virtual void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};

