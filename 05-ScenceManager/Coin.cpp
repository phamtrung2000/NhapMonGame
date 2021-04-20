#include "Coin.h"
#include "PlayScence.h"
#include "ItemBrick.h"
#include "EffectSmoke.h"

Coin::Coin() : Item()
{
	Category = CATEGORY::ITEM;
	TypeItem = ITEM_TYPE_COIN;

	ObjType = OBJECT_TYPE_COIN;

	isBrickToCoin = false;
	AppearTime = 0;
	Score = COIN_SCORE;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(12);
	this->SetAnimationSet(ani_set);
}

void Coin::Render()
{
	animation_set->at(COIN_ANI)->Render(x, y);
}

void Coin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}

Coin::Coin(float x, float y)
{
	ObjType = OBJECT_TYPE_COIN;
	vx = vy = 0.0f;
	Category = CATEGORY::ITEM;
	isBrickToCoin = false;
	SetPosition(x, y);
	AppearTime = 0;
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isBrickToCoin == true && GetTickCount64() - AppearTime >= COIN_APPEARTIME)
	{
		ItemBrick* itembrick = new ItemBrick(NORMAL, this->x, this->y);
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ITEMBRICK_ANISET_ID);
		itembrick->SetAnimationSet(ani_set);
		_Grid->AddStaticObject(itembrick, this->x, this->y);

		this->canDelete = true;
	}
}
