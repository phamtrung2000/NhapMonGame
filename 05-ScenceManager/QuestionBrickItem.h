#include "Item.h"

#define MONEY							0
#define QUESTIONBRICKITEM_MONEY_ANI		0
#define MONEY_GRAVITY					0.0007f
#define MONEY_SPEED_Y					0.3f

#define MUSHROOM						1
#define QUESTIONBRICKITEM_MUSHROOM_ANI	1
#define MUSHROOM_GRAVITY				0.0007f
#define MUSHROOM_SPEED_X				0.05f 
#define MUSHROOM_SPEED_Y				0.02f 

#define LEAF							2
#define LEAF_ANI_LEFT					2
#define LEAF_ANI_RIGHT					3
#define LEAF_GRAVITY					0.00002f
#define LEAF_SPEED_X					0.03f 
#define LEAF_SPEED_Y					0.1f

#define QUESTIONBRICKITEM__BBOX			16
#define MONEY_BBOX_WIDTH				8

#define QUESTIONBRICKITEM_STATE_INIT	0
#define	QUESTIONBRICKITEM_STATE_MOVE_RIGHT	1
#define	QUESTIONBRICKITEM_STATE_MOVE_LEFT	2

#define QUESTIONBRICKITEM__SCORE		1000
#define QUESTIONBRICKITEM_MONEY__SCORE		100

class QuestionBrickItem : public Item
{
public:
	int Item;
	ULONGLONG AppearTime;
	QuestionBrickItem(int item,float x = 0.0f, float y = 0.0f);
	~QuestionBrickItem() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

