#pragma once
#include "Brick.h"

#define BRICK_SPEED_Y	0.05f
#define MAX_HIGH	8

#define MUSICBRICK_STATE_NORMAL		0
#define MUSICBRICK_STATE_COLLISION	1


class MusicBrick : public Brick
{
public:
	bool isCollision, isCollisionMario;
	int direction; // Hướng va chạm -> hướng mà gạch di chuyển, musicbrick chỉ có lên xuống, hidden thì đủ 4 hướng
	// -1 : left, -2 : top , 1 : right, -2 : bottom

	MusicBrick(float x = 0.0f, float y = 0.0f);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

