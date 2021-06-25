#include "Item.h"

#define MUSHROOM						1
#define BUTTONP							2
#define BRICKITEM_MONEY					3
#define LEAF							4

#define BRICKITEM_STATE_INIT	0
#define	BRICKITEM_STATE_MOVE_RIGHT	1
#define	BRICKITEM_STATE_MOVE_LEFT	2
#define BRICKITEM_STATE_COLLISION	3

#define BRICKITEM_ANISET_ID	14

#define BRICKITEM_MUSHROOM_ANI	0
#define MUSHROOM_GRAVITY				0.0007f
#define MUSHROOM_SPEED_X				0.05f 
#define MUSHROOM_SPEED_Y				0.02f 


#define	BUTTONP_ANI_NORMAL		1
#define	BUTTONP_ANI_PRESS		2

#define	BUTTONP_BBOX_WIDTH	16
#define BUTTONP_BBOX_HEIGHT	16	
#define BUTTONP_PRESS_BBOX_HEIGHT	6

#define BRICKITEM_MONEY_ANI		3
#define MONEY_GRAVITY					0.0007f
#define MONEY_SPEED_Y					0.3f

#define LEAF_ANI_LEFT					4
#define LEAF_ANI_RIGHT					5
#define LEAF_GRAVITY					0.00002f
#define LEAF_SPEED_X					0.03f 
#define LEAF_SPEED_Y					0.1f

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