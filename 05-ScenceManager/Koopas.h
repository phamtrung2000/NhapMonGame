﻿#pragma once
#include "Enemy.h"

#define KOOPAS_SHELL_SPEED					0.12f
#define KOOPAS_SHELL_GRAVITY				0.0002f
#define KOOPAS_SHELL_2_ACCELERATION			0.0005f

#define KOOPAS_BBOX_WIDTH					16
#define KOOPAS_BBOX_HEIGHT					24
#define KOOPAS_BBOX_HEIGHT_SHELL			15
#define KOOPAS_BBOX_WIDTH_SHELL				18

#define KOOPAS_STATE_SHELL					5
#define KOOPAS_STATE_SHELL_2				6
#define KOOPAS_STATE_SHELL_HOLD				7
#define KOOPAS_STATE_SHELL_2_HOLD			8
#define KOOPAS_STATE_SHELL_WALKING_RIGHT	9
#define KOOPAS_STATE_SHELL_WALKING_LEFT		10
#define KOOPAS_STATE_SHELL_2_WALKING_RIGHT	11
#define KOOPAS_STATE_SHELL_2_WALKING_LEFT	12
#define KOOPAS_STATE_DIE					13

#define KOOPAS_ANI_WALKING_LEFT				0
#define KOOPAS_ANI_WALKING_RIGHT			1
#define KOOPAS_ANI_SHELL					2
#define KOOPAS_ANI_SHELL_2					3
#define KOOPAS_ANI_SHELL_WALKING_RIGHT		4
#define KOOPAS_ANI_SHELL_WALKING_LEFT		5
#define KOOPAS_ANI_SHELL_2_WALKING_RIGHT	6
#define KOOPAS_ANI_SHELL_2_WALKING_LEFT		7
#define KOOPAS_ANI_SHELL_REVIVE				8
#define KOOPAS_ANI_SHELL_2_REVIVE			9

#define KOOPAS_REVIVE_TIME					8000
#define KOOPAS_START_REVIVE_TIME			5000
#define MAX									100000000
#define MIN									-100000000

#define KOOPAS_SCORE	100

class Koopas : public Enemy
{
public:
	Koopas();
	bool isKicked, isShell,isShell_2, isHold;
	float X_max, X_min;
	bool CountXmaxXmin; // đã tính Xmax Xmin hay chưa để khỏi tính lại

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	virtual void CollisionWithPlayer(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};


