#pragma once
#include "GameObject.h"

#define ITEMBRICK_SPEED_Y	0.05f
#define MAX_HIGH	5

#define BRICK_STATE_NORMAL		0
#define BRICK_STATE_COLLISION	1
#define BRICK_STATE_EMPTY		2

#define ANI_BRICK_NORMAL 0
#define	ANI_BRICK_COLLISION	1

#define NORMAL	0
#define MUSHROOM 1
#define BUTTONP	2

#define ITEMBRICK_WIDTH	16.0f
#define ITEMBRICK_ANISET_ID	13

class ItemBrick : public CGameObject
{
public:
	float Start_Y;
	bool isCollision;
	bool hasItem;
	int Item;
	bool check;
public:
	ItemBrick(int item, float x = 0.0f, float y = 0.0f);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

