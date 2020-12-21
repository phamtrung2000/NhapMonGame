#pragma once

#include "GameObject.h"
#define EFFECTSMOKE_APPEARTIME 250
#define EFFECT_SMOKE_ANI_SET_ID 1101

#define ANI_EFFECT_SMOKE	0

class EffectHit : public CGameObject
{
public:
	ULONGLONG AppearTime;
	EffectHit(float x, float y)
	{
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SMOKE_ANI_SET_ID);
		this->SetAnimationSet(ani_set);
		this->x = x;
		this->y = y;
		Category = CATEGORY::EFFECT;
		AppearTime = GetTickCount64();
	}
	~EffectHit() {}
	void Render()
	{
		animation_set->at(ANI_EFFECT_SMOKE)->Render(x, y);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		if (GetTickCount64() - AppearTime >= EFFECTSMOKE_APPEARTIME)
			this->isDie = true;
	}
};