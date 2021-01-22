#include "FlyWood.h"
#include "PlayScence.h"

FlyWood::FlyWood() : CGameObject()
{
	Width = FLYWOOD_WIDTH;
	Height = FLYWOOD_HEIGHT;
	isCollision = false;
	ObjType = OBJECT_TYPE_FLYWOOD;
	SetState(FLYWOOD_STATE_MOVE);
	Category = CATEGORY::OBJECT;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FLYWOOD_ANI_SET_ID);
	this->SetAnimationSet(ani_set);
}

FlyWood::FlyWood(float x, float y)
{
	StartX = x;
	StartY = y;

	Width = FLYWOOD_WIDTH;
	Height = FLYWOOD_HEIGHT;
	isCollision = false;
	ObjType = OBJECT_TYPE_FLYWOOD;
	SetState(FLYWOOD_STATE_MOVE);
	Category = CATEGORY::OBJECT;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FLYWOOD_ANI_SET_ID);
	this->SetAnimationSet(ani_set);
}

FlyWood::~FlyWood() {};
void FlyWood::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	float cam_x = _Camera->cam_x;
	// chiều dài màn hình
	int cam_w = CGame::GetInstance()->GetScreenWidth();

	//DebugOut(L"x = %f, cam_x = %i ,cam_w = %i\n", x, cam_x, cam_w);
	// ra khỏi camera -> delete
	if (x > cam_x + static_cast<float>(cam_w) || x + Width < cam_x)
	{
		isDisappear = true;
		return;
	}
	else
	{
		if (isDisappear == true)
		{
			SetPosition(StartX, StartY);
			isDisappear = false;
		}
	}
	if (isDisappear == false)
	{
		CGameObject::Update(dt);
		x += dx;
		y += dy;
		if (isCollision == true)
		{
			vx = 0;
			vy += FLYWOOD_GRAVITY * dt;
			if (y > _Map->GetHeight())
				canDelete = true;
		}
	}
}
void FlyWood::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + Width;
	b = y + Height;
}

void FlyWood::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case FLYWOOD_STATE_MOVE:
		{
			vx = -FLYWOOD_SPEEDX;
		}
		break;

		case FLYWOOD_STATE_FALL:
		{
			isCollision = true;
		}break;
	}
	
}

void FlyWood::Render()
{
	animation_set->at(FLYWOOD_ANI)->Render(x, y);
	//RenderBoundingBox();
}
