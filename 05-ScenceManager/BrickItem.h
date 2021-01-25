#include "Item.h"

#define BRICKITEM_MONEY							3
#define MUSHROOM						1
#define BUTTONP							2


#define BRICKITEM_STATE_INIT	0
#define	BRICKITEM_STATE_MOVE_RIGHT	1
#define	BRICKITEM_STATE_MOVE_LEFT	2
#define BRICKITEM_STATE_COLLISION	3

class BrickItem : public Item
{
public:
	int Item;
	// BUTTON P
	bool isPressed, ChangeToCoin;
	ULONGLONG AppearTime;

	BrickItem(int item, float x = 0.0f, float y = 0.0f);
	~BrickItem() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};