#pragma once
#include "GameObject.h"
#include "Utils.h"

#define	FIREBULLET_GRAVITY		0.0007f

#define ANI_BULLET_RIGHT	0
#define	ANI_BULLET_LEFT		1

class FireBullet :	public CGameObject
{
public:
	bool FireMario;
	int nx;
	FireBullet(float a, float b)
	{
		nx = 1;
		FireMario = isDie = false;
		ObjType = 100;
		x = a;
		y = b;
		Category = CATEGORY::WEAPON;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state) {}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = x + 8;
		bottom = y + 8;
	}
};

