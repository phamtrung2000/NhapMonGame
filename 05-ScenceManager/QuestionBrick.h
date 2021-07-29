#pragma once
#include "Brick.h"

#define BRICK_SPEED_Y	0.05f

#define BRICK_STATE_NORMAL		0
#define BRICK_STATE_COLLISION	1
#define BRICK_STATE_EMPTY		2

class QuestionBrick : public Brick
{
public:
	bool isCollision;
	bool hasItem;
	int Item;
public:
	QuestionBrick();
	QuestionBrick(int item,float x = 0.0f, float y = 0.0f);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

