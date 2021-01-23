#include "Notification.h"
#include "PlayScence.h"

#define BREAKITEMBRICK_LEFT_ANI 0
#define BREAKITEMBRICK_RIGHT_ANI 1

#define BREAKITEMBREAKITEMBRICK_ANI_SET_ID 1251

Notification::Notification(float _x, float _y,int card)
{
	ObjType = OBJECT_TYPE_BREAKITEMBRICK;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BREAKITEMBREAKITEMBRICK_ANI_SET_ID);
	this->SetAnimationSet(ani_set);
	Category = CATEGORY::EFFECT;
	SetPosition(_x, _y);
	Width = 165;
	Height = 60;
	TypeCard = card;
	Stop = false;
}

void Notification::Render()
{
	animation_set->at(0)->Render(x, y);
	if (Stop == true)
		Item1->Draw(this->x + Width - 40, y  + 20);
}

void Notification::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + OBJECT_BBOX_WIDTH_HEIGHT / 2;
	b = y + OBJECT_BBOX_WIDTH_HEIGHT / 2;
}

void Notification::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (TypeCard == 0)
	{
		Item1 = CSprites::GetInstance()->Get(90002);
	}
	else if(TypeCard == 1)
	{
		Item1 = CSprites::GetInstance()->Get(90004);
	}
	else
		Item1 = CSprites::GetInstance()->Get(90004);

	if (y < _Map->GetHeight() - 150)
	{
		Stop = true;
		return;
	}
	vy -= 0.0001f * dt;
	x += dx;
	y += dy;

}
