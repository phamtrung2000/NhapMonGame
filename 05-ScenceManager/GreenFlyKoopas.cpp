#include "GreenFlyKoopas.h"
#include "FireBullet.h"
#include "Block.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Mario.h"
#include "Ground.h"
#include "GreenKoopas.h"
#include "PlayScence.h"
#include "ItemBrick.h"

GreenFlyKoopas::GreenFlyKoopas() : GreenKoopas()
{
	ObjType = OBJECT_TYPE_GREENFLYKOOPAS;
	Health = 2;
}

GreenFlyKoopas::~GreenFlyKoopas()
{
}

void GreenFlyKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GREENKOOPAS_BBOX_WIDTH;
	if (isShell == true || isShell_2 == true)
		bottom = y + GREENKOOPAS_BBOX_HEIGHT_SHELL;
	else
		bottom = y + GREENKOOPAS_BBOX_HEIGHT;
	if (isHold == true)
	{
		left = left - 1.f;
		right = right - 1.f;
	}
}

void GreenFlyKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GreenKoopas::Update(dt, coObjects);
}

void GreenFlyKoopas::Render()
{
	int ani = 0;
	if (Health == 2)
	{
		if (nx == LEFT)
			ani = GREENFLYKOOPAS_ANI_WALKING_LEFT;
		else
			ani = GREENFLYKOOPAS_ANI_WALKING_RIGHT;
		animation_set->at(ani)->Render(x, y);
	}
	else
	{
		GreenKoopas::Render();
	}
}

void GreenFlyKoopas::SetState(int state)
{
	GreenKoopas::SetState(state);
}

void GreenFlyKoopas::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (Health == 1)
	{
		GreenKoopas::CollisionWithEnemy(e, min_tx, min_ty, nx, ny);
	}
	else
	{
		if (e->nx != 0)
		{
			x += dx;
			y += min_ty * dy + ny * 0.4f;
		}
		else if (e->ny < 0)
		{
			x += min_tx * dx + nx * 0.4f;
			y += dy;
		}
	}
}

void GreenFlyKoopas::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	GreenKoopas::CollisionWithObject(e, min_tx, min_ty, nx, ny);
	if (Health == 2)
	{
		if (e->ny < 0)
			vy = -ENEMY_DIE_DEFLECT_SPEED;
	}
}

void GreenFlyKoopas::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	GreenKoopas::CollisionWithItem(e, min_tx, min_ty, nx, ny);
}

void GreenFlyKoopas::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	GreenKoopas::CollisionWithWeapon(e, min_tx, min_ty, nx, ny);
}

