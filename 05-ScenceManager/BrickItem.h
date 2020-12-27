#include "GameObject.h"
#include "Utils.h"
#include "Block.h"
#include "Goomba.h"
#include "Brick.h"
#include "WarpPipe.h"
#include "PlayScence.h"

#define MUSHROOM						1
#define BRICKITEM_MUSHROOM_ANI	0
#define MUSHROOM_GRAVITY				0.0007f
#define MUSHROOM_SPEED_X				0.04f 
#define MUSHROOM_SPEED_Y				0.02f 

#define BUTTONP							2
#define	BUTTONP_ANI_NORMAL		1
#define	BUTTONP_ANI_PRESS		2
#define	BUTTONP_BBOX_WIDTH	16
#define BUTTONP_BBOX_HEIGHT	16	
#define BUTTONP_PRESS_BBOX_HEIGHT	6

#define BRICKITEM__BBOX			16
#define BRICKITEM_STATE_INIT	0
#define	BRICKITEM_STATE_MOVE	1
#define	BRICKITEM_STATE_MOVE_RIGHT	2
#define	BRICKITEM_STATE_MOVE_LEFT	3
#define BRICKITEM_STATE_COLLISION	4

class BrickItem : public CGameObject
{
public:
	float Start_X;
	float Start_Y;
	int Item;
	int Time;

	// BUTTON P
	bool isPressed;
public:
	BrickItem(int item, float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void CaclVx(int objx);
};