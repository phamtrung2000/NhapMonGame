#pragma once
#include "GameObject.h"

#define BRICK_TYPE_NORMAL	0
#define BRICK_TYPE_QUESTION 1
#define BRICK_TYPE_ITEM		2
#define BRICK_TYPE_MUSIC	3

#define BIRCK_NORMAL		0
#define BRICK_CLOUD			1

#define NORMALBRICK_ANI_NORMAL		0
#define NORMALBRICK_ANI_CLOUD		1

#define QUESTIONBRICK_ANI_NORMAL	2
#define	QUESTIONBRICK_ANI_COLLISION	3
#define	ITEMBRICK_ANI_NORMAL		4
#define	MUSICBRICK_ANI_NORMAL		5
#define	HIDDENMUSICBRICK_ANI_NORMAL		6

class Brick : public CGameObject
{
public:
	int type_of_brick; // loại gạch
	int Type; // mỗi loại gạch lại có loại gạch nhỏ
	Brick();
	Brick(float x, float y);
	Brick(int type, float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state) {};
};