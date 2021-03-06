﻿#pragma once
#include "GreenKoopas.h"

#define GREENKOOPAS_WALKING_SPEED				0.05f
#define GREENKOOPAS_SHELL_SPEED					0.15f
#define	GREENKOOPAS_GRAVITY						0.0007f
#define GREENKOOPAS_SHELL_GRAVITY				0.002f
#define GREENKOOPAS_SHELL_2_GRAVITY				0.0003f
#define GREENKOOPAS_BBOX_WIDTH					16
//#define GREENKOOPAS_BBOX_HEIGHT					27
#define GREENKOOPAS_BBOX_HEIGHT_SHELL			16

#define GREENKOOPAS_ANI_WALKING_LEFT			0
#define GREENKOOPAS_ANI_WALKING_RIGHT			1
#define GREENKOOPAS_ANI_SHELL					2
#define GREENKOOPAS_ANI_SHELL_2					3
#define GREENKOOPAS_ANI_SHELL_WALKING_RIGHT		4
#define GREENKOOPAS_ANI_SHELL_WALKING_LEFT		5
#define GREENKOOPAS_ANI_SHELL_2_WALKING_RIGHT	6
#define GREENKOOPAS_ANI_SHELL_2_WALKING_LEFT	7
#define GREENKOOPAS_ANI_SHELL_REVIVE			8
#define GREENKOOPAS_ANI_SHELL2_REVIVE			9
#define GREENFLYKOOPAS_ANI_WALKING_LEFT			10
#define GREENFLYKOOPAS_ANI_WALKING_RIGHT		11
#define GREENKOOPAS_REVIVE_TIME					800000
#define GREENKOOPAS_START_REVIVE_TIME			500000

class GreenFlyKoopas : public GreenKoopas
{
public:
	GreenFlyKoopas();
	~GreenFlyKoopas();
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};
//
//#pragma once
//#include "Koopas.h"
//#define GREENKOOPAS_WALKING_SPEED				0.05f
//#define GREENKOOPAS_SHELL_SPEED					0.15f
//#define	GREENKOOPAS_GRAVITY						0.0007f
//#define GREENKOOPAS_SHELL_GRAVITY				0.002f
//#define GREENKOOPAS_SHELL_2_GRAVITY				0.0003f
//#define GREENKOOPAS_BBOX_WIDTH					16
////#define GREENKOOPAS_BBOX_HEIGHT					27
//#define GREENKOOPAS_BBOX_HEIGHT_SHELL			16
//#define GREENKOOPAS_STATE_WALKING_RIGHT			100
//#define GREENKOOPAS_STATE_WALKING_LEFT			200
//#define GREENKOOPAS_STATE_SHELL					300
//#define GREENKOOPAS_STATE_SHELL_HOLD			400
//#define GREENKOOPAS_STATE_SHELL_WALKING_RIGHT	500
//#define GREENKOOPAS_STATE_SHELL_WALKING_LEFT	600
//#define GREENKOOPAS_STATE_DIE					700
//#define GREENKOOPAS_STATE_SHELL_2				800
//#define GREENKOOPAS_ANI_WALKING_LEFT			0
//#define GREENKOOPAS_ANI_WALKING_RIGHT			1
//#define GREENKOOPAS_ANI_SHELL					2
//#define GREENKOOPAS_ANI_SHELL_2					3
//#define GREENKOOPAS_ANI_SHELL_WALKING_RIGHT		4
//#define GREENKOOPAS_ANI_SHELL_WALKING_LEFT		5
//#define GREENKOOPAS_ANI_SHELL_2_WALKING_RIGHT	6
//#define GREENKOOPAS_ANI_SHELL_2_WALKING_LEFT	7
//#define GREENKOOPAS_ANI_SHELL_REVIVE			8
//#define GREENKOOPAS_ANI_SHELL2_REVIVE			9
//#define GREENFLYKOOPAS_ANI_WALKING_LEFT			10
//#define GREENFLYKOOPAS_ANI_WALKING_RIGHT		11
//#define GREENKOOPAS_REVIVE_TIME					800000
//#define GREENKOOPAS_START_REVIVE_TIME			500000
//
//class GreenFlyKoopas : public Koopas
//{
//public:
//	GreenFlyKoopas();
//	~GreenFlyKoopas();
//	bool isKicked, GoAround, isShell, isShell_2, isHold;
//	bool isAttacked = false;
//	float X_max, X_min;
//	bool CountXmaxXmin;
//	bool Revive;
//	ULONGLONG ReviveTime;
//	int Health;
//
//	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
//	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
//	virtual void Render();
//	virtual void SetState(int state);
//};