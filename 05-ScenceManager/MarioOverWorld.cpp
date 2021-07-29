
#include "MarioOverWorld.h"
#include "Gate.h"
#include "World1.h"


//MarioOverWorld* MarioOverWorld::GetInstance()
//{
//	if (__instance == NULL) __instance = new MarioOverWorld();
//	return __instance;
//}

MarioOverWorld::MarioOverWorld(float x, float y) : CGameObject()
{
	ObjType = OBJECT_TYPE_MARIOOVERWORLD;
	level = MARIO_OVERWORLD_LEVEL_SMALL;
	SetState(MARIO_OVERWORLD_STATE_IDLE);
	this->x = start_x = x;
	this->y = start_y = y;

	Scene = 0;
	InGate = false;
}

void MarioOverWorld::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		switch (coObjects->at(i)->Category)
		{
			case CATEGORY::OBJECT:
			{
				// lấy render box của 2 obj để kiểm tra xem chúng có nằm bên trong nhau hay không
				if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true
					&& coObjects->at(i)->ObjType == OBJECT_TYPE_GATE)
				{
					Gate* gate = dynamic_cast<Gate*>(coObjects->at(i));

					CanGoLeft = gate->MarioCanGo[0];
					CanGoUp = gate->MarioCanGo[1];
					CanGoRight = gate->MarioCanGo[2];
					CanGoDown = gate->MarioCanGo[3];

					// Code này áp dụng cho qua màn 1.1 mới được vào 1.2
					
					//if (gate->GateNumber > 0 && gate->GateNumber % 2 == 0 && gate->isComplete == false)
					//{
					//	SetState(MARIO_OVERWORLD_STATE_IN_GATE);
					//}
					//else
					//	InGate = false;
					//if (Scene <= gate->GateNumber) // xong màn +1 lên = 11, nếu k có thì gán lại = 10
					//	Scene = gate->GateNumber;
					//else
					//{
					//	gate->isComplete = true;
					//	if (Scene > 10 && gate->GateNumber == 10)
					//	{
					//		CanGoRight = 1;
					//	}
					//}
					//if (Scene <= gate->GateNumber) // xong màn +1 lên = 11, nếu k có thì gán lại = 10
					//	Scene = gate->GateNumber;
					//else
					//{
					//	gate->isComplete = true;
					//	if (Scene > 10 && gate->GateNumber == 10)
					//	{
					//		CanGoRight = 1;
					//	}
					//}

					if (gate->GateNumber > 0 && gate->GateNumber % 2 == 0)
					{
						SetState(MARIO_OVERWORLD_STATE_IN_GATE);
						Scene = gate->GateNumber;
					}
					else
						InGate = false;

					if (vx != 0)
					{
						int a = (int)x;
						int b = int(gate->x);
						if (vx >= 0)
						{
							if ( a == b + 1)
							{
								vx = vy = 0;
							}
						}
						else
						{
							if (a == b + 2)
							{
								vx = vy = 0;
							}
						}
					}
					else if (vy != 0)
					{
						int a = (int)y;
						int b = int(gate->y);
						if (vy >= 0)
						{
							if (a - 1 == b)
							{
								vy = 0;
							}
						}
						else
						{
							if (a + 1 == b)
							{
								vy = 0;
							}
						}
					}
				}
			}
			break;
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Gate*>(e->obj))
			{
				Gate* gate = dynamic_cast<Gate*>(e->obj);
				SetState(MARIO_OVERWORLD_STATE_IN_GATE);
				{
					this->x = gate->x + 1;
					this->y = gate->y;
					vx = 0;
					vy = 0;
				}
				if(Scene<= gate->GateNumber)
					Scene = gate->GateNumber;
				
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	/*if(InGate==true)
		DebugOut(L"InGate==true\n");
	else
		DebugOut(L"InGate==false\n");
	DebugOut(L"x = %f, y = %f, state %i\n", x, y, state);*/
	if (vy < 0 && y < 0)
	{
		SetPosition(x, 0);
		vy = 0;
	}
	if(InGate==true)
		DebugOut(L"InGate==true\n");
	else
		DebugOut(L"InGate==false\n");
}

void MarioOverWorld::Render()
{
	int ani = -1;

	switch (level)
	{
	case MARIO_OVERWORLD_LEVEL_SMALL:
	{
		ani = MARIO_OVERWORLD_ANI_SMALL;
	}
	break;

	case MARIO_OVERWORLD_LEVEL_BIG:
	{
		ani = MARIO_OVERWORLD_ANI_SMALL;
	}
	break;

	case MARIO_OVERWORLD_LEVEL_TAIL:
	{
		ani = MARIO_OVERWORLD_ANI_SMALL;
	}
	break;

	case MARIO_OVERWORLD_LEVEL_FIRE:
	{
		ani = MARIO_OVERWORLD_ANI_SMALL;
	}
	break;
	}

	animation_set->at(ani)->Render(x, y);
}

void MarioOverWorld::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_OVERWORLD_STATE_WALKING_RIGHT:
		if (CanGoRight == 1 && vx == 0.0f && vy == 0.0f)
		{
			x += 2;
			vx = MARIO_OVERWORLD_WALKING_SPEED;
			vy = 0;
			nx = 1;
		}
		break;
	case MARIO_OVERWORLD_STATE_WALKING_LEFT:
		if (CanGoLeft == 1 && vx == 0.0f && vy == 0.0f)
		{
			x -= 2;
			vx = -MARIO_OVERWORLD_WALKING_SPEED;
			vy = 0;
			nx = -1;
		}
		break;
	case MARIO_OVERWORLD_STATE_IDLE:
		vx = vy = 0;
		break;
	case MARIO_OVERWORLD_STATE_WALKING_UP:
		if (CanGoUp == 1 && vx == 0.0f && vy == 0.0f)
		{
			y -= 2;
			vy = -MARIO_OVERWORLD_WALKING_SPEED;
			vx = 0;
		}
		break;
	case MARIO_OVERWORLD_STATE_WALKING_DOWN:
		if (CanGoDown == 1 && vx == 0.0f && vy == 0.0f)
		{
			y += 2;
			vy = MARIO_OVERWORLD_WALKING_SPEED;
			vx = 0;
		}
		break;
	case MARIO_OVERWORLD_STATE_IN_GATE:
		InGate = true;
		break;
	}
}

void MarioOverWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (level)
	{
		case MARIO_OVERWORLD_LEVEL_SMALL:
		{
			right = x + MARIO_OVERWORLD_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_OVERWORLD_SMALL_BBOX_HEIGHT;
		}
		break;

		case MARIO_OVERWORLD_LEVEL_BIG:
		{
			right = x + MARIO_OVERWORLD_BIG_BBOX_WIDTH;
			bottom = y + MARIO_OVERWORLD_BIG_BBOX_HEIGHT;
		}break;

		case MARIO_OVERWORLD_LEVEL_TAIL:
		{
			right = x + MARIO_OVERWORLD_BIG_BBOX_WIDTH;
			bottom = y + MARIO_OVERWORLD_TAIL_BBOX_HEIGHT;
		}
		break;

		case MARIO_OVERWORLD_LEVEL_FIRE:
		{
			right = x + MARIO_OVERWORLD_BIG_BBOX_WIDTH;
			bottom = y + MARIO_OVERWORLD_BIG_BBOX_HEIGHT;
		
		}break;
	}
}