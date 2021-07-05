#pragma once
#include "GameObject.h"

#define MARIO_TAIL_WIDTH  9.0f//8.0f
#define MARIO_TAIL_HEIGHT 6.0f//2.0f

#define MARIOTAIL_ANI_0_RIGHT 0
#define MARIOTAIL_ANI_1_RIGHT 1
#define MARIOTAIL_ANI_2_RIGHT 2
#define MARIOTAIL_ANI_3_RIGHT 3
#define MARIOTAIL_ANI_4_RIGHT 4
#define MARIOTAIL_ANI_5_RIGHT 5
#define MARIOTAIL_ANI_6_RIGHT 6
#define MARIOTAIL_ANI_7_RIGHT 7

#define MARIOTAIL_ANI_0_LEFT 8
#define MARIOTAIL_ANI_1_LEFT 9
#define MARIOTAIL_ANI_2_LEFT 10
#define MARIOTAIL_ANI_3_LEFT 11
#define MARIOTAIL_ANI_4_LEFT 12
#define MARIOTAIL_ANI_5_LEFT 13
#define MARIOTAIL_ANI_6_LEFT 14
#define MARIOTAIL_ANI_7_LEFT 15
#define MARIOTAIL_ANI_INVISIBLE 16
#define MARIOTAIL_ANI_WALKING_RIGHT 17
#define MARIOTAIL_ANI_WALKING_LEFT 18
#define MARIOTAIL_ANI_FLYINGLOW_RIGHT 19
#define MARIOTAIL_ANI_FLYINGLOW_LEFT	20

#define MARIOTAIL_ANI_SET_ID 1200

class MarioTail:public CGameObject //mỗi khi mario quật đuôi thì tạo object đuôi thay 
{
public:
	bool isInvisible;
	bool attack; // biến để mario chỉ tấn công 1 vật duy nhất
	MarioTail(float, float);
	~MarioTail() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state) {}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	
};

