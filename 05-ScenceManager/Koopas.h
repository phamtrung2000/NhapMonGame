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

#define KOOPAS_WALKING_SPEED				0.05f
//#define KOOPAS_WALKING_SPEED				0.0001f

#define KOOPAS_SHELL_SPEED					0.15f
#define	KOOPAS_GRAVITY						0.0007f
#define KOOPAS_SHELL_GRAVITY				0.002f
#define KOOPAS_SHELL_2_GRAVITY				0.0003f
#define KOOPAS_BBOX_WIDTH					15
#define KOOPAS_BBOX_HEIGHT					26
#define KOOPAS_BBOX_HEIGHT_SHELL			15
#define KOOPAS_STATE_WALKING_RIGHT			100
#define KOOPAS_STATE_WALKING_LEFT			200
#define KOOPAS_STATE_SHELL					300
#define KOOPAS_STATE_SHELL_HOLD				400
#define KOOPAS_STATE_SHELL_WALKING_RIGHT	500
#define KOOPAS_STATE_SHELL_WALKING_LEFT		600
#define KOOPAS_STATE_DIE					700
#define KOOPAS_STATE_SHELL_2				800
#define KOOPAS_ANI_WALKING_LEFT				0
#define KOOPAS_ANI_WALKING_RIGHT			1
#define KOOPAS_ANI_SHELL					2
#define KOOPAS_ANI_SHELL_2					3
#define KOOPAS_ANI_SHELL_WALKING_RIGHT		4
#define KOOPAS_ANI_SHELL_WALKING_LEFT		5
#define KOOPAS_ANI_SHELL_2_WALKING_RIGHT	6
#define KOOPAS_ANI_SHELL_2_WALKING_LEFT		7
#define KOOPAS_ANI_SHELL_REVIVE				8
#define KOOPAS_REVIVE_TIME					8000
#define KOOPAS_START_REVIVE_TIME			5000
#define MAX									100000000
#define MIN									-100000000

class Koopas : public CGameObject
{
public:
	Koopas();
	bool isKicked, GoAround, isShell,isShell_2, isHold;
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

//void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
//{
//	CGameObject::Update(dt, coObjects);
//	if (GetState() == KOOPAS_STATE_SHELL_2)
//	{
//		vy += KOOPAS_SHELL_2_GRAVITY * dt;
//	}
//	else if (isHold!=true)
//	{
//	//	DebugOut(L"Update 33333333333 vy=%f\n", vy);
//		vy += KOOPAS_SHELL_GRAVITY * dt;
//	}
//
//	DebugOut(L"state=%i, vx=%f,vy=%f, nx=%i, x min = %f, x max = %f\n", state, vx, vy, nx, X_min, X_max);
//
//	/*if (GoAround == true)
//	{
//		if (isShell != true && isShell_2 != true)
//		{
//			if (x >= X_max && vx > 0)
//				SetState(KOOPAS_STATE_WALKING_LEFT);
//			else if (x <= X_min && vx < 0)
//				SetState(KOOPAS_STATE_WALKING_RIGHT);
//		}
//	}
//	*/
//
//	vector<LPCOLLISIONEVENT> coEvents;
//	vector<LPCOLLISIONEVENT> coEventsResult;
//
//	coEvents.clear();
//
//	if (state != KOOPAS_STATE_DIE)
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
//		this->x += min_tx * dx + nx * 0.4f;
//		if (ny != 0) vy = 0;
//	
//		//for (UINT i = 0; i < coEventsResult.size(); i++)
//		//{
//		//	LPCOLLISIONEVENT e = coEventsResult[i];
//		//	
//		//	if (dynamic_cast<Brick*>(e->obj))
//		//	{
//		//		GoAround = false;
//		//		//x += dx;
//		//	}
//		//	if (dynamic_cast<Block*>(e->obj) )
//		//	{
//		//		//DebugOut(L"e->nx=%f,e->ny=%f\n", e->nx, e->ny);
//		//		if (e->ny < 0)
//		//		{
//		//			if (CountXmaxXmin == false)
//		//			{
//		//				DebugOut(L"1111111111111\n");
//		//				Block* block = dynamic_cast<Block*>(e->obj);
//		//				X_min = block->x;
//		//				X_max = X_min + (block->Width - 16);
//		//				CountXmaxXmin = true;
//		//			}
//		//		}
//		//		this->vx = this->nx * KOOPAS_WALKING_SPEED;
//		//		x += dx;
//		//	}
//		//	
//		//	else if (dynamic_cast<WarpPipe*>(e->obj))
//		//	{
//		//		this->nx = -this->nx;
//		//		this->vx = this->nx * KOOPAS_WALKING_SPEED;
//		//		x += dx;
//		//	}
//		//	else if (dynamic_cast<QuestionBrick*>(e->obj))
//		//	{
//		//		QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
//		//		if (e->nx != 0)
//		//		{
//		//			vx = -vx;
//		//			if (brick->GetState() == BRICK_STATE_NORMAL && brick->Item < 10 && (isShell==true || isShell_2 == true))
//		//				brick->SetState(BRICK_STATE_COLLISION);
//		//		}
//		//	}
//		//	else if (dynamic_cast<CGoomba*>(e->obj))
//		//	{
//		//		if (e->nx != 0)
//		//		{
//		//			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
//		//			if (goomba->GetState() == GOOMBA_STATE_WALKING)
//		//			{
//		//				if ((isShell == true|| isShell_2 == true) && vx != 0)
//		//				{
//		//					goomba->SetState(GOOMBA_STATE_DIE_2);
//		//				}
//		//			}
//		//		}
//		//		else
//		//		{
//		//
//		//		}
//		//	}
//		//	else if (dynamic_cast<Koopas*>(e->obj))
//		//	{
//		//		this->vx = this->nx * KOOPAS_WALKING_SPEED;
//		//		x += dx;
//		//		//this->y += min_ty * dy + ny * 0.4f;
//		//	}
//		//}
//
//		for (UINT i = 0; i < coEventsResult.size(); i++)
//		{
//			LPCOLLISIONEVENT e = coEventsResult[i];
//
//			if (dynamic_cast<WarpPipe*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj))
//			{
//				if (e->nx != 0)
//				{
//					if (GetState() == KOOPAS_STATE_WALKING_RIGHT)
//						SetState(KOOPAS_STATE_WALKING_LEFT);
//					else if (GetState() == KOOPAS_STATE_WALKING_LEFT)
//						SetState(KOOPAS_STATE_WALKING_RIGHT);
//					x += dx;
//					//this->y += min_ty * dy + ny * 0.4f;
//				}
//
//			}
//			else if (dynamic_cast<Block*>(e->obj))
//			{
//				if (CountXmaxXmin == false)
//				{
//					Block* block = dynamic_cast<Block*>(e->obj);
//					X_min = block->x;
//					X_max = X_min + (block->Width - 16);
//					CountXmaxXmin = true;
//				}
//				this->vx = this->nx * GOOMBA_WALKING_SPEED;
//				x += dx;
//				//this->y += min_ty * dy + ny * 0.4f;
//			}
//			else if (dynamic_cast<CGoomba*>(e->obj))
//			{
//				this->vx = this->nx * GOOMBA_WALKING_SPEED;
//				x += dx;
//				this->y += min_ty * dy + ny * 0.4f;
//			}
//			else if (dynamic_cast<Koopas*>(e->obj))
//			{
//				Koopas* kp = dynamic_cast<Koopas*>(e->obj);
//				if (kp->isShell == false || kp->isShell_2 == false)
//				{
//					this->vx = this->nx * GOOMBA_WALKING_SPEED;
//					x += dx;
//					this->y += min_ty * dy + ny * 0.4f;
//				}
//			}
//			else if (dynamic_cast<FireBullet*>(e->obj))
//			{
//				FireBullet* fb = dynamic_cast<FireBullet*>(e->obj);
//				if (fb->FireMario == false)
//				{
//					DebugOut(L"11111111111111\n");
//					this->vx = this->nx * GOOMBA_WALKING_SPEED;
//					vy = 0;
//					x += dx;
//				}
//			}
//		}
//	}
//	//else
//	//{
//	//	float min_tx, min_ty, nx = 0, ny;
//	//	float rdx = 0;
//	//	float rdy = 0;
//	//
//	//	// TODO: This is a very ugly designed function!!!!
//	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
//	//
//	//	// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
//	//	//if (rdx != 0 && rdx!=dx)
//	//	//	x += nx*abs(rdx); 
//	//
//	//	// block every object first!
//	//	x += min_tx * dx + nx * 0.4f;
//	//	y += min_ty * dy + ny * 0.4f;
//	//
//	//	if (ny != 0) vy = 0;
//	//
//	//	for (UINT i = 0; i < coEventsResult.size(); i++)
//	//	{
//	//		LPCOLLISIONEVENT e = coEventsResult[i];
//	//		
//	//		if (dynamic_cast<WarpPipe*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj) )
//	//		{
//	//			DebugOut(L"e->nx=%f,e->ny=%f\n", e->nx, e->ny);
//	//			if (e->nx != 0)
//	//			{
//	//				vx = -vx;
//	//				vy = 0;
//	//			}
//	//			//y += min_ty * dy + ny * 0.4f;
//	//
//	//		}
//	//		else if (dynamic_cast<Block*>(e->obj) || dynamic_cast<CGoomba*>(e->obj))
//	//		{
//	//			x += dx;
//	//			//y += min_ty * dy + ny * 0.4f;
//	//		}
//	//		else if (dynamic_cast<Brick*>(e->obj))
//	//		{
//	//			if (e->ny < 0)
//	//				vy = 0;
//	//		}
//	//	}
//	//}
//	
//
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//	
//	
//	/*DebugOut(L"x =%f, bottom=%f, max=%f, min x=%f, vx=%f\n", x, X_max, X_min,vx);
//	if (isHold == true)
//		DebugOut(L"koopas->isHold==true, state=%i, vx=%f,vy=%f, nx=%i\n",state,vx,vy,nx);
//	else
//		DebugOut(L"koopas->isHold==false, state=%i, vx=%f,vy=%f, nx=%i\n", state,vx,vy,nx);*/
//	
//}


