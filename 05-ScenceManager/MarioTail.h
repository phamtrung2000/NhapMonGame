#pragma once
#include "GameObject.h"

#define MARIO_TAIL_WIDTH  8.0f
#define MARIO_TAIL_HEIGHT 2.0f

class MarioTail:public CGameObject //mỗi khi mario quật đuôi thì tạo object đuôi thay 
{
public:
	MarioTail(float, float);
	~MarioTail() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state) {}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = x + MARIO_TAIL_WIDTH;
		bottom = y + MARIO_TAIL_HEIGHT;
	}
};

