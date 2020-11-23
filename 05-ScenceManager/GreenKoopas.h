#pragma once
#include "GameObject.h"
#include "Block.h"
#include "WarpPipe.h"
#include "Utils.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Mario.h"
#include "Ground.h"
#include "PlayScence.h"

#define GREENKOOPAS_WALKING_SPEED				0.05f
#define GREENKOOPAS_SHELL_SPEED					0.15f
#define	GREENKOOPAS_GRAVITY						0.0007f
#define GREENKOOPAS_SHELL_GRAVITY				0.002f
#define GREENKOOPAS_SHELL_2_GRAVITY				0.0003f
#define GREENKOOPAS_BBOX_WIDTH					16
#define GREENKOOPAS_BBOX_HEIGHT					26
#define GREENKOOPAS_BBOX_HEIGHT_SHELL			16
#define GREENKOOPAS_STATE_WALKING_RIGHT			100
#define GREENKOOPAS_STATE_WALKING_LEFT			200
#define GREENKOOPAS_STATE_SHELL					300
#define GREENKOOPAS_STATE_SHELL_HOLD			400
#define GREENKOOPAS_STATE_SHELL_WALKING_RIGHT	500
#define GREENKOOPAS_STATE_SHELL_WALKING_LEFT	600
#define GREENKOOPAS_STATE_DIE					700
#define GREENKOOPAS_STATE_SHELL_2				800
#define GREENKOOPAS_ANI_WALKING_LEFT			0
#define GREENKOOPAS_ANI_WALKING_RIGHT			1
#define GREENKOOPAS_ANI_SHELL					2
#define GREENKOOPAS_ANI_SHELL_2					3
#define GREENKOOPAS_ANI_SHELL_WALKING_RIGHT		4
#define GREENKOOPAS_ANI_SHELL_WALKING_LEFT		5
#define GREENKOOPAS_ANI_SHELL_2_WALKING_RIGHT	6
#define GREENKOOPAS_ANI_SHELL_2_WALKING_LEFT	7
#define GREENKOOPAS_ANI_SHELL_REVIVE			8
#define GREENKOOPAS_REVIVE_TIME					8000
#define GREENKOOPAS_START_REVIVE_TIME			5000

class GreenKoopas : public CGameObject
{
public:
	GreenKoopas();
	bool isKicked, GoAround, isShell, isShell_2, isHold;
	bool isAttacked = false;
	float X_max, X_min;
	bool CountXmaxXmin;

	bool Revive;
	DWORD ReviveTime;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
