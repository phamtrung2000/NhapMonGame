#include "MenuOption.h"

MenuOption::MenuOption() 
{
	is1Player = true;
	ObjType = 0;
}


void MenuOption::Render()
{
	int ani = -1;
	if (is1Player == true)
		ani = 0;
	else
		ani = 1;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void MenuOption::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + OPTION_WIDTH;
	bottom = top + OPTION_HEIGHT;
}