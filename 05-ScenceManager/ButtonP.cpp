#include "ButtonP.h"
#include "PlayScence.h"
#include "ItemBrick.h"
#include "Coin.h"
#include "EffectHit.h"

ButtonP::ButtonP(float X, float Y)
{
	x = X;
	StartY = y = Y;
	ObjType = OBJECT_TYPE_BUTTONP;
	Category = CATEGORY::ITEM;
	state = BUTTONP_STATE_NORMAL;
	vx = vy = 0.0f;
	isPressed = false;
}

void ButtonP::SetBrickToCoin()
{
	for (unsigned int i = 0; i < Bricks.size(); i++)
	{
		ItemBrick* brick = dynamic_cast<ItemBrick*>(Bricks[i]);
		brick->check = true;
	}
}

void ButtonP::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BUTTONP_BBOX_WIDTH;
	if (isPressed == false)
	{
		bottom = top + BUTTONP_BBOX_HEIGHT;
	}
	else
	{
		bottom = y + BUTTONP_PRESS_BBOX_HEIGHT;
	}
	
	

}

void ButtonP::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isPressed == true)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->Category == CATEGORY::OBJECT && coObjects->at(i)->ObjType == OBJECT_TYPE_ITEMBRICK)
			{
				ItemBrick* itembrick = (ItemBrick*)coObjects->at(i);
				if (itembrick->Item == NORMAL)
				{
					float temp_x = itembrick->x;
					float temp_y = itembrick->y;
					itembrick->canDelete = true;
					
					/*auto hit = new EffectHit(temp_x, temp_y);
					_PlayScene->objects.push_back(hit);*/
					Coin* coin = new Coin();
					coin->SetPosition(temp_x, temp_y);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(12);
					coin->SetAnimationSet(ani_set);
					_Grid->AddStaticObject(coin, temp_x, temp_y);
				}
			}
		}
	}
}

void ButtonP::Render()
{
	int ani = -1;
	if (isPressed == false)
	{
		ani = BUTTONP_ANI_NORMAL;
	}
	else 
	{
		ani = BUTTONP_ANI_PRESS;
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void ButtonP::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BUTTONP_STATE_NORMAL:
	{
		isPressed = false;
	}
	break;

	case BUTTONP_STATE_PRESS:
	{
		if (isPressed == false)
		{
			isPressed = true;
			y = y + (BUTTONP_BBOX_HEIGHT - BUTTONP_PRESS_BBOX_HEIGHT);
		}
	}break;

	}
}
