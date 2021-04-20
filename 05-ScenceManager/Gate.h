#pragma once
#include "GameObject.h"
class Gate : public CGameObject
{
public:
	int GateNumber;
	bool isComplete;
	int MarioCanGo[4]; // kiểm tra 4 hướng xem mario có thể đi hay không

	Gate(int a);
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + 16;
		bottom = top + 16;
	}
	virtual void Render();
};

