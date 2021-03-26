#include "Goomba.h"
#include "WarpPipe.h"
#include "Block.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Koopas.h"
#include "FireBullet.h"
#include "Ground.h"
#include "Game.h"

Goomba::Goomba() : Enemy()
{
	ObjType = OBJECT_TYPE_GOOMBA;
	EnemyType = ENEMY_TYPE_GOOMBA;
	SetState(ENEMY_STATE_WALKING_LEFT);
}

void Goomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != ENEMY_STATE_DIE_IS_ATTACKED)
	{
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == ENEMY_STATE_DIE_IS_JUMPED)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	if (GetState() != ENEMY_STATE_DIE_IS_JUMPED)
		vy += GOOMBA_GRAVITY * dt;

	if (state == ENEMY_STATE_DIE_IS_JUMPED)
	{
		if (TimeToDie <= GOOMBA_TIMETODIE)
			TimeToDie++;
		else
			canDelete = true;
	}
	else if (state == ENEMY_STATE_DIE_IS_ATTACKED)
	{
		if(y > _Map->GetHeight())
			canDelete = true;
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

	if (state != ENEMY_STATE_DIE_IS_JUMPED)
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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj)
			{
				switch (e->obj->Category)
				{
				case CATEGORY::GROUND:
				{
					if (dynamic_cast<Ground*>(e->obj))
					{
						if (ny != 0) vy = 0;
						if (e->ny < 0)
						{
							x += min_tx * dx + nx * 0.4f;
						}
						else if (e->nx != 0)
						{
							//y += min_ty * dy + ny * 0.4f;
							if (GetState() == ENEMY_STATE_WALKING_RIGHT)
								SetState(ENEMY_STATE_WALKING_LEFT);
							else
								SetState(ENEMY_STATE_WALKING_RIGHT);
						}
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

				case CATEGORY::PLAYER:
					CollisionWithPlayer(e, min_tx, min_ty, nx, ny);
					break;
				
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"vy = %f, dy = %f, state = %i\n", vy, dy, state);
}

void Goomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == ENEMY_STATE_DIE_IS_JUMPED)
	{
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == ENEMY_STATE_DIE_IS_ATTACKED)
	{
		ani = GOOMBA_ANI_DIE_2;
	}
	animation_set->at(ani)->Render(x, y);
}

void Goomba::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
		case ENEMY_STATE_DIE_IS_JUMPED:
		{
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		}
		break;

		case ENEMY_STATE_DIE_IS_ATTACKED:
		{
			vy = -GOOMBA_DIE_DEFLECT_SPEED;
		}
		break;

		case ENEMY_STATE_WALKING_RIGHT:
		{
			vx = GOOMBA_WALKING_SPEED;
		}
		break;

		case ENEMY_STATE_WALKING_LEFT:
		{
			vx = -GOOMBA_WALKING_SPEED;
		}
		break;
	}
}

void Goomba::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	x += dx;
	this->y += min_ty * dy + ny * 0.1f - 0.5f;
}

void Goomba::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (dynamic_cast<Block*>(e->obj))
	{
		this->vx = this->nx * GOOMBA_WALKING_SPEED;
		x += dx;
	}
	else
	{
		if (e->nx != 0)
		{
			this->nx = -this->nx;
			this->vx = this->nx * GOOMBA_WALKING_SPEED;
			x += dx;
		}
		else
		{
			x += dx;
			this->y += min_ty * dy + ny * 0.1f - 0.5f;
		}
	}
}

void Goomba::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Enemy::CollisionWithItem(e, min_tx, min_ty, nx, ny);
}

void Goomba::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Enemy::CollisionWithWeapon(e, min_tx, min_ty, nx, ny);
}

void Goomba::CollisionWithPlayer(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Enemy::CollisionWithPlayer(e, min_tx, min_ty, nx, ny);
}

//#include "Goomba.h"
//#include "WarpPipe.h"
//#include "QuestionBrick.h"
//#include "Block.h"
//
//#define GRAVITY				0.0007f
//
//Goomba::Goomba()
//{
//	SetState(GOOMBA_STATE_WALKING);
//	ObjType = 2;
//}
//
//void Goomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
//{
//	left = x;
//	top = y;
//	right = x + GOOMBA_BBOX_WIDTH;
//
//	if (state == ENEMY_STATE_DIE_IS_JUMPED)
//		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
//	else
//		bottom = y + GOOMBA_BBOX_HEIGHT;
//}
//
//void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
//{
//	CGameObject::Update(dt, coObjects);
//	//
//	// TO-DO: make sure Goomba can interact with the world and to each of them too!
//	// 
//	if (state != ENEMY_STATE_DIE_IS_JUMPED)
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
//void Goomba::Render()
//{
//	int ani = GOOMBA_ANI_WALKING;
//	if (state == ENEMY_STATE_DIE_IS_JUMPED) {
//		ani = GOOMBA_ANI_DIE;
//	}
//
//	animation_set->at(ani)->Render(x, y);
//
//	//RenderBoundingBox();
//}
//
//void Goomba::SetState(int state)
//{
//	CGameObject::SetState(state);
//	switch (state)
//	{
//	case ENEMY_STATE_DIE_IS_JUMPED:
//		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
//		vx = 0;
//		vy = 0;
//		break;
//	case GOOMBA_STATE_WALKING:
//		vx = GOOMBA_WALKING_SPEED;
//	}
//}
