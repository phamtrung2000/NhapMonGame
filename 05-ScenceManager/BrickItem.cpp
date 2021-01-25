#include "BrickItem.h"
#include "ItemBrick.h"
#include "Coin.h"
#include "EffectSmoke.h"
#include "PlayScence.h"
#include "Item.h"
#include "Brick.h"
#include "WarpPipe.h"
#include "Goomba.h"
#include "Block.h"
#include "EffectScore.h"

#define BRICKITEM_ANISET_ID	14

#define BRICKITEM_MUSHROOM_ANI	0
#define MUSHROOM_GRAVITY				0.0007f
#define MUSHROOM_SPEED_X				0.05f 
#define MUSHROOM_SPEED_Y				0.02f 


#define	BUTTONP_ANI_NORMAL		1
#define	BUTTONP_ANI_PRESS		2

#define	BUTTONP_BBOX_WIDTH	16
#define BUTTONP_BBOX_HEIGHT	16	
#define BUTTONP_PRESS_BBOX_HEIGHT	6

#define BRICKITEM_MONEY_ANI		3
#define MONEY_GRAVITY					0.0007f
#define MONEY_SPEED_Y					0.3f


BrickItem::BrickItem(int item, float x, float y) : Item()
{
	TypeItem = ITEM_TYPE_BRICKITEM;
	Start_X = x;
	Start_Y = y;
	switch (item)
	{
	case BRICKITEM_MONEY:
		Score = 100; break;
	case MUSHROOM:
		Score = 1000; break;
	}

	ChangeToCoin = isPressed = isInit = false;
	Item = item;
	ObjType = OBJECT_TYPE_BRICKITEM;
	
	this->x = x;
	this->y = y;
	SetState(BRICKITEM_STATE_INIT);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BRICKITEM);
	this->SetAnimationSet(ani_set);
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT;

	if (item == BRICKITEM_MONEY)
		AppearTime = GetTickCount64();
	else
		AppearTime = 0;


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
				if (Start_Y - y > Height)
				{
					if(this->x <=_Mario->x)
						SetState(BRICKITEM_STATE_MOVE_LEFT);
					else
						SetState(BRICKITEM_STATE_MOVE_RIGHT);
				}
					
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

					for (UINT i = 0; i < coEventsResult.size(); i++)
					{
						LPCOLLISIONEVENT e = coEventsResult[i];

						if (e->obj)
						{
							switch (e->obj->Category)
							{
								case CATEGORY::GROUND:
								{
									if (ny != 0) vy = 0;
									if (e->ny < 0)
									{
										x += min_tx * dx + nx * 0.4f;
										if (OnGround == false)
										{
											y += min_ty * dy + ny * 0.1f - 0.3f;
											OnGround = true; // xử lý chạm đất
										}
									}
									else if (e->nx != 0)
									{
										y += min_ty * dy + ny * 0.1f - 0.3f;
										if (GetState() == BRICKITEM_STATE_MOVE_RIGHT)
											SetState(BRICKITEM_STATE_MOVE_LEFT);
										else
											SetState(BRICKITEM_STATE_MOVE_RIGHT);
									}
								}
								break;

								case CATEGORY::OBJECT:
									CollisionWithObject(e, min_tx, min_ty, nx, ny);
									break;
								case CATEGORY::ENEMY:
									CollisionWithEnemy(e, min_tx, min_ty, nx, ny);
									break;

								case CATEGORY::ITEM:
									CollisionWithItem(e, min_tx, min_ty, nx, ny);
									break;

								case CATEGORY::WEAPON:
									CollisionWithWeapon(e, min_tx, min_ty, nx, ny);
									break;

							}
						}
					}
				}

				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
			}
		}break;

		case BUTTONP:
		{
			if (isPressed == true && ChangeToCoin == false)
			{
				for (UINT i = 0; i < coObjects->size(); i++)
				{
					if (coObjects->at(i)->Category == CATEGORY::OBJECT && coObjects->at(i)->ObjType == OBJECT_TYPE_ITEMBRICK)
					{
						ItemBrick* itembrick = (ItemBrick*)coObjects->at(i);
						if (itembrick->Item == NORMAL)
						{
						
							itembrick->canDelete = true;
							/*auto smoke = new EffectSmoke(itembrick->x, itembrick->y);
							smoke->AmountTimeAppear = 2 * EFFECTSMOKE_APPEARTIME;
							_PlayScene->objects.push_back(smoke);*/
						
							Coin* coin = new Coin();
							coin->SetPosition(itembrick->x, itembrick->y);
							coin->isBrickToCoin = true;
							coin->AppearTime = GetTickCount64();
							CAnimationSets* animation_sets = CAnimationSets::GetInstance();
							LPANIMATION_SET ani_set = animation_sets->Get(12);
							coin->SetAnimationSet(ani_set);
							_PlayScene->objects.push_back(coin);
						}
					}
				}
				ChangeToCoin = true;
			}
		}break;

		case BRICKITEM_MONEY:
		{
			vy += MONEY_GRAVITY * dt;
			if (GetTickCount64() - AppearTime >= 800)
			{
				canDelete = true;
				EffectScore* Score = new EffectScore(this->x, this->y, this->Score);
				_PlayScene->objects.push_back(Score);
				_HUD->UpdateScore(this, 0);
			}
			y += dy;

		}break;
	}
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

		case BRICKITEM_MONEY:
			ani = BRICKITEM_MONEY_ANI;
			break;
	}

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();

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

			case BRICKITEM_MONEY:
				vy = -MONEY_SPEED_Y;
				break;
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
	right = x + OBJECT_BBOX_WIDTH_HEIGHT;
	switch (Item)
	{
		case MUSHROOM:
		{
			bottom = y + OBJECT_BBOX_WIDTH_HEIGHT;
		}break;

		case BUTTONP:
		{
			if (isPressed == false)
			{
				bottom = y + OBJECT_BBOX_WIDTH_HEIGHT;
			}
			else
			{
				bottom = y + BUTTONP_PRESS_BBOX_HEIGHT;
			}
		}break;
	}
	
}

