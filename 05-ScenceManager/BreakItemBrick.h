#pragma once
#include "GameObject.h"

class BreakItemBrick : public CGameObject
{
public:
	BreakItemBrick(int type,float x, float y, float vx, float vy);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};