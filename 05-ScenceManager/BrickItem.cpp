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
#include "ListItemBrick.h"


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
					if (coObjects->at(i)->Category == CATEGORY::OBJECT && coObjects->at(i)->ObjType == OBJECT_TYPE_LISTITEMBRICK)
					{
						ListItemBrick* listbrick = (ListItemBrick*)coObjects->at(i);
						for (unsigned int i = 0; i < listbrick->Bricks.size();)
						{
							if (listbrick->Bricks.at(i)->Item == NORMAL)
							{
								ItemBrick* itembrick = listbrick->Bricks.at(i);
								//itembrick->canDelete = true;

								auto smoke = new EffectSmoke(itembrick->x, itembrick->y);
								smoke->AmountTimeAppear = 2 * EFFECTSMOKE_APPEARTIME;
								_Grid->AddStaticObject(smoke, itembrick->x, itembrick->y);

								Coin* coin = new Coin();
								coin->SetPosition(itembrick->x , itembrick->y);
								coin->isBrickToCoin = true;
								coin->AppearTime = GetTickCount64();
								CAnimationSets* animation_sets = CAnimationSets::GetInstance();
								LPANIMATION_SET ani_set = animation_sets->Get(12);
								coin->SetAnimationSet(ani_set);
								_Grid->AddStaticObject(coin, itembrick->x, itembrick->y);

								listbrick->Bricks.erase(listbrick->Bricks.begin() + i);
							}
							else
							{
								//i++;
								unsigned int j = i;
								ListItemBrick* listbrick1 = new ListItemBrick();
								for (j; j < listbrick->Bricks.size(); j++)
								{
									if (listbrick->Bricks.at(j)->Item == NORMAL) // viên đầu tiên k bao h là normal
									{
										break;
									}
									else
									{
										ItemBrick* itembrick = listbrick->Bricks.at(j);
										if (listbrick1->Bricks.size() == 0)
										{
											listbrick1->y = itembrick->y;
										}
										listbrick1->Bricks.push_back(itembrick);
									}
								}
								_Grid->AddStaticObject(listbrick1, listbrick1->Bricks.at(0)->x, listbrick1->Bricks.at(0)->y);
								i = j;
							}
						}
						listbrick->canDelete = true;
					}
				}
				ChangeToCoin = true;
				_Camera->Shake = true;
			}
		}break;

		case BRICKITEM_MONEY:
		{
			vy += MONEY_GRAVITY * dt;
			if (GetTickCount64() - AppearTime >= 800)
			{
				canDelete = true;
				EffectScore* Score = new EffectScore(this->x, this->y, this->Score);
				_Grid->AddMovingObject(Score, this->x, this->y);
				_HUD->UpdateScore(this, 0);
			}
			y += dy;

		}break;

		case LEAF:
		{
			x += dx;
			y += dy;
			if (isInit == false)
			{
				if (static_cast<float>(Start_Y - y) >= static_cast <float> (3.5 * OBJECT_BBOX_WIDTH_HEIGHT))
				{
					vy = 0;
					SetState(BRICKITEM_STATE_MOVE_RIGHT);
				}

			}
			else
			{
				vy += LEAF_GRAVITY * dt;
				if (x - Start_X >= OBJECT_BBOX_WIDTH_HEIGHT)
				{
					SetState(BRICKITEM_STATE_MOVE_LEFT);
				}
				else if (x - Start_X <= 0)
				{
					SetState(BRICKITEM_STATE_MOVE_RIGHT);
				}
			}
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

		case LEAF:
		{
			ani = LEAF_ANI_RIGHT;
			if (GetState() == BRICKITEM_STATE_MOVE_LEFT)
				ani = LEAF_ANI_LEFT;
			else if (GetState() == BRICKITEM_STATE_MOVE_RIGHT)
				ani = LEAF_ANI_RIGHT;
		}break;
	}

	animation_set->at(ani)->Render(x, y);
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

			case LEAF:
				vy = -LEAF_SPEED_Y;
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
		else if (Item == LEAF)
		{
			vx = LEAF_SPEED_X;
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
		else if (Item == LEAF)
		{
			vx = -LEAF_SPEED_X;
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

		default:
		{
			bottom = y + OBJECT_BBOX_WIDTH_HEIGHT;
		}break;
	}
}

