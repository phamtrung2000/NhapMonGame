#pragma once
#include "GameObject.h"

class Weapon : public CGameObject
{
public:
	bool isMarioWeapon;
	Weapon();
	~Weapon() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	
	virtual void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};

};