#include "QuestionBrickItem.h"
#include "PlayScence.h"
#include "EffectScore.h"

QuestionBrickItem::QuestionBrickItem(int item,float x, float y) : Item()
{
	TypeItem = ITEM_TYPE_QUESTIONBRICKITEM;
	Start_X = x;
	Start_Y = y;

	Item = item;
	ObjType = OBJECT_TYPE_QUESTIONBRICKITEM;
	
	this->x = x;
	this->y = y;
	vx = vy = 0;
	SetState(QUESTIONBRICKITEM_STATE_INIT);
	AppearTime = GetTickCount64();
	switch (item)
	{
	case MONEY:
		Score = QUESTIONBRICKITEM_MONEY__SCORE; break;
	case MUSHROOM: case LEAF:
	{
		IsMovingObject = true;
		Score = QUESTIONBRICKITEM__SCORE;
	}
	break;
	}

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(QBI_MUSHROOM_ANISET_ID);
	this->SetAnimationSet(ani_set);
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
				canDelete = true;
				EffectScore* Score = new EffectScore(this->x, this->y, this->Score);
				_Grid->AddMovingObject(Score, this->x, this->y);
				_HUD->UpdateScore(this, 0);
			}
			y += dy;
		
		}break;

		case MUSHROOM:
		{
			if (isInit == false)
			{
				y += dy;
				if (Start_Y - y > QUESTIONBRICKITEM__BBOX)
				{
					if (this->x <= _Mario->x)
						SetState(QUESTIONBRICKITEM_STATE_MOVE_LEFT);
					else
						SetState(QUESTIONBRICKITEM_STATE_MOVE_RIGHT);
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

	case QUESTIONBRICKITEM_STATE_MOVE_RIGHT:
	{
		isInit = true;
		nx = RIGHT;
		if (Item == MUSHROOM)
		{
			vx = MUSHROOM_SPEED_X;
		}
		else if (Item == LEAF)
		{
			vx = LEAF_SPEED_X;
		}
	}
	break;

	case QUESTIONBRICKITEM_STATE_MOVE_LEFT:
	{
		isInit = true;
		nx = LEFT;
		if (Item == MUSHROOM)
		{
			vx = -MUSHROOM_SPEED_X;
		}
		else if (Item == LEAF)
		{
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

