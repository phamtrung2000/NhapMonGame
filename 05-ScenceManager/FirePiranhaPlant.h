#pragma once
#include "GreenPlant.h"

#define	FIREPIRANHAPLANT_SPEED_Y		0.03f
#define	FIREPIRANHAPLANT_BBOX_HEIGHT	32
#define	FIREPIRANHAPLANT_BBOX_WIDTH		16

#define	ATTACK_ZONE_Y	200
#define	MAX_ATTACK_ZONE_X	150
#define	MIN_ATTACK_ZONE_X	32

#define FIREPIRANHAPLANT_STATE_HIDE		0
#define FIREPIRANHAPLANT_STATE_APPEAR	100
#define FIREPIRANHAPLANT_STATE_ATTACK	200
#define FIREPIRANHAPLANT_STATE_DIE		300
#define FIREPIRANHAPLANT_STATE_STOP		400

#define FIREPIRANHAPLANT_ANI_APPEAR_HIDE_UP_RIGHT	0
#define FIREPIRANHAPLANT_ANI_APPEAR_HIDE_DOWN_RIGHT	1
#define FIREPIRANHAPLANT_ANI_APPEAR_HIDE_UP_LEFT	2
#define FIREPIRANHAPLANT_ANI_APPEAR_HIDE_DOWN_LEFT	3
#define FIREPIRANHAPLANT_ANI_ATTACK_UP_RIGHT		4
#define FIREPIRANHAPLANT_ANI_ATTACK_DOWN_RIGHT		5
#define FIREPIRANHAPLANT_ANI_ATTACK_UP_LEFT			6
#define FIREPIRANHAPLANT_ANI_ATTACK_DOWN_LEFT		7
#define FIREPIRANHAPLANT_SCORE 100

class FirePiranhaPlant : public GreenPlant
{
public:
	float Startposy;
	ULONGLONG CalcAtkTime;
	ULONGLONG AppearTime;

	float AtkPosY;
	float StartAtkPosX;
	float StartAtkPosY;

	// lấy vị trí mario để khi mario tới tầm thì cây xuất hiện
	float Mario_X;
	float Mario_Y;

	float VxBullet;
	float VyBullet;
	int NumberBullet;
	bool canAttack,isAttacking;
	float WarpPipeHeight;
	bool Stop;
public:
	FirePiranhaPlant();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + FIREPIRANHAPLANT_BBOX_WIDTH;
		bottom = top + FIREPIRANHAPLANT_BBOX_HEIGHT;
	}

	void GetEnemyPos(float eX, float eY) { Mario_X = eX; Mario_Y = eY; }
	void CalcAttackZone();
	void CalcAtkPos();
};