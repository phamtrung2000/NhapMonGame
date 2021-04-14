#pragma once

#include "GameObject.h"
#define EFFECTSCORE_APPEARTIME 800
#define EFFECTSCORE_ANI_SET_ID 1102
#define EFFECTSCORE_SPEED_Y		0.04f

#define EFFECTSCORE_ANI_100		0
#define EFFECTSCORE_ANI_200		1
#define EFFECTSCORE_ANI_400		2
#define EFFECTSCORE_ANI_800		3
#define EFFECTSCORE_ANI_1000	4
#define EFFECTSCORE_ANI_2000	5
#define EFFECTSCORE_ANI_4000	6
#define EFFECTSCORE_ANI_8000	7
#define EFFECTSCORE_ANI_1UP		8


class EffectScore : public CGameObject
{
public:
	ULONGLONG AppearTime;
	int score;
	EffectScore(float x, float y,int Score)
	{
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(EFFECTSCORE_ANI_SET_ID);
		this->SetAnimationSet(ani_set);
		this->x = x;
		this->y = y;
		Category = CATEGORY::EFFECT;
		AppearTime = GetTickCount64();
		vy = -EFFECTSCORE_SPEED_Y;
		score = Score;
	}
	~EffectScore() {}

	void Render()
	{
		int ani = EFFECTSCORE_ANI_100;
		switch (score)
		{
		case 100:
			ani = EFFECTSCORE_ANI_100; break;
		case 200:
			ani = EFFECTSCORE_ANI_200; break;
		case 400:
			ani = EFFECTSCORE_ANI_400; break;
		case 800:
			ani = EFFECTSCORE_ANI_800; break;
		case 1000:
			ani = EFFECTSCORE_ANI_1000; break;
		case 2000:
			ani = EFFECTSCORE_ANI_2000; break;
		case 4000:
			ani = EFFECTSCORE_ANI_4000; break;
		case 8000:
			ani = EFFECTSCORE_ANI_8000; break;
		case 10000:
			ani = EFFECTSCORE_ANI_1UP; break;
		/*default:
			ani = EFFECTSCORE_ANI_1UP; break;*/
		}

		animation_set->at(ani)->Render(x, y);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		CGameObject::Update(dt);
		y += dy;
		if (GetTickCount64() - AppearTime >= EFFECTSCORE_APPEARTIME)
			this->canDelete = true;
	}
};