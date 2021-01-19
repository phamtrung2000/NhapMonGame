#include "Item.h"

#define MUSHROOM						1
#define BRICKITEM_MUSHROOM_ANI	0
#define MUSHROOM_GRAVITY				0.0007f
#define MUSHROOM_SPEED_X				0.05f 
#define MUSHROOM_SPEED_Y				0.02f 

#define BUTTONP							2
#define	BUTTONP_ANI_NORMAL		1
#define	BUTTONP_ANI_PRESS		2
#define	BUTTONP_BBOX_WIDTH	16
#define BUTTONP_BBOX_HEIGHT	16	
#define BUTTONP_PRESS_BBOX_HEIGHT	6

#define BRICKITEM__BBOX			16

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

	BrickItem(int item, float x = 0.0f, float y = 0.0f);
	~BrickItem() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};