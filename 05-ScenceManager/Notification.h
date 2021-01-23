#pragma once
#include "GameObject.h"

class Notification : public CGameObject
{
public:
	int TypeCard;
	LPSPRITE Item1;
	bool Stop;
	Notification(float x, float y,int card);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};