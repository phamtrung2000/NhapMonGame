#include "GameObject.h"
#include "Utils.h"
#include "Block.h"
#include "Goomba.h"
#include "Brick.h"
#include "WarpPipe.h"

#define MUSHROOM						1
#define QUESTIONBRICKITEM_MUSHROOM_ANI	0
#define MUSHROOM_GRAVITY				0.0007f
#define MUSHROOM_SPEED_X				0.04f 
#define MUSHROOM_SPEED_Y				0.02f 

#define LEAF							2
#define LEAF_ANI_LEFT					2
#define LEAF_ANI_RIGHT					3
#define LEAF_GRAVITY					0.00002f
#define LEAF_SPEED_X					0.03f 
#define LEAF_SPEED_Y					0.1f

#define QUESTIONBRICKITEM__BBOX			16
#define MONEY_BBOX_WIDTH				8
#define QUESTIONBRICKITEM_STATE_INIT	1
#define	QUESTIONBRICKITEM_STATE_MOVE	2
#define	QUESTIONBRICKITEM_STATE_MOVE_RIGHT	3
#define	QUESTIONBRICKITEM_STATE_MOVE_LEFT	4

class BrickItem : public CGameObject
{
public:
	float Start_X;
	float Start_Y;
	int Item;
	int Time;
public:
	BrickItem(int item, float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void CaclVx(int objx);
};