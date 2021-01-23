#pragma once
#include "GameObject.h"
class Block : public CGameObject
{
public:
	Block(int W, int H);
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};

