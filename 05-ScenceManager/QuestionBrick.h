//#pragma once
//#include "GameObject.h"
//#define ANI_BRICK_NORMAL 0
//#define	ANI_BRICK_COLLISION	1
//
//#define BRICK_SPEED_Y	0.05
//#define MAX_HIGH	5
//#define BRICK_STATE_COLLISION	0
//#define BRICK_STATE_NOTHINGLEFT 1
//class QuestionBrick :
//	public CGameObject
//{
//	float Start_Position;
//public:
//	bool isCollision;
//	QuestionBrick()
//	{
//		isCollision = false;
//		Start_Position = y;
//	}
//	void Render();
//	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
//	void SetState(int state);
//	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
//	{
//		left = x;
//		top = y;
//		right = x + 16;
//		bottom = y + 16;
//	}
//	~QuestionBrick();
//};
//

#pragma once
#include "GameObject.h"

#define BRICK_SPEED_Y	0.05
#define MAX_HIGH	5

#define BRICK_STATE_NORMAL		0
#define BRICK_STATE_COLLISION	1
#define BRICK_STATE_EMPTY		2

#define ANI_BRICK_NORMAL 0
#define	ANI_BRICK_COLLISION	1

class QuestionBrick : public CGameObject
{
public:
	float Start_Y;
	bool isCollision;
	int hasItem;
	int Item;
public:
	QuestionBrick(int item,float x = 0.0f, float y = 0.0f);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = x + 16;
		bottom = y + 16;
	}
};

