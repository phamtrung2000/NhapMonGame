#include "Goomba.h"
#include "WarpPipe.h"
#include "Block.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Koopas.h"
#include "FireBullet.h"
#include "Ground.h"


CGoomba::CGoomba()
{
	isDie2 = isDie = false;
	ObjType = 2;
	SetState(GOOMBA_STATE_WALKING);
	nx = 1;
	vy = 0;
	Category = CATEGORY::ENEMY;
}

//void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
//{
//	CGameObject::Update(dt, coObjects);
//
//	if(GetState() != GOOMBA_STATE_DIE)
//		vy += GOOMBA_GRAVITY * dt;
//	if (state == GOOMBA_STATE_DIE)
//	{
//		if(time_die<=20)
//			time_die++;
//		else
//		{
//			isDie = true;
//		}
//	}
//	else if (state == GOOMBA_STATE_DIE_2)
//	{
//		//vy += GOOMBA_GRAVITY * dt;
//		if(y > 450)
//			isDie = true;
//	}
//
//	
//	if (vx < 0 && x < 0)
//	{
//		x = 0; 
//		vx = -vx;
//		nx = -nx;
//	}
//
//	vector<LPCOLLISIONEVENT> coEvents;
//	vector<LPCOLLISIONEVENT> coEventsResult;
//
//	coEvents.clear();
//
//	if (state != GOOMBA_STATE_DIE)
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
//
//			if (dynamic_cast<WarpPipe*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj))
//			{
//				if (e->nx != 0)
//				{
//					this->nx = -this->nx;
//					this->vx = this->nx * GOOMBA_WALKING_SPEED;
//					x += dx;
//					//this->y += min_ty * dy + ny * 0.4f;
//				}
//				
//			}
//			else if (dynamic_cast<Block*>(e->obj))
//			{
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
//				if (kp->isShell == false && kp->isShell_2 == false)
//				{
//					this->vx = this->nx * GOOMBA_WALKING_SPEED;
//					x += dx;
//					this->y += min_ty * dy + ny * 0.4f;
//				}
//				else
//				{
//					if (kp->GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT || kp->GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
//					{
//						SetState(GOOMBA_STATE_DIE_2);
//					}
//				}
//			}
//			else if (dynamic_cast<FireBullet*>(e->obj))
//			{
//				FireBullet* fb = dynamic_cast<FireBullet*>(e->obj);
//				if (fb->FireMario == false)
//				{
//					this->vx = this->nx * GOOMBA_WALKING_SPEED;
//					vy = 0;
//					x += dx;
//				}
//			}
//		}
//	}
//
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (GetState() != GOOMBA_STATE_DIE)
		vy += GOOMBA_GRAVITY * dt;
	if (state == GOOMBA_STATE_DIE)
	{
		if (time_die <= 20)
			time_die++;
		else
		{
			isDie = true;
		}
	}
	else if (state == GOOMBA_STATE_DIE_2)
	{
		//vy += GOOMBA_GRAVITY * dt;
		if (y > 450)
			isDie = true;
	}


	if (vx < 0 && x < 0)
	{
		x = 0;
		vx = -vx;
		nx = -nx;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 


		// chuẩn + ít lỗi nhất
		this->x += min_tx * dx + nx * 0.4f;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<WarpPipe*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					this->nx = -this->nx;
					this->vx = this->nx * GOOMBA_WALKING_SPEED;
					x += dx;
					//this->y += min_ty * dy + ny * 0.4f;
				}

			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				this->vx = this->nx * GOOMBA_WALKING_SPEED;
				x += dx;
				//this->y += min_ty * dy + ny * 0.4f;
			}
			else if (dynamic_cast<CGoomba*>(e->obj))
			{
				this->vx = this->nx * GOOMBA_WALKING_SPEED;
				x += dx;
				this->y += min_ty * dy + ny * 0.4f;
			}
			else if (dynamic_cast<Koopas*>(e->obj))
			{
				Koopas* kp = dynamic_cast<Koopas*>(e->obj);
				if (kp->isShell == false && kp->isShell_2 == false)
				{
					this->vx = this->nx * GOOMBA_WALKING_SPEED;
					x += dx;
					this->y += min_ty * dy + ny * 0.4f;
				}
				else
				{
					if (kp->GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT || kp->GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
					{
						SetState(GOOMBA_STATE_DIE_2);
					}
				}
			}
			else if (dynamic_cast<FireBullet*>(e->obj))
			{
				FireBullet* fb = dynamic_cast<FireBullet*>(e->obj);
				if (fb->FireMario == false)
				{
					this->vx = this->nx * GOOMBA_WALKING_SPEED;
					vy = 0;
					x += dx;
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == GOOMBA_STATE_DIE_2)
	{
		ani = GOOMBA_ANI_DIE_2;
	}
	animation_set->at(ani)->Render(x, y);
	
	/*if (isDie == false)
		DebugOut(L"isDie == false\t");
	else
		DebugOut(L"isDie == true\t");*/

	//DebugOut(L"5.Render Goomba Ani: %i, State=%i, time_die=%i\n", ani, state,time_die);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
		{
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
		}
		break;

		case GOOMBA_STATE_DIE_2:
		{
			nx = -nx;
			vx = -vx;
			vy = -0.1f;
		}
		break;

		case GOOMBA_STATE_WALKING: 
		{
			vx = GOOMBA_WALKING_SPEED;
		}
		break;
	}
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != GOOMBA_STATE_DIE_2)
	{
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}
	
}

//#include "Goomba.h"
//#include "WarpPipe.h"
//#include "QuestionBrick.h"
//#include "Block.h"
//
//#define GRAVITY				0.0007f
//
//CGoomba::CGoomba()
//{
//	SetState(GOOMBA_STATE_WALKING);
//	ObjType = 2;
//}
//
//void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
//{
//	left = x;
//	top = y;
//	right = x + GOOMBA_BBOX_WIDTH;
//
//	if (state == GOOMBA_STATE_DIE)
//		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
//	else
//		bottom = y + GOOMBA_BBOX_HEIGHT;
//}
//
//void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
//{
//	CGameObject::Update(dt, coObjects);
//	//
//	// TO-DO: make sure Goomba can interact with the world and to each of them too!
//	// 
//	if (state != GOOMBA_STATE_DIE)
//		vy += GRAVITY * dt;
//	vector<LPCOLLISIONEVENT> coEvents;
//	vector<LPCOLLISIONEVENT> coEventsResult;
//
//	coEvents.clear();
//	CalcPotentialCollisions(coObjects, coEvents);
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
//	
//		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
//	
//		// Collision logic with Goombas
//		for (UINT i = 0; i < coEventsResult.size(); i++)
//		{
//			LPCOLLISIONEVENT e = coEventsResult[i];
//			if (e->nx && !dynamic_cast<Block*>(e->obj))
//			{
//				vx = -vx;
//			}
//			x += dx;
//			if (ny != 0) vy = 0;
//			y += min_ty * dy + ny * 0.5f;
//		}
//	}
//	
//}
//
//void CGoomba::Render()
//{
//	int ani = GOOMBA_ANI_WALKING;
//	if (state == GOOMBA_STATE_DIE) {
//		ani = GOOMBA_ANI_DIE;
//	}
//
//	animation_set->at(ani)->Render(x, y);
//
//	RenderBoundingBox();
//}
//
//void CGoomba::SetState(int state)
//{
//	CGameObject::SetState(state);
//	switch (state)
//	{
//	case GOOMBA_STATE_DIE:
//		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
//		vx = 0;
//		vy = 0;
//		break;
//	case GOOMBA_STATE_WALKING:
//		vx = GOOMBA_WALKING_SPEED;
//	}
//}
