#pragma once
#include "GameObject.h"

#define OPTION_WIDTH  112
#define OPTION_HEIGHT 24

class MenuOption : public CGameObject
{
public:
	bool is1Player;
public:
	MenuOption();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};