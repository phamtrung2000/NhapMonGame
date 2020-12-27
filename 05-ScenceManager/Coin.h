#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH  8
#define COIN_BBOX_HEIGHT 16
#define COIN_ANI		 0
//#define COIN_SCORE		50	
#define COIN_APPEARTIME 15000


class Coin : public CGameObject
{
public:
	// các biến hỗ trợ việc tiền biến thành gạch lại như cũ
	bool isBrickToCoin;
	ULONGLONG AppearTime;


	Coin();
	Coin(float x,float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};