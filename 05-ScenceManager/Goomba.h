#pragma once
#include "GameObject.h"
#include "Utils.h"

//#define GOOMBA_WALKING_SPEED	0.05f
#define GOOMBA_WALKING_SPEED	0.05f
#define	GOOMBA_GRAVITY			0.0007f

#define GOOMBA_BBOX_WIDTH		16
#define GOOMBA_BBOX_HEIGHT		15
#define GOOMBA_BBOX_HEIGHT_DIE	9

#define GOOMBA_STATE_WALKING	100
#define GOOMBA_STATE_DIE		200
#define GOOMBA_STATE_DIE_2		300

#define GOOMBA_ANI_WALKING	0
#define GOOMBA_ANI_DIE		1
#define GOOMBA_ANI_DIE_2	2

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	bool isDie2;
public: 	
	CGoomba();
	virtual void SetState(int state);
};