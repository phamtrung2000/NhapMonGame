#pragma once
#include "GameObject.h"


#define	GREENFIREPLANT_SPEED_Y		0.03f
#define	GREENFIREPLANT_BBOX_HEIGHT	24
#define	GREENFIREPLANT_BBOX_WIDTH		16

#define	ATTACK_ZONE_Y	200
#define	ATTACK_ZONE_X	150

#define GREENFIREPLANT_STATE_HIDE		0
#define GREENFIREPLANT_STATE_APPEAR	100
#define GREENFIREPLANT_STATE_ATTACK	200
#define GREENFIREPLANT_STATE_DIE		300
#define GREENFIREPLANT_STATE_STOP		400

#define GREENFIREPLANT_ANI_APPEAR_HIDE_UP_LEFT	0
#define GREENFIREPLANT_ANI_APPEAR_HIDE_DOWN_LEFT	1
#define GREENFIREPLANT_ANI_ATTACK_UP_LEFT			2
#define GREENFIREPLANT_ANI_ATTACK_DOWN_LEFT		3

class GreenFirePlant : public CGameObject
{
public:
	float Startposy;
	DWORD CalcAtkTime;
	DWORD AppearTime;

	float AtkPosY;
	float StartAtkPosX;
	float StartAtkPosY;

	float Mario_X;// lấy vị trí mario để khi mario tới tầm thì cây xuất hiện
	float Mario_Y;// lấy vị trí mario để khi mario tới tầm thì cây xuất hiện

	float VxBullet;
	float VyBullet;
	int NumberBullet;
	bool canAttack, isAttacking;
	float WarpPipeHeight;
	bool Stop;
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

//void GreenFirePlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
//{
//	CGameObject::Update(dt);
//	
//	//y += dy;
//
//	if (isInit == false)
//	{
//		Startposy = y;
//		isInit = true;
//	}
//
//	if (AppearTime == 0)
//	{
//		// set giá trị time mới
//		AppearTime = GetTickCount64();
//	}
//
//	if (canAttack && CalcAtkTime == 0)
//	{
//		CalcAtkTime = GetTickCount64();
//	}
//	if (canAttack && GetTickCount64() - CalcAtkTime >= 2000)
//	{
//		canAttack = false;
//		CalcAtkTime = 0;
//	}
//
//	//DebugOut(L"Startposy=%f, y=%f\n", Startposy, y);
//	// cây chạm đáy, chuyển thành state appear để đi lên
//	if (y - Startposy  >= 0)
//	{
//		if (GetTickCount64() - AppearTime >= 3000)
//		{
//			SetState(GREENFIREPLANT_STATE_APPEAR);
//		//	DebugOut(L"GREENFIREPLANT_STATE_APPEAR\n");
//			AppearTime = 0;
//		}
//	}
//
//	
//	if ( y - Startposy <= -32)
//	{
//		if (GetTickCount64() - AppearTime >= 3000)
//		{
//			SetState(GREENFIREPLANT_STATE_HIDE);
//		//	DebugOut(L"GREENFIREPLANT_STATE_HIDE\n");
//			AppearTime = 0;
//		}
//		CalcAttackZone();
//	}
//
//	if (canAttack && NumberBullet == 1)
//	{
//		CalcAtkPos();
//	}
//	
//	vector<LPCOLLISIONEVENT> coEvents;
//	vector<LPCOLLISIONEVENT> coEventsResult;
//
//	coEvents.clear();
//
//	if (state != GREENFIREPLANT_STATE_DIE)
//		CalcPotentialCollisions(coObjects, coEvents);
//
//	if (coEvents.size() == 0)
//	{
//		x += dx;
//		y += dy;
//	}
//	else
//	{
//		float min_tx, min_ty, nx = 0, ny;
//		float rdx = 0;
//		float rdy = 0;
//
//		// TODO: This is a very ugly designed function!!!!
//		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
//
//		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
//		//if (rdx != 0 && rdx!=dx)
//		//	x += nx*abs(rdx); 
//
//
//		// chuẩn + ít lỗi nhất
//		this->x += min_tx * dx + nx * 0.4f;
//		if (ny != 0) vy = 0;
//
//		for (UINT i = 0; i < coEventsResult.size(); i++)
//		{
//			LPCOLLISIONEVENT e = coEventsResult[i];
//			if (e->obj->ObjType == OBJECT_TYPE_WARPPIPE)
//			{
//				DebugOut(L"11111111111111111\n");
//			}
//			DebugOut(L"type = %i\n", e->obj->ObjType);
//		
//		}
//	}
//
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//	DebugOut(L"state = %i\n", state);
//}