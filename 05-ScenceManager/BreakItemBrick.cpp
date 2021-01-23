#include "BreakItemBrick.h"
#include "PlayScence.h"

#define	BREAKITEMBRICK_GRAVITY		0.0007f

#define BREAKITEMBRICK_LEFT_ANI 0
#define BREAKITEMBRICK_RIGHT_ANI 1

#define BREAKITEMBREAKITEMBRICK_ANI_SET_ID 22

BreakItemBrick::BreakItemBrick(int direction,float _x, float _y, float _vx, float _vy)
{
	ObjType = OBJECT_TYPE_BREAKITEMBRICK;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BREAKITEMBREAKITEMBRICK_ANI_SET_ID);
	this->SetAnimationSet(ani_set);
	this->nx = direction;
	vx = nx * _vx;
	vy = _vy;
	Category = CATEGORY::EFFECT;
	SetPosition(_x, _y);
	Width = Height = OBJECT_BBOX_WIDTH_HEIGHT / 2;
}

void BreakItemBrick::Render()
{
	int ani = BREAKITEMBRICK_LEFT_ANI;
	if (nx == RIGHT)
		ani = BREAKITEMBRICK_RIGHT_ANI;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void BreakItemBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + OBJECT_BBOX_WIDTH_HEIGHT/2;
	b = y + OBJECT_BBOX_WIDTH_HEIGHT/2;
}

void BreakItemBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += BREAKITEMBRICK_GRAVITY * dt;
	if (y > _Map->GetHeight())
		this->canDelete = true;
	x += dx;
	y += dy;
}
