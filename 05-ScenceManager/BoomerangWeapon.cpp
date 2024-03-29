﻿#include "BoomerangWeapon.h"
#include "Camera.h"
#include "Mario.h"

#define BOOMERANG_SPEED_X 0.06f
#define BOOMERANG_SPEED_Y 0.003f
#define BOOMERANG_GRAVITY 0.0001f
#define BOOMERANG_ANI_SET_ID 102

BoomerangWeapon::BoomerangWeapon(int nx, float x, float y) : Weapon()
{
	isMarioWeapon = false;
	ObjType = OBJECT_TYPE_BOOMERANG;
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT;
	SetPosition(x, y);
	isMarioWeapon = false;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANG_ANI_SET_ID);
	this->SetAnimationSet(ani_set);
	Time = 0;
	this->nx = nx;
}
BoomerangWeapon::~BoomerangWeapon()
{

}
void BoomerangWeapon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + Width;
	b = t + Height;
}

void BoomerangWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	Time++;

	if (Time <= 150)
	{
		x += dx;
		y += dy;
	}
	else
	{
		if (Time <= 200)
		{
			// trái : -1  : + lên, phải : +1 : trừ đi
			vx -= nx * BOOMERANG_GRAVITY * dt;
			vy += BOOMERANG_GRAVITY * dt;
			x += dx;
			y += dy;
		}
		else if (Time <= 350)
		{
			vx -= nx * BOOMERANG_GRAVITY * dt;
			if (vy > 0)
				vy -= BOOMERANG_GRAVITY * dt;
			else
				vy = 0;
			x += dx;
			y += dy;
		}
		else
			canDelete = true;
		
	}
	DebugOut(L"Time = %i, vx = %f\n", Time, vx);

}

void BoomerangWeapon::Render()
{
	int ani = 0;
	if (nx == -1)
	{
		if (vx == 0 && vy == 0)
			ani = ANI_BOOMERANG_HOLD_LEFT;
		else
			ani = ANI_BOOMERANG_THROW_LEFT;
		
	}
	else
	{
		if (vx == 0 && vy == 0)
			ani = ANI_BOOMERANG_HOLD_RIGHT;
		else
			ani = ANI_BOOMERANG_THROW_RIGHT;
	}
	animation_set->at(ani)->Render(x, y);
}

