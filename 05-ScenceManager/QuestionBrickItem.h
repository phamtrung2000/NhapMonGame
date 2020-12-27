#include "GameObject.h"
#include "Utils.h"
#include "Block.h"
#include "Goomba.h"
#include "Brick.h"
#include "WarpPipe.h"

#define MONEY							0
#define QUESTIONBRICKITEM_MONEY_ANI		0
#define MONEY_GRAVITY					0.0007f
#define MONEY_SPEED_Y					0.3f

#define MUSHROOM						1
#define QUESTIONBRICKITEM_MUSHROOM_ANI	1
#define MUSHROOM_GRAVITY				0.0007f
#define MUSHROOM_SPEED_X				0.04f 
#define MUSHROOM_SPEED_Y				0.02f 

#define LEAF							2
#define LEAF_ANI_LEFT					2
#define LEAF_ANI_RIGHT					3
#define LEAF_GRAVITY					0.00002f
#define LEAF_SPEED_X					0.03f 
#define LEAF_SPEED_Y					0.1f

#define QUESTIONBRICKITEM__BBOX			16
#define MONEY_BBOX_WIDTH				8
#define QUESTIONBRICKITEM_STATE_INIT	1
#define	QUESTIONBRICKITEM_STATE_MOVE	2
#define	QUESTIONBRICKITEM_STATE_MOVE_RIGHT	3
#define	QUESTIONBRICKITEM_STATE_MOVE_LEFT	4

#define QUESTIONBRICKITEM__SCORE		1000
#define QUESTIONBRICKITEM_MONEY__SCORE		100

class QuestionBrickItem : public CGameObject
{
public:
	float Start_X;
	float Start_Y;
	int Item;
	ULONGLONG AppearTime;
	int score;
public:
	QuestionBrickItem(int item,float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void CaclVx(int objx);
};

//DebugOut(L"state=%i, nx=%i, vx = %f\n", state,nx,vx);
	//if (Item == MUSHROOM)
	//{
	//	if (isInit == false)
	//	{
	//		y += dy;
	//		if (Start_Y - y > QUESTIONBRICKITEM__BBOX)
	//			SetState(QUESTIONBRICKITEM_STATE_MOVE);
	//	}
	//	else
	//	{
	//		vy += MUSHROOM_GRAVITY * dt;
	//
	//		vector<LPCOLLISIONEVENT> coEvents;
	//		vector<LPCOLLISIONEVENT> coEventsResult;
	//
	//		coEvents.clear();
	//
	//		CalcPotentialCollisions(coObjects, coEvents);
	//
	//		if (coEvents.size() == 0)
	//		{
	//			x += dx;
	//			y += dy;
	//		}
	//		else
	//		{
	//			float min_tx, min_ty, nx = 0, ny;
	//			float rdx = 0;
	//			float rdy = 0;
	//
	//			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	//
	//			//for (UINT i = 0; i < coEventsResult.size(); i++)
	//			//{
	//			//	LPCOLLISIONEVENT e = coEventsResult[i];
	//			//
	//			//	if (dynamic_cast<Brick*>(e->obj))
	//			//	{
	//			//
	//			//		x += dx;
	//			//		// cai if nay lam cai gi` ?
	//			//	/*if (e->nx)
	//			//		vx = 0; */
	//			//	}
	//			//	else if (dynamic_cast<WarpPipe*>(e->obj))
	//			//	{
	//			//		DebugOut(L"WarpPipe\n");
	//			//		vx = -vx;
	//			//		y += dy;
	//			//	}
	//			//	else if (dynamic_cast<CGoomba*>(e->obj))
	//			//	{
	//			//		DebugOut(L"Goombaa\n");
	//			//		x += dx;
	//			//	}
	//			//	if (dynamic_cast<Block*>(e->obj))
	//			//	{
	//			//		x += dx;
	//			//	}
	//			//	if (!dynamic_cast<CGoomba*>(e->obj))
	//			//		if (ny != 0) vy = 0;
	//			//	x += min_tx * dx + nx * 0.4f;
	//			//	y += min_ty * dy + ny * 0.5f;
	//			//}
	//
	//			x += min_tx * dx + nx * 0.4f;
	//			y += min_ty * dy + ny * 0.4f;
	//
	//			if (ny != 0) vy = 0;
	//
	//			for (UINT i = 0; i < coEventsResult.size(); i++)
	//			{
	//				LPCOLLISIONEVENT e = coEventsResult[i];
	//
	//				if (dynamic_cast<Brick*>(e->obj))
	//				{
	//					x += dx;
	//				}
	//				else if (dynamic_cast<WarpPipe*>(e->obj))
	//				{
	//					DebugOut(L"WarpPipe\n");
	//					vx = -vx;
	//					y += dy;
	//				}
	//				else if (dynamic_cast<CGoomba*>(e->obj))
	//				{
	//					DebugOut(L"Goombaa\n");
	//					x += dx;
	//				}
	//				else if (dynamic_cast<Block*>(e->obj))
	//				{
	//					x += dx;
	//				}
	//				else if (!dynamic_cast<CGoomba*>(e->obj))
	//					if (ny != 0) 
	//						vy = 0;
	//
	//			}
	//		}
	//		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//	}
	//}
	//else if (Item == MONEY)
	//{
	//	if (isInit == false)
	//	{
	//		Time++;
	//		y += dy;
	//		vy += MONEY_GRAVITY * dt;
	//		if (Time >= MONEY_APPEAR_TIME)
	//		{
	//			isDie = true;
	//			Time = 0;
	//		}
	//			
	//	}
	//}
	//DebugOut(L"vx=%f\n",vx);
