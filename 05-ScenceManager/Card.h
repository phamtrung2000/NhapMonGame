#pragma once
#include "Item.h"

#define CARD_EMPTY		0
#define CARD_STAR		1
#define CARD_FLOWER		2
#define CARD_MUSHROOM	3


#define TIME_CHANGE 500
#define CARD_WIDTH	30
#define CARD_HEIGHT	28

#define CARD_EMPTY_ANI		0
#define CARD_STAR_ANI		1
#define CARD_FLOWER_ANI		2
#define CARD_MUSHROOM_ANI	3


#define CARD_STATE_NORMAL		0
#define CARD_STATE_EMPTY		1

class Card : public Item
{
	ULONGLONG timeChange;
	bool hasItem;
public:
	Card();
	~Card() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

