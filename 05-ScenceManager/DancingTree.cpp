#include "DancingTree.h"

void DancingTree::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DANCINGTREE_BBOX;
	bottom = y + DANCINGTREE_BBOX;
}

void DancingTree::Render()
{
	animation_set->at(0)->Render(x, y);
}

DancingTree::DancingTree()
{
	ObjType = OBJECT_TYPE_DANCINGTREE;
	Category = CATEGORY::OBJECT;
	CurAnimation = CAnimations::GetInstance()->Get(10);
}
