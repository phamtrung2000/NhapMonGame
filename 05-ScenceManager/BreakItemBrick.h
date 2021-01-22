#pragma once
#include "GameObject.h"

#define BREAKITEMBRICK_BBOX_WIDTH  8
#define BREAKITEMBRICK_BBOX_HEIGHT 8

#define BREAKITEMBRICK_LEFT_ANI 0
#define BREAKITEMBRICK_RIGHT_ANI 1
#define BREAKITEMBREAKITEMBRICK_ANI_SET_ID 22

class BreakItemBrick : public CGameObject
{
public:
	BreakItemBrick(int type,float x, float y, float vx, float vy);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};