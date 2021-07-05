#pragma once
#include "MusicBrick.h"

#define MUSICBRICK_STATE_HIDDEN	2
#define MUSICBRICK_STATE_APPEAR	3

class HiddenMusicBrick : public MusicBrick
{
public:
	bool isHidden; // mới vô sẽ ẩn, tác động vào thì mới xuất hiện

	HiddenMusicBrick(float x = 0.0f, float y = 0.0f);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

