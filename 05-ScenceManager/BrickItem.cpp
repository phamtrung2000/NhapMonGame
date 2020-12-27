#include "BrickItem.h"
#include "ItemBrick.h"
#include "Coin.h"
#define BRICKITEM_ANISET_ID	14

BrickItem::BrickItem(int item, float x, float y) : CGameObject()
{
	isInit = false;
	Item = item;
	ObjType = OBJECT_TYPE_BRICKITEM;
	Start_X = x;
	Start_Y = y;
	this->x = x;
	this->y = y;
	vx = vy = 0;
	SetState(BRICKITEM_STATE_INIT);
	Category = CATEGORY::ITEM;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BRICKITEM_ANISET_ID);
	this->SetAnimationSet(ani_set);
	
}

void BrickItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	switch (Item)
	{
		case MUSHROOM:
		{
			if (isInit == false)
			{
				y += dy;
				if (Start_Y - y > BRICKITEM__BBOX)
					SetState(BRICKITEM_STATE_MOVE);
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

	case BUTTONP:
	{
		if (isPressed == true)
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				if (coObjects->at(i)->Category == CATEGORY::OBJECT && coObjects->at(i)->ObjType == OBJECT_TYPE_ITEMBRICK)
				{
					ItemBrick* itembrick = (ItemBrick*)coObjects->at(i);
					if (itembrick->Item == NORMAL)
					{
						float temp_x = itembrick->x;
						float temp_y = itembrick->y;
						itembrick->isDie = true;

						/*auto hit = new EffectHit(temp_x, temp_y);
						_PlayScene->objects.push_back(hit);*/
						Coin* coin = new Coin();
						coin->SetPosition(temp_x, temp_y);
						CAnimationSets* animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(12);
						coin->SetAnimationSet(ani_set);
						_PlayScene->objects.push_back(coin);
					}
				}
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
	//		if (Start_Y - y > BRICKITEM__BBOX)
	//			SetState(BRICKITEM_STATE_MOVE);
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

void BrickItem::Render()
{
	int ani = -1;
	switch (Item)
	{
		case MUSHROOM:
			ani = BRICKITEM_MUSHROOM_ANI;
			break;

		case BUTTONP:
		{
			if (isPressed == false)
				ani = BUTTONP_ANI_NORMAL;
			else
				ani = BUTTONP_ANI_PRESS;
		}break;

	}

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();

}

void BrickItem::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICKITEM_STATE_INIT:
	{
		switch (Item)
		{
		
			case MUSHROOM:
				vy = -MUSHROOM_SPEED_Y;
				break;

			case BUTTONP:
				isPressed = false;
				break;
		}

	}
	break;

	case BRICKITEM_STATE_MOVE:
	{
		isInit = true;
		if (Item == MUSHROOM)
		{
			if (nx == 1)
				vx = MUSHROOM_SPEED_X;
			else
				vx = -MUSHROOM_SPEED_X;
		}
	}
	break;
	case BRICKITEM_STATE_MOVE_RIGHT:
	{
		isInit = true;
		if (Item == MUSHROOM)
		{
			nx = 1;
			vx = MUSHROOM_SPEED_X;

		}
		
	}
	break;
	case BRICKITEM_STATE_MOVE_LEFT:
	{
		isInit = true;
		if (Item == MUSHROOM)
		{
			nx = -1;
			vx = -MUSHROOM_SPEED_X;

		}
		
	}
	break;

	case BRICKITEM_STATE_COLLISION:
	{
		switch (Item)
		{
		case BUTTONP:
		{
			if (isPressed == false)
			{
				isPressed = true;
				y = y + (BUTTONP_BBOX_HEIGHT - BUTTONP_PRESS_BBOX_HEIGHT);
			}
		}
		}

	}
	break;
	}
}

void BrickItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BRICKITEM__BBOX;
	switch (Item)
	{
		case MUSHROOM:
		{
			bottom = y + BRICKITEM__BBOX;
		}break;

		case BUTTONP:
		{
			if (isPressed == false)
			{
				bottom = top + BUTTONP_BBOX_HEIGHT;
			}
			else
			{
				bottom = y + BUTTONP_PRESS_BBOX_HEIGHT;
			}
		}break;
	}
	
}

void BrickItem::CaclVx(int objx)
{
	//if (objx > x + (BRICKITEM__BBOX /3) )
	if (objx < x)
		nx = 1;
	else
		nx = -1;
}
