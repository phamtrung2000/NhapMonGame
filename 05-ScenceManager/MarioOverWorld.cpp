
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
	InGate = false;
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

		// how to push back MarioOverWorld if collides with a moving objects, what if MarioOverWorld is pushed this way into another object?
		/*if (rdx != 0 && rdx!=dx)
			x += 2*nx*abs(5); */

			//if (ny != 0 && untouchable == false) vy = 0;
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj))
			{
				/*if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;*/
				Ground* ground = dynamic_cast<Ground*>(e->obj);
				if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
					
				}
				else
					y += min_ty * dy + ny * 0.4f;
			}
			else if (dynamic_cast<Gate*>(e->obj))
			{
				SetState(MARIO_OVERWORLD_STATE_IN_GATE);
				x += dx;
				y += dy;
				Gate* gate = dynamic_cast<Gate*>(e->obj);
				Scene = gate->GateNumber;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	if (coObjects->at(i)->ObjType == OBJECT_TYPE_GATE)
	//	{
	//		Gate* gate = dynamic_cast<Gate*>(coObjects->at(i));
	//		// lấy render box của 2 obj để kiểm tra xem chúng có nằm bên trong nhau hay không
	//		if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true)
	//		{
	//			/*if (gate->isComplete == false && this->vx > 0)
	//				this->vx = 0;*/
	//		}
	//	}
	//}
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
	DebugOut(L"x = %f, y = %f, state %i\n", x, y, state); 
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
	RenderBoundingBox();
}

void MarioOverWorld::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_OVERWORLD_STATE_WALKING_RIGHT:
		vx = MARIO_OVERWORLD_WALKING_SPEED;
		vy = 0;
		nx = 1;
		break;
	case MARIO_OVERWORLD_STATE_WALKING_LEFT:
		vx = -MARIO_OVERWORLD_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case MARIO_OVERWORLD_STATE_IDLE:
		vx = vy = 0;
		break;
	case MARIO_OVERWORLD_STATE_WALKING_UP:
		vy = -MARIO_OVERWORLD_WALKING_SPEED;
		vx = 0;
		break;
	case MARIO_OVERWORLD_STATE_WALKING_DOWN:
		vy = MARIO_OVERWORLD_WALKING_SPEED;
		vx = 0;
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