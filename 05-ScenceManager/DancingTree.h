#include "GameObject.h"
#include "WorldMap.h"

#define DANCINGTREE_BBOX 16

class DancingTree : public CGameObject
{
	CAnimation* CurAnimation;
public:
	DancingTree();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
};

