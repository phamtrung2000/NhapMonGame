#pragma once
#include "FirePiranhaPlant.h"


#define	GREENFIREPLANT_SPEED_Y		0.03f
#define	GREENFIREPLANT_BBOX_HEIGHT	24
#define	GREENFIREPLANT_BBOX_WIDTH		16

#define	ATTACK_ZONE_Y	200
#define	MAX_ATTACK_ZONE_X	150

#define GREENFIREPLANT_STATE_HIDE		0
#define GREENFIREPLANT_STATE_APPEAR	100
#define GREENFIREPLANT_STATE_ATTACK	200
#define GREENFIREPLANT_STATE_DIE		300
#define GREENFIREPLANT_STATE_STOP		400

#define GREENFIREPLANT_ANI_APPEAR_HIDE_UP_LEFT	0
#define GREENFIREPLANT_ANI_APPEAR_HIDE_DOWN_LEFT	1
#define GREENFIREPLANT_ANI_ATTACK_UP_LEFT			2
#define GREENFIREPLANT_ANI_ATTACK_DOWN_LEFT		3

class GreenFirePlant : public FirePiranhaPlant
{
	
public:
	GreenFirePlant();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + GREENFIREPLANT_BBOX_WIDTH;
		bottom = top + GREENFIREPLANT_BBOX_HEIGHT;
	}

	void GetEnemyPos(float eX, float eY) { Mario_X = eX; Mario_Y = eY; }
	void CalcAttackZone();
	void CalcAtkPos();
};