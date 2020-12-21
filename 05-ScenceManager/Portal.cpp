#include "Portal.h"
#include "PlayScence.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id )
{
	ObjType = OBJECT_TYPE_PORTAL;
	this->scene_id = scene_id;
	x = l; 
	y = t;
	Width = r;
	Height = b;
	Category = CATEGORY::PORTAL;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}