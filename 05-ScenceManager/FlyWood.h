#pragma once
#include "GameObject.h"


#define FLYWOOD_STATE_MOVE		0
#define FLYWOOD_STATE_FALL		1
#define FLYWOOD_GRAVITY			0.001f
#define FLYWOOD_SPEEDX		0.05f

#define FLYWOOD_WIDTH 48
#define FLYWOOD_HEIGHT 16

#define FLYWOOD_ANI_SET_ID 20

#define FLYWOOD_ANI 0

class FlyWood : public CGameObject
{
public:
	bool isCollision;
	FlyWood();
	FlyWood(float,float);
	~FlyWood();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void UpdatePosition(DWORD dt) {};
	void Render();
	void SetState(int state);
};