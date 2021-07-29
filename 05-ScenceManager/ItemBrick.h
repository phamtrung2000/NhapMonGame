#pragma once
#include "Brick.h"

#define ITEMBRICK_SPEED_Y	0.05f
//#define max_high	5

#define BRICK_STATE_NORMAL		0
#define BRICK_STATE_COLLISION	1
#define BRICK_STATE_EMPTY		2
#define ITEMBRICK_STATE_DIE		3

#define NORMAL	0
#define MUSHROOM 1
#define BUTTONP	2
#define MONEYX10 3
#define ITEMBRICK_LEAF 4

#define ITEMBRICK_WIDTH	16.0f
#define ITEMBRICK_ANISET_ID	13

class ItemBrick : public Brick
{
public:
	bool isCollision;
	bool hasItem;
	int Item;
	bool check;
	int CountMoney;
	ItemBrick();
	ItemBrick(int item, float x = 0.0f, float y = 0.0f);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};