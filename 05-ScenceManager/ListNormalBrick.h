#pragma once
#include "Brick.h"

class ListNormalBrick : public Brick
{
public:
	int NumberBrick;
	int TypeListNormalBrick;
	vector<Brick*>Bricks;
	ListNormalBrick(int number, vector<int>list, float startx, float starty);
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};


