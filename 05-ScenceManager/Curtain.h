#pragma once
#include "GameObject.h"

#define CURTAIN_ANIMATION	0

class Curtain : public CGameObject
{
public:
	bool IsUp, IsDown, StopRender, IsAppear, choose;
	ULONGLONG AppearTime;
	//Curtain* ct;
	int Type;
	
	Curtain();
	Curtain(float X, float Y);
	Curtain(float X, float Y,int type);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	};
	virtual void SetState(int state);
};

