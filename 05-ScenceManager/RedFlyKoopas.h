#pragma once
#include "Koopas.h"

#define KOOPAS_WALKING_SPEED				0.05f
#define KOOPAS_SHELL_SPEED					0.15f
#define	KOOPAS_GRAVITY						0.0007f
#define KOOPAS_SHELL_GRAVITY				0.002f
#define KOOPAS_SHELL_2_GRAVITY				0.0003f
#define KOOPAS_BBOX_WIDTH					16
#define KOOPAS_BBOX_HEIGHT					24
#define KOOPAS_BBOX_HEIGHT_SHELL			15
#define KOOPAS_BBOX_WIDTH_SHELL				18

#define KOOPAS_STATE_FLY_DOWN		10
#define KOOPAS_STATE_FLY_UP			11

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
#define REDFLYKOOPAS_ANI_WALKING_LEFT		10

#define KOOPAS_REVIVE_TIME					8000
#define KOOPAS_START_REVIVE_TIME			5000
#define MAX									100000000
#define MIN									-100000000

class RedFlyKoopas : public Koopas
{
public:
	RedFlyKoopas();
	~RedFlyKoopas();
	bool isKicked, isShell, isShell_2, isHold;
	float X_max, X_min;
	bool CountXmaxXmin;
	ULONGLONG StartChangeUpDown;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};


