#include "Card.h"
#include "PlayScence.h"

Card::Card() : Item()
{
	TypeItem = CARD_STAR;
	Width = CARD_WIDTH;
	Height = CARD_HEIGHT;
	ObjType = OBJECT_TYPE_CARD;
	timeChange = GetTickCount64();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(19);
	this->SetAnimationSet(ani_set);
	hasItem = true;
	SetState(CARD_STATE_NORMAL);
	Category = CATEGORY::ITEM;
}

void Card::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (hasItem == true)
	{
		if (timeChange != 0 && GetTickCount64() - timeChange > TIME_CHANGE)
		{
			timeChange = GetTickCount64();
			if (TypeItem >= CARD_MUSHROOM)
			{
				TypeItem = CARD_STAR;
			}
			else
				TypeItem++;
		}
	}
	else
		TypeItem = CARD_EMPTY;
}

void Card::Render()
{
	int ani = 0;
	switch (TypeItem)
	{
	case  CARD_EMPTY:
		ani = CARD_EMPTY_ANI; break;
	case  CARD_STAR	:
		ani = CARD_STAR_ANI; break;
	case CARD_FLOWER:
		ani = CARD_FLOWER_ANI; break;
	case CARD_MUSHROOM	:
		ani = CARD_MUSHROOM_ANI; break;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void Card::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CARD_STATE_NORMAL:
	{
		hasItem = true;
	}
	break;

	case CARD_STATE_EMPTY:
	{
		hasItem = false;
	}break;

	}
}

void Card::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + Height;
}
