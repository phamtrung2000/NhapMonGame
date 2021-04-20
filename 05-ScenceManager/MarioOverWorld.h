#pragma once
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "GameObject.h"
#include "Ground.h"

#define MARIO_OVERWORLD_WALKING_SPEED		0.1f 

#define MARIO_OVERWORLD_STATE_IDLE			0
#define MARIO_OVERWORLD_STATE_WALKING_RIGHT	100
#define MARIO_OVERWORLD_STATE_WALKING_LEFT	200
#define MARIO_OVERWORLD_STATE_WALKING_UP	300
#define MARIO_OVERWORLD_STATE_WALKING_DOWN	400
#define MARIO_OVERWORLD_STATE_IN_GATE		500 // mario đứng trên cổng

#define MARIO_OVERWORLD_ANI_SMALL		0
#define MARIO_OVERWORLD_ANI_BIG		1

#define	MARIO_OVERWORLD_LEVEL_SMALL	1
#define	MARIO_OVERWORLD_LEVEL_BIG		2
#define	MARIO_OVERWORLD_LEVEL_TAIL	3
#define	MARIO_OVERWORLD_LEVEL_FIRE	4

#define MARIO_OVERWORLD_SMALL_BBOX_WIDTH  14//14
#define MARIO_OVERWORLD_SMALL_BBOX_HEIGHT 16

#define MARIO_OVERWORLD_BIG_BBOX_WIDTH  16
#define MARIO_OVERWORLD_BIG_BBOX_HEIGHT 21

#define MARIO_OVERWORLD_TAIL_BBOX_HEIGHT 24



class MarioOverWorld : public CGameObject
{
public:
	//static MarioOverWorld* __instance;

	int level;
	float start_x;			// initial position of MarioOverWorld at scene
	float start_y;
	int Scene;
	bool InGate; // mario đang đứng trên cổng thì mới cho phép chuyển cảnh

	int CanGoLeft,
		CanGoRight,
		CanGoUp,
		CanGoDown;

	//static MarioOverWorld* GetInstance();
	MarioOverWorld(float x = 0.0f, float y = 0.0f);
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};