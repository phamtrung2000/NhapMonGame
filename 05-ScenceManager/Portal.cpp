#include "Portal.h"
#include "PlayScence.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id )
{
	ObjType = OBJECT_TYPE_PORTAL;
	this->scene_id = scene_id;
	x = l; 
	y = t;
	Width = (int)r;
	Height = (int)b;
	Category = CATEGORY::OBJECT;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(1);
	this->SetAnimationSet(ani_set);
}

void CPortal::Render()
{
	
}

void CPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}