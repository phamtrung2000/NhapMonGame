#pragma once
#include "Enemy.h"

#define	GREENPLANT_SPEED_Y		0.03f
#define	GREENPLANT_BBOX_HEIGHT	24
#define	GREENPLANT_BBOX_WIDTH	16

#define	ATTACK_ZONE_Y	200

#define GREENPLANT_STATE_HIDE		0
#define GREENPLANT_STATE_APPEAR		100
#define GREENPLANT_STATE_DIE		200
#define GREENPLANT_STATE_STOP		300

#define GREENPLANT_ANI_APPEAR_HIDE	0
#define APPEAR_TIME 500


class GreenPlant : public Enemy
{
public:
	ULONGLONG AppearTime;

	// lấy vị trí mario để khi mario tới tầm thì cây xuất hiện
	float Mario_X;
	float Mario_Y;

	int WarpPipeHeight, WarpPipeWidth;

	bool Stop, isBlocked;
public:
	GreenPlant();
	~GreenPlant();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + GREENPLANT_BBOX_WIDTH;
		bottom = top + GREENPLANT_BBOX_HEIGHT;
	}

	void GetEnemyPos(float eX, float eY) { Mario_X = eX; Mario_Y = eY; }
	void CalcAttackZone();
	void ChangeStateHideAppear();
};
