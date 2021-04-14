#include "Item.h"
#include "PlayScence.h"

Item::Item() : CGameObject()
{
	Category = CATEGORY::ITEM;
	TypeItem = -1;
	Start_X = Start_Y = 0;
	Score = 0;
	OnGround = false;
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void Item::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	x += dx;
	if (e->ny < 0)
		y += dy;
	else
		this->y += min_ty * dy + ny * 0.1f - 0.5f;
}

void Item::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj)
	{
		if (e->nx != 0)
		{
			if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
			{
				x += dx;
				y += min_ty * dy + ny * 0.4f;
			}
			else //if (e->obj->ObjType == OBJECT_TYPE_WARPPIPE || e->obj->ObjType == OBJECT_TYPE_QUESTIONBRICK || e->obj->ObjType == OBJECT_TYPE_ITEMBRICK || e->obj->ObjType == OBJECT_TYPE_BRICK)
			{
				y += min_ty * dy + ny * 0.2f;
				if (GetState() == ITEM_STATE_MOVE_RIGHT)
					SetState(ITEM_STATE_MOVE_LEFT);
				else
					SetState(ITEM_STATE_MOVE_RIGHT);
			}
		}
		else if (e->ny != 0)
		{
			x += min_tx * dx + nx * 0.4f;
			if (OnGround == false)
			{
				y += min_ty * dy + ny * 0.1f - 0.3f;
				OnGround = true; // xử lý chạm đất
			}
			if (ny != 0) vy = 0;
		}
	}
}

void Item::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	x += dx;
	if (e->ny < 0)
		y += dy;
	else
		this->y += min_ty * dy + ny * 0.1f - 0.5f;
}

void Item::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	x += dx;
	if (e->ny < 0)
		y += dy;
	else
		this->y += min_ty * dy + ny * 0.1f - 0.5f;
}
