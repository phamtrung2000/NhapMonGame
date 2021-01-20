#pragma once

#include "GameObject.h"
#define EFFECTHIT_APPEARTIME 250
#define EFFECT_HIT_ANI_SET_ID 1101

#define ANI_EFFECT_HITTAIL			0
#define ANI_EFFECT_HITFIREBULLET	1
#define TYPE_TAIL		0
#define TYPE_FIREBULLET 1

class EffectHit : public CGameObject
{
public:
	int type;
	ULONGLONG AppearTime;
	EffectHit(float x, float y, int _type)
	{
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		type = _type;
		LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_HIT_ANI_SET_ID);
		this->SetAnimationSet(ani_set); 

		this->x = x;
		this->y = y;
		Category = CATEGORY::EFFECT;
		AppearTime = GetTickCount64();
	}
	~EffectHit() {}
	void Render()
	{
		switch (type)
		{
		case TYPE_TAIL:
			animation_set->at(ANI_EFFECT_HITTAIL)->Render(x, y); break;
		case TYPE_FIREBULLET:
			animation_set->at(ANI_EFFECT_HITFIREBULLET)->Render(x, y); break;
		}
		
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		if (GetTickCount64() - AppearTime >= EFFECTHIT_APPEARTIME)
			this->canDelete = true;
	}
};