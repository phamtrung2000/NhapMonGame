#include "QuestionBrickItem.h"
#include "PlayScence.h"
#include "EffectScore.h"

QuestionBrickItem::QuestionBrickItem(int item,float x, float y) : CGameObject()
{
	isInit = false;
	Item = item;
	ObjType = 8;
	Start_X = x;
	Start_Y = y;
	this->x = x;
	this->y = y;
	vx = vy = 0;
	SetState(QUESTIONBRICKITEM_STATE_INIT);
	Category = CATEGORY::ITEM;
	AppearTime = GetTickCount64();
	switch (item)
	{
	case MONEY:
		score = QUESTIONBRICKITEM_MONEY__SCORE; break;
	case MUSHROOM: case LEAF:
		score = QUESTIONBRICKITEM__SCORE; break;
	}
}

void QuestionBrickItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	switch (Item)
	{
	case MONEY:
	{
		vy += MONEY_GRAVITY * dt;
		if (GetTickCount64() - AppearTime >= 800)
		{
			isDie = true;
			EffectScore* score = new EffectScore(this->x, this->y, this->score);
			_PlayScene->objects.push_back(score);
		}
			
		y += dy;
		
	}break;

	case MUSHROOM:
	{
		if (isInit == false)
		{
			y += dy;
			if (Start_Y - y > QUESTIONBRICKITEM__BBOX)
				SetState(QUESTIONBRICKITEM_STATE_MOVE);
		}
		else
		{
			vy += MUSHROOM_GRAVITY * dt;

			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();

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

				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

				
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				if (ny != 0) vy = 0;

				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult[i];

					if (dynamic_cast<Brick*>(e->obj))
					{
						x += dx;
					}
					else if (dynamic_cast<WarpPipe*>(e->obj))
					{
						
						vx = -vx;
						y += dy;
					}
					else if (dynamic_cast<CGoomba*>(e->obj))
					{
						DebugOut(L"Goombaa\n");
						x += dx;
					}
					else if (dynamic_cast<Block*>(e->obj))
					{
						x += dx;
					}
					else if (!dynamic_cast<CGoomba*>(e->obj))
						if (ny != 0)
							vy = 0;

				}
			}

			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}break;

	case LEAF:
	{
		x += dx;
		y += dy;
		if (isInit == false)
		{
			if (static_cast<float>(Start_Y - y) >= static_cast < float> (3.5 * QUESTIONBRICKITEM__BBOX))
			{
				vy = 0;
				SetState(QUESTIONBRICKITEM_STATE_MOVE_RIGHT);
			}
			
		}
		else
		{
			vy += LEAF_GRAVITY * dt;
			if (x - Start_X >=  QUESTIONBRICKITEM__BBOX)
			{
				SetState(QUESTIONBRICKITEM_STATE_MOVE_LEFT);
			}
			else if (x - Start_X <= 0)
			{
				SetState(QUESTIONBRICKITEM_STATE_MOVE_RIGHT);
			}
		}
	}break;
	default:
		break;
	}
	
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
}

void QuestionBrickItem::Render()
{
	int ani = -1;
	switch (Item)
	{
	case MONEY:
		ani = QUESTIONBRICKITEM_MONEY_ANI;
		break;
	case MUSHROOM:
		ani = QUESTIONBRICKITEM_MUSHROOM_ANI;
		break;
	case LEAF:
	{
		ani = LEAF_ANI_RIGHT;
		if(GetState() == QUESTIONBRICKITEM_STATE_MOVE_LEFT)
			ani = LEAF_ANI_LEFT;
		else if (GetState() == QUESTIONBRICKITEM_STATE_MOVE_RIGHT)
			ani = LEAF_ANI_RIGHT;
	}break;
	default:
		break;
	}

	/*if( Item == MONEY )
		ani = QUESTIONBRICKITEM_MONEY_ANI;
	else
		ani = QUESTIONBRICKITEM_MUSHROOM_ANI;
		ani = LEAF_ANI_RIGHT;*/
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();

}

void QuestionBrickItem::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case QUESTIONBRICKITEM_STATE_INIT:
	{
		switch (Item)
		{
		case MONEY:
			vy = -MONEY_SPEED_Y;
			break;
		case MUSHROOM:
			vy = -MUSHROOM_SPEED_Y;
			break;
		case LEAF:
			vy = -LEAF_SPEED_Y;
			break;
		default:
			break;
		}
		
	}
	break;

	case QUESTIONBRICKITEM_STATE_MOVE:
	{
		isInit = true;
		if (Item == MUSHROOM)
		{
			if (nx == 1)
				vx = MUSHROOM_SPEED_X;
			else
				vx = -MUSHROOM_SPEED_X;
		}
		else if (Item == LEAF)
		{
			if (nx == 1)
				vx = LEAF_SPEED_X;
			else
				vx = -LEAF_SPEED_X;
		}
		
	}
	break;

	case QUESTIONBRICKITEM_STATE_MOVE_RIGHT:
	{
		isInit = true;
		if (Item == MUSHROOM)
		{
			nx = 1;
			vx = MUSHROOM_SPEED_X;
			
		}
		else if (Item == LEAF)
		{
			nx = 1;
			vx = LEAF_SPEED_X;
		}
	}
	break;
	case QUESTIONBRICKITEM_STATE_MOVE_LEFT:
	{
		isInit = true;
		if (Item == MUSHROOM)
		{
			nx = -1;
			vx = -MUSHROOM_SPEED_X;

		}
		else if (Item == LEAF)
		{
			nx = -1;
			vx = -LEAF_SPEED_X;
		}
	}
	break;

	}
}

void QuestionBrickItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if(Item==MONEY)
		right = x + MONEY_BBOX_WIDTH;
	else
		right = x + QUESTIONBRICKITEM__BBOX;
	bottom = y + QUESTIONBRICKITEM__BBOX;
}

void QuestionBrickItem :: CaclVx(int objx)
{
	//if (objx > x + (QUESTIONBRICKITEM__BBOX /3) )
	if (objx < x)
		nx = 1;
	else
		nx = -1;
}
