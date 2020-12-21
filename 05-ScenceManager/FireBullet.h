#pragma once
#include "GameObject.h"


#define	FIREBULLET_GRAVITY		0.0007f
#define	FIREBULLET_VX_SPEED		0.25f

#define ANI_BULLET_RIGHT	0
#define	ANI_BULLET_LEFT		1
#define	FIREBULLET_BBOX		8

class FireBullet :	public CGameObject
{
public:
	bool FireMario;
	FireBullet(float a, float b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

