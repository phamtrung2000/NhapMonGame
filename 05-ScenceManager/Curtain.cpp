#include "Curtain.h"
#include "Camera.h"

Curtain::Curtain()
{
	choose = StopRender = IsAppear = IsDown = IsUp = false;
	//AppearTime = 0;
	AppearTime = GetTickCount64();
	Type = 0;
}
Curtain::Curtain(float X, float Y)
{
	StopRender = true;
	choose = IsAppear = IsDown = IsUp = false;
	//AppearTime = 0;
	AppearTime = GetTickCount64();
	Type = 0;
}

Curtain::Curtain(float X, float Y, int type)
{
	
	choose = IsAppear = IsDown = IsUp = false;
	//AppearTime = 0;
	AppearTime = GetTickCount64();
	Type = type;
	if (type == 0 || type == 1)
		StopRender = false;
	else
		StopRender = true;
}

//void Curtain::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
//{
//	CGameObject::Update(dt);
//	if (IsUp == false && AppearTime != 0)
//	{
//		if (AppearTime != 0 && GetTickCount64() - AppearTime > 1000 && IsUp == false)
//		{
//			vy = -0.1;
//			AppearTime = 0;
//			IsUp = true;
//			IsDown = true;
//		}
//			
//	}
//
//	if (IsUp)
//	{
//		if (y + 187 < 0)
//		{
//			IsDown = true;/*
//			IsUp = false;*/
//			//canDelete = true;
//		}
//	}
//	 if (IsDown)
//	{
//		if (AppearTime == 0)
//			AppearTime = GetTickCount64();
//		else
//		{
//			if (GetTickCount64() - AppearTime > 2500)
//				vy = 0.23;
//			else if (GetTickCount64() - AppearTime > 6000)
//				StopRender = true;
//		}
//	}
//	else if (IsDown && y >= 0)
//	{
//		vy = 0; y = 0;
//	}
//
//	y += dy;
//
//	
//
//	DebugOut(L"AppearTime = %u\n", AppearTime);
//}

void Curtain::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	if (Type == 0)
	{
		if (AppearTime != 0 && GetTickCount64() - AppearTime > 1000)
		{
			vy = -0.1f;
			AppearTime = 0;
			IsUp = true;
		}
		if (y + 187 < 0)
		{
			canDelete = true;
		}
	}
	else if (Type == 1)
	{
		if (GetTickCount64() - AppearTime > 4000 || IsDown == true)
			vy = 0.23f;
		if (GetTickCount64() - AppearTime > 8000)
			StopRender = true;
		
		if (y >= 0)
		{
			vy = 0; y = 0;
		}
	}
	else if (Type == 2)
	{
		if (GetTickCount64() - AppearTime > 10000)
			StopRender = false;
	}
	else
	{
		vy = 0;
	}
	y += dy;
}

void Curtain::Render()
{
	if (!StopRender)
	{
		if (!choose)
		{
			animation_set->at(CURTAIN_ANIMATION)->Render(x, y);
		}
		else
			animation_set->at(1)->Render(x, y);

	}
}

void Curtain::SetState(int state)
{
}

