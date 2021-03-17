#pragma once
#include "Weapon.h"

#define ANI_BOOMERANG_THROW_RIGHT	0
#define	ANI_BOOMERANG_THROW_LEFT	1
#define ANI_BOOMERANG_HOLD_RIGHT	2
#define	ANI_BOOMERANG_HOLD_LEFT	3

class BoomerangWeapon : public Weapon
{
public:
	int Time;
	BoomerangWeapon(int nx, float x, float y);
	~BoomerangWeapon();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
};