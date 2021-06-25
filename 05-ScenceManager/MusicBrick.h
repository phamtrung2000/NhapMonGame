#pragma once
#include "GameObject.h"

#define BRICK_SPEED_Y	0.05f
#define MAX_HIGH	8

#define MUSICBRICK_STATE_NORMAL		0
#define MUSICBRICK_STATE_COLLISION	1

#define ANI_BRICK_NORMAL 0
#define	ANI_BRICK_COLLISION	1

#define QUESTIONBRICK_WIDTH	16

class MusicBrick : public CGameObject
{
public:
	float Start_Y;
	bool isCollision;

	MusicBrick(float x = 0.0f, float y = 0.0f);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

