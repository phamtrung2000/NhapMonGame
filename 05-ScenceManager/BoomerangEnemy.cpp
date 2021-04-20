#include "BoomerangEnemy.h"
#include "PlayScence.h"
#include "BoomerangWeapon.h"

#define BOOMERANGENEMY_DIE_DEFLECT_SPEED	0.25f
#define BOOMERANGENEMY_WALKING_SPEED	0.05f
#define	BOOMERANGENEMY_GRAVITY			0.0007f
#define BOOMERANG_JUMP_DEFLECT_SPEED 0.6f

#define BOOMERANGENEMY_ANI_WALKING_RIGHT	0
#define BOOMERANGENEMY_ANI_WALKING_LEFT		1
#define BOOMERANGENEMY_ANI_THROWING_RIGHT	2
#define BOOMERANGENEMY_ANI_THROWING_LEFT	3
#define BOOMERANGENEMY_ANI_DIE_RIGHT		4
#define BOOMERANGENEMY_ANI_DIE_LEFT	5

#define BOOMERANGENEMY_SCORE	100
#define BOOMERANGENEMY_TIMETODIE	20

#define BOOMERANGENEMY_WIDTH_BBOX 16
#define BOOMERANGENEMY_HEIGHT_BBOX 24
#define BOOMERANG_TIME_DIE 300



BoomerangEnemy::BoomerangEnemy() : Enemy()
{
	countThrow = 0;
	ObjType = OBJECT_TYPE_BOOMERANGENEMY;
	EnemyType = ENEMY_TYPE_BOOMERANG_ENEMY;
	CanThrow = false;
	Width = BOOMERANGENEMY_WIDTH_BBOX;
	Height = BOOMERANGENEMY_HEIGHT_BBOX;
	TimeToMove = GetTickCount64();
	TimeToJump = GetTickCount64();
	TimeToThrow = 0;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BOOMERANGENEMY);
	this->SetAnimationSet(ani_set);
}

BoomerangEnemy::BoomerangEnemy(float x, float y) : Enemy()
{
	countThrow = 0;
	ObjType = OBJECT_TYPE_BOOMERANGENEMY;
	CanThrow = false;
	Width = BOOMERANGENEMY_WIDTH_BBOX;
	Height = BOOMERANGENEMY_HEIGHT_BBOX;
	StartX = x;
	StartY = y;
	TimeToMove = GetTickCount64();
	TimeToJump = GetTickCount64();
	TimeToThrow = 300;
	EnemyType = ENEMY_TYPE_BOOMERANG_ENEMY;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BOOMERANGENEMY);
	this->SetAnimationSet(ani_set);
	weapon = NULL;
}

void BoomerangEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + Width;
	bottom = y + Height;
}

void BoomerangEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	{
		Enemy::Update(dt, coObjects);
		if (isDisappear == false)
		{
			//if (GetState() != BOOMERANGENEMY_STATE_DIE)
			vy += BOOMERANGENEMY_GRAVITY * dt;

			if (state == ENEMY_STATE_DIE_IS_JUMPED || state == ENEMY_STATE_DIE_IS_ATTACKED)
			{
				x += dx;
				y += dy;
				weapon->canDelete = true;
				if (y > _Map->GetHeight() + 50)
				{
					canDelete = true;
				}
				return;
			}

			if (vx < 0 && x < 0)
			{
				x = 0;
				vx = -vx;
				nx = -nx;
			}
			TimeToThrow++;
			if (_Mario->x < this->x)
			{
				nx = LEFT;
				if (TimeToMove != 0 && GetTickCount64() - TimeToMove > 1500 && abs(_Mario->x - this->x) >= 20)
				{
					vx = -vx;
					TimeToMove = GetTickCount64();
				}

				if (TimeToJump != 0 && GetTickCount64() - TimeToJump > 3000)
				{
					SetState(ENEMY_STATE_JUMPING_HIGH_RIGHT);
					TimeToJump = GetTickCount64();
				}

				if (TimeToThrow != 0 && TimeToThrow > 400)
				{
					SetState(BOOMERANGENEMY_STATE_THROWING_LEFT);
					TimeStartThrow = GetTickCount64();
					TimeToThrow = 0;
				}
			}
			else
			{
				nx = RIGHT;
				if (TimeToMove != 0 && GetTickCount64() - TimeToMove > 1500 && abs(_Mario->x - this->x) >= 20)
				{
					vx = -vx;
					TimeToMove = GetTickCount64();
				}

				if (TimeToJump != 0 && GetTickCount64() - TimeToJump > 3000)
				{
					SetState(ENEMY_STATE_JUMPING_HIGH_LEFT);
					TimeToJump = GetTickCount64();
				}

				if (TimeToThrow != 0 && TimeToThrow > 400)
				{
					SetState(BOOMERANGENEMY_STATE_THROWING_RIGHT);
					TimeStartThrow = GetTickCount64();
					TimeToThrow = 0;
				}
			}

			if (CanThrow == true)
			{
				if (isThrow == false)
				{
					CreateBoomerang();
					isThrow = true;
				}
				if (isThrow == true)
				{
					ULONGLONG  a = GetTickCount64() - TimeStartThrow;
					weapon->SetPosition(this->x - 2, this->y - 5);
					weapon->SetSpeed(0, 0);
					if (TimeStartThrow != 0 && a > 1000)
					{
						weapon->SetSpeed(this->nx * 0.1f, -0.015f);
						isThrow = true;
						TimeStartThrow = 0;
						if (nx == RIGHT)
							SetState(ENEMY_STATE_WALKING_RIGHT);
						else
							SetState(ENEMY_STATE_WALKING_LEFT);
						CanThrow = isThrow = false;

					}
				}

			}


			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;
			coEvents.clear();

			CalcPotentialCollisions(coObjects, coEvents);

			if (coEvents.size() == 0) {
				x += dx;
				y += dy;
			}
			else
			{
				float min_tx, min_ty, nx = 0, ny;
				float rdx = 0;
				float rdy = 0;

				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

				if (ny != 0) vy = 0;
				for (UINT i = 0; i < coEventsResult.size(); i++) {
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
			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
	
}


void BoomerangEnemy::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
		case BOOMERANGENEMY_STATE_THROWING_RIGHT:
		{
			CanThrow = true;
			nx = RIGHT;
		}break;

		case BOOMERANGENEMY_STATE_THROWING_LEFT:
		{
			CanThrow = true;
			nx = LEFT;
		}break;
	}

}

void BoomerangEnemy::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
}
void BoomerangEnemy::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj != NULL)
	{
		if (e->ny < 0)
		{
			OnGroud = true;
			if (GetState() == ENEMY_STATE_INIT)
				SetState(ENEMY_STATE_WALKING_RIGHT);
		}

		if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
		{
			if (e->nx != 0)
			{
				x += dx;
			}
			else if (e->ny < 0)
			{
				x += min_tx * dx + nx * 0.4f;
			}
			if (ny != 0) vy = 0;
		}
		else
		{
			if (e->nx != 0)
			{
				if (GetState() == ENEMY_STATE_WALKING_RIGHT)
					SetState(ENEMY_STATE_WALKING_LEFT);
				else if (GetState() == ENEMY_STATE_WALKING_LEFT)
					SetState(ENEMY_STATE_WALKING_RIGHT);
			}
			else
			{
				x += min_tx * dx + nx * 0.4f;
			}
			if (ny != 0) vy = 0;
		}
	}
}
void BoomerangEnemy::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	
}
void BoomerangEnemy::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj->ObjType == OBJECT_TYPE_BOOMERANG)
	{
		x += dx;
		if (e->ny < 0)
			y += dy;
		BoomerangWeapon* boomerang = dynamic_cast<BoomerangWeapon*>(e->obj);
		if (boomerang->isMarioWeapon == false)
		{
			boomerang->canDelete = true;
		}
	}
	else
	{
		x += dx;
		y += min_ty * dy + ny * 0.2f;
	}

}
void BoomerangEnemy::CreateBoomerang()
{
	/*BoomerangWeapon* boom = new BoomerangWeapon(this->nx,this->x, this->y);
	_PlayScene->objects.push_back(boom);*/
	weapon = new BoomerangWeapon(this->nx, this->x, this->y);
	weapon->SetSpeed(0, 0);
	weapon->Time = TimeToThrow;
	_Grid->AddMovingObject(weapon, this->x, this->y);
}



void BoomerangEnemy::Render()
{
	int ani = BOOMERANGENEMY_ANI_WALKING_RIGHT;
	if (isDie == true )
	{
		if (nx == RIGHT)
			ani = BOOMERANGENEMY_ANI_DIE_RIGHT;
		else
			ani = BOOMERANGENEMY_ANI_DIE_LEFT;
	}
	else
	{
		if (nx == RIGHT)
		{
			if(CanThrow == false)
				ani = BOOMERANGENEMY_ANI_WALKING_RIGHT;
			else
				ani = BOOMERANGENEMY_ANI_THROWING_RIGHT;
		}
		else 
		{
			if (CanThrow == false)
				ani = BOOMERANGENEMY_ANI_WALKING_LEFT;
			else
				ani = BOOMERANGENEMY_ANI_THROWING_LEFT;
		}
	}
	animation_set->at(ani)->Render(x, y);
	DebugOut(L"nx = %i, state = %i, ani = %i, TimeToThrow = %i \n", nx, state, ani, TimeToThrow);;
	////RenderBoundingBox();
}

