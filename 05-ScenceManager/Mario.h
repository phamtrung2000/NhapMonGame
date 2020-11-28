#pragma once
#include "GameObject.h"
#include "FireBullet.h"
#include "Brick.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_RUNNING_MAX_SPEED	0.2f 
#define MARIO_FLY_MOVING_SPEED	0.1f 
#define GIA_TOC					0.003125f // gia tốc
#define MAX_LEVEL_OF_WALKING	32
#define MAX_LEVEL_OF_RUNNING	64

#define MARIO_JUMP_SPEED_FAST		0.25f // chuẩn
#define MARIO_JUMP_SPEED_SLOW		0.18f // chuẩn
#define MARIO_FLY_SPEED				0.1f // chuẩn
#define MARIO_JUMP_DEFLECT_SPEED 0.17f
#define MARIO_GRAVITY			0.0005f
#define MARIO_GRAVITY_FLYING	0.00005f
#define MARIO_DIE_DEFLECT_SPEED	 0.2f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_JUMP_LOW		500
#define MARIO_STATE_FLY_LOW			600
#define MARIO_STATE_FLY_HIGH		700
#define MARIO_STATE_SITDOWN			800
#define MARIO_STATE_ATTACK			900
#define MARIO_STATE_RUNNING			1000

#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_IDLE_LEFT				1
#define MARIO_ANI_SMALL_IDLE_RIGHT			2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_DIE						8

#define MARIO_ANI_BIG_JUMP_RIGHT			9
#define MARIO_ANI_BIG_JUMP_LEFT				10

#define MARIO_ANI_SMALL_JUMP_RIGHT			11
#define MARIO_ANI_SMALL_JUMP_LEFT			12

#define MARIO_ANI_SMALL_STOP_RIGHT			13
#define MARIO_ANI_SMALL_STOP_LEFT			14

#define MARIO_ANI_SMALL_RUNNING_RIGHT		15
#define MARIO_ANI_SMALL_RUNNING_LEFT		16

#define MARIO_ANI_BIG_STOP_RIGHT			17
#define MARIO_ANI_BIG_STOP_LEFT				18

#define MARIO_ANI_BIG_RUNNING_RIGHT			19
#define MARIO_ANI_BIG_RUNNING_LEFT			20


#define MARIO_ANI_TAIL_IDLE_RIGHT			21
#define MARIO_ANI_TAIL_IDLE_LEFT			22

#define MARIO_ANI_TAIL_WALKING_RIGHT		23
#define MARIO_ANI_TAIL_WALKING_LEFT			24

#define MARIO_ANI_TAIL_JUMP_RIGHT			25
#define MARIO_ANI_TAIL_JUMP_LEFT			26

#define MARIO_ANI_TAIL_STOP_RIGHT			27
#define MARIO_ANI_TAIL_STOP_LEFT			28

#define MARIO_ANI_TAIL_RUNNING_RIGHT		29
#define MARIO_ANI_TAIL_RUNNING_LEFT			30

#define MARIO_ANI_TAIL_FALLING_RIGHT		31
#define MARIO_ANI_TAIL_FALLING_LEFT			32

#define MARIO_ANI_TAIL_FLYING_RIGHT			33
#define MARIO_ANI_TAIL_FLYING_LEFT			34

#define MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT			35
#define MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT			36

#define MARIO_ANI_TAIL_SITDOWN_RIGHT		37
#define MARIO_ANI_TAIL_SITDOWN_LEFT			38

#define MARIO_ANI_TAIL_ATTACK_1				39
#define MARIO_ANI_TAIL_ATTACK_2				40
#define MARIO_ANI_TAIL_ATTACK_3				41
#define MARIO_ANI_TAIL_ATTACK_4				42

#define MARIO_ANI_BIG_SITDOWN_RIGHT			43
#define MARIO_ANI_BIG_SITDOWN_LEFT			44

#define MARIO_ANI_FIRE_IDLE_RIGHT			45
#define MARIO_ANI_FIRE_IDLE_LEFT			46

#define MARIO_ANI_FIRE_WALKING_RIGHT		47
#define MARIO_ANI_FIRE_WALKING_LEFT			48

#define MARIO_ANI_FIRE_STOP_RIGHT			49
#define MARIO_ANI_FIRE_STOP_LEFT			50

#define MARIO_ANI_FIRE_JUMP_RIGHT			51
#define MARIO_ANI_FIRE_JUMP_LEFT			52

#define MARIO_ANI_FIRE_RUNNING_RIGHT		53
#define MARIO_ANI_FIRE_RUNNING_LEFT			54

#define MARIO_ANI_FIRE_SITDOWN_RIGHT		55
#define MARIO_ANI_FIRE_SITDOWN_LEFT			56

#define MARIO_ANI_FIRE_ATTACK_RIGHT_1		57
#define MARIO_ANI_FIRE_ATTACK_RIGHT_2		58
#define MARIO_ANI_FIRE_ATTACK_LEFT_1		59
#define MARIO_ANI_FIRE_ATTACK_LEFT_2		60

#define MARIO_ANI_SMALL_KICK_RIGHT			61
#define MARIO_ANI_SMALL_KICK_LEFT			62
#define MARIO_ANI_SMALL_HOLD_IDLE_RIGHT		63
#define MARIO_ANI_SMALL_HOLD_WALK_RIGHT		64
#define MARIO_ANI_SMALL_HOLD_IDLE_LEFT		65
#define MARIO_ANI_SMALL_HOLD_WALK_LEFT		66

#define MARIO_ANI_BIG_KICK_RIGHT			67
#define MARIO_ANI_BIG_KICK_LEFT				68
#define MARIO_ANI_BIG_HOLD_IDLE_RIGHT		69
#define MARIO_ANI_BIG_HOLD_WALK_RIGHT		70
#define MARIO_ANI_BIG_HOLD_IDLE_LEFT		71
#define MARIO_ANI_BIG_HOLD_WALK_LEFT		72

#define MARIO_ANI_SMALL_HOLD_JUMP_RIGHT		73
#define MARIO_ANI_SMALL_HOLD_JUMP_LEFT		74
#define MARIO_ANI_BIG_HOLD_JUMP_RIGHT		75
#define MARIO_ANI_BIG_HOLD_JUMP_LEFT		76

#define MARIO_ANI_BIG_FALL_RIGHT	77
#define MARIO_ANI_BIG_FALL_LEFT		78

#define MARIO_ANI_TAIL_KICK_RIGHT	79
#define MARIO_ANI_TAIL_KICK_LEFT	80
#define MARIO_ANI_TAIL_HOLD_IDLE_RIGHT		81
#define MARIO_ANI_TAIL_HOLD_WALK_RIGHT		82
#define MARIO_ANI_TAIL_HOLD_IDLE_LEFT		83
#define MARIO_ANI_TAIL_HOLD_WALK_LEFT		84
#define MARIO_ANI_TAIL_HOLD_JUMP_RIGHT		85
#define MARIO_ANI_TAIL_HOLD_JUMP_LEFT		86
#define MARIO_ANI_TAIL_FLY_FALL_RIGHT		87
#define MARIO_ANI_TAIL_FLY_FALL_LEFT		88

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_TAIL	3
#define	MARIO_LEVEL_FIRE	4

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 27
#define MARIO_BIG_BBOX_SITDOWN_HEIGHT 18

#define MARIO_TAIL_BBOX_WIDTH  21
#define MARIO_TAIL_BBOX_HEIGHT 28
#define MARIO_TAIL_BBOX_SITDOWN_HEIGHT 18

#define MARIO_UNTOUCHABLE_TIME 5000

#define LEFT -1
#define RIGHT 1


#define TIME_ATTACK 5
#define TIME_FLY 200000 //200
#define TIME_FLY_S 5
class Mario : public CGameObject
{
public:
	int level;
	bool untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of Mario at scene
	float start_y; 

	bool OnGround;
	bool isRunning;
	int	 level_of_running;
	bool ChangeDirection;
	int  level_of_walking;
	int  level_of_stopping;
	bool isMaxRunning;
	
	bool isFalling;
	bool isSitDown;
	bool isAttacking, endAttack;
	int time_attack;
	int time_fly;
	int NumberBullet;
	int ani;
	bool isHolding = false, pressA = false, canKick = false;
	int Height;

	bool canFlyX, canFlyS; // biến để cắm cờ lúc mario đạt max running và bay lên thì level running không trừ cho đến hết tgian bay
	bool isFlyingLow,isFlyingHigh;
public: 
	Mario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = true; untouchable_start = GetTickCount64(); }
	void Reset();
	void DownLevel();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

//#include <algorithm>
//#include <assert.h>
//#include "Utils.h"
//
//#include "Mario.h"
//#include "Game.h"
//
//#include "Goomba.h"
//#include "Koopas.h"
//#include "Portal.h"
//#include "Block.h"
//#include "Brick.h"
//#include "QuestionBrick.h"
//#include "QuestionBrickItem.h"
//#include "FireBullet.h"
//#include "Ground.h"
//#include "WarpPipe.h"
//
//#define TIME_ATTACK 5
//#define TIME_FLY 200
//#define TIME_FLY_S 5
//
//
//Mario::Mario(float x, float y) : CGameObject()
//{
//	isDie = false;
//	level = MARIO_LEVEL_SMALL;
//	ObjType = untouchable = 0;
//	SetState(MARIO_STATE_IDLE);
//
//	start_x = x; 
//	start_y = y; 
//	this->x = x; 
//	this->y = y; 
//
//	ChangeDirection = isRunning = isMaxRunning = isFlyingHigh = canFlyX = canFlyS  = isFalling = isSitDown = isAttacking = endAttack = false;
//	OnGround = true;
//	level_of_walking = level_of_running = 1;
//	level_of_stopping = 1;
//
//	time_attack = time_fly = 0;
//	ani = 0;
//	NumberBullet = 2;
//}
//
//void Mario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
//{
//	// Calculate dx, dy 
//	CGameObject::Update(dt);
//	if (canFlyX == true)
//	{
//		time_fly++;
//		// hết thời gian bay
//		if (time_fly > TIME_FLY)
//		{
//			time_fly = 0;
//			canFlyX = isFlyingHigh = false;
//			level_of_running = 10;
//			vy = 0;
//			SetState(MARIO_STATE_IDLE);
//		}
//	}
//	else if (canFlyS == true)
//	{
//		time_fly++;
//		if (time_fly > TIME_FLY)
//		{
//			canFlyS = isFlyingHigh = false;
//			level_of_running = 10;
//			vy = 0;
//			SetState(MARIO_STATE_IDLE);
//		}
//	}
//	// Simple fall down
//	if (isFlyingLow == false)
//		vy += MARIO_GRAVITY * dt;
//	else
//	{
//		vy += MARIO_GRAVITY_FLYING * dt;
//	}
//
//	if (vy >= 0 && OnGround == false)
//		isFalling = true;
//
//	if (isAttacking == true)
//	{
//		time_attack++;
//		if (time_attack >= 5 * TIME_ATTACK)
//		{
//			time_attack = 0;
//			isAttacking = false;
//		}
//	}
//
//	if (isRunning == false && level_of_running > 1)
//	{
//		/*if (isflying != true &&)
//			level_of_running--;*/
//	}
//
//	if (isHolding == true && pressA == false)
//		canKick = true;
//
//	if(canKick==true)
//	{
//		time_attack++;
//		if (time_attack >= 2 * TIME_ATTACK)
//		{
//			canKick = false;
//			time_attack = 0;
//		}
//	}
//
//	if (level_of_running == MAX_LEVEL_OF_RUNNING)
//		isMaxRunning = true;
//	else
//		isMaxRunning = false;
//
//	switch (level)
//	{
//	case MARIO_LEVEL_SMALL:
//		Height = MARIO_SMALL_BBOX_HEIGHT; break;
//	case MARIO_LEVEL_BIG: case MARIO_LEVEL_FIRE:
//		Height = MARIO_BIG_BBOX_HEIGHT; break;
//	case MARIO_LEVEL_TAIL:
//		Height = MARIO_TAIL_BBOX_HEIGHT; break;
//	}
//
//	if (vy < 0 && y < 0)
//	{
//		y = 0;
//	}
//		
//	vector<LPCOLLISIONEVENT> coEvents;
//	vector<LPCOLLISIONEVENT> coEventsResult;
//
//	coEvents.clear();
//
//	// turn off collision when die 
//	if (state!=MARIO_STATE_DIE)
//		CalcPotentialCollisions(coObjects, coEvents);
//
//	// reset untouchable timer if untouchable time has passed
//	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
//	{
//		untouchable_start = 0;
//		untouchable = 0;
//	}
//
//	// No collision occured, proceed normally
//	if (coEvents.size()==0)
//	{
//		x += dx; 
//		y += dy;
//	}
//	else
//	{
//		float min_tx, min_ty, nx = 0, ny;
//		float rdx = 0;
//		float rdy = 0;
//
//		// TODO: This is a very ugly designed function!!!!
//		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
//
//		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
//		/*if (rdx != 0 && rdx!=dx)
//			x += 2*nx*abs(5); */
//
//		// block every object first!
//		x += min_tx * dx + nx * 0.4f;
//		if (ny != 0) vy = 0;
//
//		/*x += min_tx * dx + nx * 0.4f;
//		y += min_ty * dy + ny * 0.4f;
//		if (nx != 0) vx = 0;
//		if (ny != 0) vy = 0;*/
//
//		for (UINT i = 0; i < coEventsResult.size(); i++)
//		{
//
//			LPCOLLISIONEVENT e = coEventsResult[i];
//			//DebugOut(L"size = %i, type=%i\n", coEventsResult.size(),e->obj->ObjType);
//
//			// xử lý tạm vụ chạm đất
//			if (e->ny < 0)
//			{
//				OnGround = true;
//				isFalling = false;
//				isFlyingLow = isFlyingHigh = false;
//			}
//
//			if (dynamic_cast<Ground*>(e->obj))
//			{
//				Ground* ground = dynamic_cast<Ground*>(e->obj);
//				if (y + min_ty * dy + ny * 0.4f + Height < ground->y)
//					y += min_ty * dy + ny * 0.4f;
//				else
//					vy = 0;
//			}
//			else if (dynamic_cast<WarpPipe*>(e->obj))
//			{
//				y += min_ty * dy + ny * 0.4f;
//			}
//			else if (dynamic_cast<Block*>(e->obj))
//			{
//				x += dx;
//				if (isFlyingHigh == true)
//				{
//					y += dy;
//				}
//			}
//			else if (dynamic_cast<Brick*>(e->obj))
//			{
//				Brick* brick = dynamic_cast<Brick*>(e->obj);
//				if (brick->Type == 1)
//				{
//					if (e->ny >0 && isFlyingHigh == true)
//					{
//						x += dx;
//						y += dy;
//					}
//					else if (e->nx != 0)
//					{
//						vx = 0;
//						y += min_ty * dy + ny * 0.4f;
//					}
//				}
//				else 
//				{
//					if (e->nx != 0)
//					{
//						vx = 0;
//						y += min_ty * dy + ny * 0.4f;
//					}
//				}
//			}
//			else if (dynamic_cast<QuestionBrick*>(e->obj))
//			{
//				// mario nhảy từ dưới lên va chạm gạch 
//				if (e->ny > 0)
//				{
//					// nếu state normal thì xử lý va chạm, nếu không thì k xử lý
//					// cả 2 đều làm cho mario k nhảy đươc tiếp + rớt xuống
//					if (e->obj->GetState() == BRICK_STATE_NORMAL)
//						e->obj->SetState(BRICK_STATE_COLLISION);
//					//vy = 0;
//				}
//				else if (e->nx != 0)
//				{
//					vx = 0;
//					y += min_ty * dy + ny * 0.4f;
//				}
//			}
//			else if (dynamic_cast<CPortal*>(e->obj))
//			{
//				CPortal* p = dynamic_cast<CPortal*>(e->obj);
//				CGame::GetInstance()->SwitchScene(p->GetSceneId());
//			}
//
//			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
//			{
//				DebugOut(L"e->nx %f, e->ny %f\n", e->nx, e->ny);
//				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
//
//				// jump on top >> kill Goomba and deflect a bit 
//				if (e->ny < 0)
//				{
//					if (goomba->GetState() == GOOMBA_STATE_WALKING)
//					{
//						goomba->SetState(GOOMBA_STATE_DIE);
//						vy = -MARIO_JUMP_DEFLECT_SPEED;
//					}
//				}
//				else if (e->nx != 0)
//				{
//					if (untouchable == 0)
//					{
//						if (goomba->GetState() == GOOMBA_STATE_WALKING)
//						{
//							if (level == MARIO_LEVEL_TAIL && isAttacking == true)
//							{
//								goomba->SetState(GOOMBA_STATE_DIE_2);
//							}
//							else
//								DownLevel();
//							
//						}
//					}
//				}
//			} 
//			else if (dynamic_cast<Koopas*>(e->obj)) // if e->obj is Goomba 
//			{
//				DebugOut(L"va cham koopas e->nx=%f,e->ny=%f\n", e->nx, e->ny);
//				Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
//				// nhảy lên đầu rùa
//				if (e->ny < 0)
//				{
//					if (koopas->GetState() == KOOPAS_STATE_WALKING_RIGHT || koopas->GetState() == KOOPAS_STATE_WALKING_LEFT)
//					{
//						koopas->SetState(KOOPAS_STATE_SHELL);
//						koopas->ReviveTime = GetTickCount64();
//						vy = -MARIO_JUMP_DEFLECT_SPEED;
//					}
//					else if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_HOLD)
//					{
//						if (this->x <= koopas->x)
//							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
//						else
//							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
//					}
//				}
//				else
//				{
//					// va chạm rùa
//					if (e->nx != 0)
//					{
//						if (untouchable == 0)
//						{
//							//if (koopas->GetState()== KOOPAS_STATE_WALKING_RIGHT || koopas->GetState()==KOOPAS_STATE_WALKING_LEFT)
//							if (koopas->vx != 0)
//							{
//								if (level == MARIO_LEVEL_TAIL && isAttacking == true)
//								{
//									koopas->SetState(KOOPAS_STATE_SHELL_2);
//									koopas->ReviveTime = GetTickCount64();
//								}
//								else
//									DownLevel();
//							}
//							else if ((koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2)
//								&& koopas->vy >= 0) // tránh trường hợp mai rùa đang trên trời vẫn đá được
//							{
//								if (e->nx < 0 )
//								{
//									if (pressA == true)
//									{
//										
//										isHolding = true;
//										koopas->nx = 1;
//										koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
//									}
//									else if(koopas->vy >= 0)
//									{
//										canKick = true;
//										koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
//									}
//
//								}
//								else if (e->nx == RIGHT)
//								{
//									if (pressA == true)
//									{
//										DebugOut(L"aaaaaaaaaaaaaaaaaaaa\n");
//										isHolding = true;
//										koopas->nx = -1;
//										koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
//									}
//									else if (koopas->vy >= 0)
//									{
//										canKick = true;
//										koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
//									}
//								}
//							}
//						}
//					}
//					// mario nhảy lên chạm chân rùa
//					else if (e->ny > 0)
//					{
//						y = y - 2;
//						if (untouchable == 0)
//						{
//							//DebugOut(L"va cham koopas\n");
//							//if (koopas->GetState()== KOOPAS_STATE_WALKING_RIGHT || koopas->GetState()==KOOPAS_STATE_WALKING_LEFT)
//							if (koopas->vx != 0)
//							{
//								switch (level)
//								{
//								case MARIO_LEVEL_SMALL:
//								{
//									SetState(MARIO_STATE_DIE);
//								}break;
//
//								case MARIO_LEVEL_TAIL:
//								{
//									if (isAttacking == true)
//									{
//										koopas->SetState(KOOPAS_STATE_SHELL_2);
//									}
//									else
//									{
//										level--;
//										StartUntouchable();
//									}
//								}break;
//
//								default:
//								{
//									level--;
//									StartUntouchable();
//								}break;
//								}
//							}
//						}
//					}
//				}
//			} // if Goomba
//			else if (dynamic_cast<FireBullet*>(e->obj))
//			{
//				FireBullet* firebullet = dynamic_cast<FireBullet*>(e->obj);
//				// jump on top >> kill firebullet and deflect a bit 
//				//if ( firebullet->FireMario == false && (e->nx != 0 || e->ny != 0))
//				if (firebullet->FireMario == false)
//				{
//					if (untouchable == 0)
//					{
//						switch (level)
//						{
//						case MARIO_LEVEL_FIRE:
//						{
//							y = (int)(y - abs(MARIO_BIG_BBOX_HEIGHT - MARIO_TAIL_BBOX_HEIGHT) * 2);
//							level--;
//							StartUntouchable();
//
//						}
//						break;
//
//						case MARIO_LEVEL_TAIL:
//						{
//							level--;
//							StartUntouchable();
//
//						}
//						break;
//
//						case MARIO_LEVEL_BIG:
//						{
//
//							y = (int)(y + abs(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT - 1));
//							level--;
//							StartUntouchable();
//						}
//						break;
//
//						case MARIO_LEVEL_SMALL:
//						{
//							SetState(MARIO_STATE_DIE);
//						}
//						break;
//						}
//
//					}
//					else
//					{
//						if (level == MARIO_LEVEL_SMALL)
//							y -= 1;
//					}
//				}
//				else
//				{
//					DebugOut(L"firebullet->FireMario == true\n");
//				}
//			}
//			else if (dynamic_cast<QuestionBrickItem*>(e->obj))
//			{
//				QuestionBrickItem* questionbrickitem = dynamic_cast<QuestionBrickItem*>(e->obj);
//
//				////va chạm nấm khi đi hoặc nấm rớt từ trên xuống
//				//if (e->nx != 0 || e->ny > 0)
//				//{
//				//	//if (questionbrickitem->GetState() == QUESTIONBRICKITEM_STATE_MOVE)
//				//	if (questionbrickitem->vx != 0)
//				//	{
//				//		questionbrickitem->isDie = true;
//				//		switch (level)
//				//		{
//				//		case MARIO_LEVEL_SMALL:
//				//		{
//				//			level++;
//				//			y -= static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) + 2;
//				//		}
//				//		break;
//				//
//				//		case MARIO_LEVEL_BIG:
//				//		{
//				//			level++;
//				//			y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
//				//		}
//				//		break;
//				//
//				//		case MARIO_LEVEL_TAIL:
//				//		{
//				//			level++;
//				//			y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
//				//		}
//				//		break;
//				//
//				//		default:
//				//		{
//				//			y -= 2;
//				//			break;
//				//		}
//				//		}
//				//	}
//				//}
//
//				questionbrickitem->isDie = true;
//				switch (level)
//				{
//				case MARIO_LEVEL_SMALL:
//				{
//					level++;
//					y -= static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) + 2;
//				}
//				break;
//
//				case MARIO_LEVEL_BIG:
//				{
//					level++;
//					y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
//				}
//				break;
//
//				case MARIO_LEVEL_TAIL:
//				{
//					level++;
//					y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
//				}
//				break;
//
//				default:
//				{
//					y -= 2;
//					break;
//				}
//
//				}
//			}
//		}
//	}
//
//
//	// clean up collision events
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//	//DebugOut(L"3. y = %f, vy =%f\n", y, vy);
//}
//
//void Mario::Render()
//{
//	ani = 0;
//	if (state == MARIO_STATE_DIE)
//		ani = MARIO_ANI_DIE;
//	else
//	{
//		switch (level)
//		{
//		case MARIO_LEVEL_SMALL:
//		{
//			// chưa nhảy // đang rớt xuống
//			if (vy >= 0)
//			{
//				// đứng yên
//				if (vx == 0)
//				{
//					if (ChangeDirection == false)
//					{
//						// phải 
//						if (nx == RIGHT)
//						{
//							ani = MARIO_ANI_SMALL_IDLE_RIGHT;
//							if (isHolding == true)
//								ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
//						}
//						// trái
//						else
//						{
//							ani = MARIO_ANI_SMALL_IDLE_LEFT;
//							if (isHolding == true)
//								ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
//						}
//					}
//					else
//						ani = MARIO_ANI_SMALL_STOP_RIGHT;
//				}
//				// đi bộ qua phải
//				else if (vx > 0)
//				{
//					if (ChangeDirection == false)
//					{
//						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
//						if (isHolding == true)
//							ani = MARIO_ANI_SMALL_HOLD_WALK_RIGHT;
//						else if (isMaxRunning == true)
//							ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
//					}
//					else if (ChangeDirection == true)
//						ani = MARIO_ANI_SMALL_STOP_RIGHT;
//				}
//				//  đi bộ qua trái
//				else
//				{
//					if (ChangeDirection == false)
//					{
//						ani = MARIO_ANI_SMALL_WALKING_LEFT;
//						if (isHolding == true)
//							ani = MARIO_ANI_SMALL_HOLD_WALK_LEFT;
//						else if (isMaxRunning == true)
//							ani = MARIO_ANI_SMALL_RUNNING_LEFT;
//					}
//					else if (ChangeDirection == true)
//						ani = MARIO_ANI_SMALL_STOP_LEFT;
//				}
//			}
//			// nhảy
//			else
//			{
//				// cầm rùa + nhảy
//				if (isHolding == true)
//				{
//					// phải 
//					if (nx == RIGHT)
//						ani = MARIO_ANI_SMALL_HOLD_JUMP_RIGHT;
//					// trái
//					else
//						ani = MARIO_ANI_SMALL_HOLD_JUMP_LEFT;
//				}
//				else
//				{
//					// phải 
//					if (nx == RIGHT)
//						ani = MARIO_ANI_SMALL_JUMP_RIGHT;
//					// trái
//					else
//						ani = MARIO_ANI_SMALL_JUMP_LEFT;
//				}
//			}
//
//			// đá
//			if (canKick == true)
//			{
//				if (nx == RIGHT)
//					ani = MARIO_ANI_SMALL_KICK_RIGHT;
//				else
//					ani = MARIO_ANI_SMALL_KICK_LEFT;
//			}
//		}
//		break;
//
//		case MARIO_LEVEL_BIG:
//		{
//			// chưa nhảy / đang rớt xuống
//			if (vy >= 0)
//			{
//				// đang chạm đất
//				if (OnGround == true)
//				{
//					// đứng yên
//					if (vx == 0)
//					{
//						// đi thẳng k chuyển hướng
//						if (ChangeDirection == false)
//						{
//							// phải 
//							if (nx == RIGHT)
//							{
//								ani = MARIO_ANI_BIG_IDLE_RIGHT;
//								if (isHolding == true)
//									ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
//								else if (isSitDown == true)
//									ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
//							}
//							// trái
//							else
//							{
//								ani = MARIO_ANI_BIG_IDLE_LEFT;
//								if (isHolding == true)
//									ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
//								else if (isSitDown == true)
//									ani = MARIO_ANI_BIG_SITDOWN_LEFT;
//							}
//						}
//					}
//					// đi bộ qua phải
//					else if (vx > 0)
//					{
//						if (ChangeDirection == false)
//						{
//							ani = MARIO_ANI_BIG_WALKING_RIGHT;
//							if (isHolding == true)
//								ani = MARIO_ANI_BIG_HOLD_WALK_RIGHT;
//							else if (isMaxRunning == true)
//								ani = MARIO_ANI_BIG_RUNNING_RIGHT;
//						}
//						else if (ChangeDirection == true)
//						{
//							if (isHolding != true)
//								ani = MARIO_ANI_BIG_STOP_RIGHT;
//						}
//					}
//					//  đi bộ qua trái
//					else
//					{
//						if (ChangeDirection == false)
//						{
//							ani = MARIO_ANI_BIG_WALKING_LEFT;
//							if (isHolding == true)
//								ani = MARIO_ANI_BIG_HOLD_WALK_LEFT;
//							else if (isMaxRunning == true)
//								ani = MARIO_ANI_BIG_RUNNING_LEFT;
//						}
//						else if (ChangeDirection == true)
//						{
//							if (isHolding != true)
//								ani = MARIO_ANI_BIG_STOP_LEFT;
//						}
//					}
//				}
//				// rớt xuống
//				else
//				{
//					if (isHolding == true)
//					{
//						// phải 
//						if (nx == RIGHT)
//							ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;
//						// trái
//						else
//							ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;
//					}
//					else
//					{
//						// phải 
//						if (nx == RIGHT)
//							ani = MARIO_ANI_BIG_FALL_RIGHT;
//						// trái
//						else
//							ani = MARIO_ANI_BIG_FALL_LEFT;
//					}
//				}
//			}
//			// nhảy
//			else
//			{
//				if (isHolding == true)
//				{
//					// phải 
//					if (nx == RIGHT)
//						ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;
//					// trái
//					else
//						ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;
//				}
//				else
//				{
//					// phải 
//					if (nx == RIGHT)
//						ani = MARIO_ANI_BIG_JUMP_RIGHT;
//					// trái
//					else
//						ani = MARIO_ANI_BIG_JUMP_LEFT;
//				}
//			}
//
//			// đá
//			if (canKick == true)
//			{
//				if (nx == RIGHT)
//					ani = MARIO_ANI_BIG_KICK_RIGHT;
//				else
//					ani = MARIO_ANI_BIG_KICK_LEFT;
//			}
//		}
//		break;
//
//		case MARIO_LEVEL_TAIL:
//		{
//			if (canFlyX == true || canFlyS == true)
//			{
//				// chưa nhảy // đang rớt xuống
//				if (vy >= 0)
//				{
//					// đang chạm đất
//					if (OnGround == true)
//					{
//						// đứng yên
//						if (vx == 0)
//						{
//							// phải 
//							if (nx == RIGHT)
//							{
//								ani = MARIO_ANI_TAIL_IDLE_RIGHT;
//								if (isHolding == true)
//									ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
//								else if (isSitDown == true)
//									ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
//								else if (isAttacking == true && endAttack == false)
//								{
//									if (time_attack <= TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_1;
//									}
//									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_2;
//									}
//									else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_3;
//									}
//									else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_4;
//									}
//									else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_1;
//									}
//								}
//							}
//							// trái
//							else
//							{
//								ani = MARIO_ANI_TAIL_IDLE_LEFT;
//								if (isHolding == true)
//									ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
//								else if (isSitDown == true)
//									ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
//								else if (isAttacking == true && endAttack == false)
//								{
//									if (time_attack <= TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_3;
//									}
//									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_2;
//									}
//									else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_1;
//									}
//									else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_4;
//									}
//									else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_3;
//									}
//								}
//							}
//						}
//						// đi bộ qua phải
//						else if (vx > 0)
//						{
//							if (ChangeDirection == false)
//							{
//								ani = MARIO_ANI_TAIL_WALKING_RIGHT;
//								if (isHolding == true)
//									ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//								else if (isMaxRunning == true)
//									ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
//							}
//							else if (ChangeDirection == true)
//								ani = MARIO_ANI_TAIL_STOP_RIGHT;
//
//							if (isAttacking == true && endAttack == false)
//							{
//								if (time_attack <= TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_1;
//								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_2;
//								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_3;
//								else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_4;
//								else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_1;
//							}
//						}
//						//  đi bộ qua trái
//						else
//						{
//							if (ChangeDirection == false)
//							{
//								ani = MARIO_ANI_TAIL_WALKING_LEFT;
//								if (isHolding == true)
//									ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
//								else if (isMaxRunning == true)
//									ani = MARIO_ANI_TAIL_RUNNING_LEFT;
//							}
//							else if (ChangeDirection == true)
//								ani = MARIO_ANI_TAIL_STOP_LEFT;
//
//							if (isAttacking == true && endAttack == false)
//							{
//								if (time_attack <= TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_3;
//								}
//								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_2;
//								}
//								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_1;
//								}
//								else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_4;
//								}
//								else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_3;
//								}
//							}
//						}
//					}
//					// rớt xuống
//					else
//					{
//						// phải 
//						if (nx == RIGHT)
//						{
//							ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//						}
//						// trái
//						else
//						{
//							ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//						}
//					}
//				}
//				// nhảy
//				else
//				{
//				// phải 
//					if (nx == RIGHT)
//					{
//						if (isMaxRunning == true)
//						{
//							if (isFlyingHigh == true)
//								ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT;
//							else
//								ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//						}
//					}
//					// trái
//					else
//					{
//						if (isMaxRunning == true)
//						{
//							if (isFlyingHigh == true)
//								ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT;
//							else
//								ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//						}
//					}
//				}
//			}
//			else
//			{
//				// chưa nhảy // đang rớt xuống
//				if (vy >= 0)
//				{
//					// đang chạm đất
//					if (OnGround == true)
//					{
//						// đứng yên
//						if (vx == 0)
//						{
//							// phải 
//							if (nx == RIGHT)
//							{
//								ani = MARIO_ANI_TAIL_IDLE_RIGHT;
//								if (isHolding == true)
//									ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
//								else if (isSitDown == true)
//									ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
//								else if (isAttacking == true && endAttack == false)
//								{
//									if (time_attack <= TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_1;
//									}
//									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_2;
//									}
//									else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_3;
//									}
//									else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_4;
//									}
//									else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_1;
//									}
//								}
//							}
//							// trái
//							else
//							{
//								ani = MARIO_ANI_TAIL_IDLE_LEFT;
//								if (isHolding == true)
//									ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
//								else if (isSitDown == true)
//									ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
//								else if (isAttacking == true && endAttack == false)
//								{
//									if (time_attack <= TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_3;
//									}
//									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_2;
//									}
//									else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_1;
//									}
//									else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_4;
//									}
//									else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_TAIL_ATTACK_3;
//									}
//								}
//							}
//						}
//						// đi bộ qua phải
//						else if (vx > 0)
//						{
//							if (ChangeDirection == false)
//							{
//								ani = MARIO_ANI_TAIL_WALKING_RIGHT;
//								if (isHolding == true)
//									ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//								else if (isMaxRunning == true)
//									ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
//							}
//							else if (ChangeDirection == true)
//								ani = MARIO_ANI_TAIL_STOP_RIGHT;
//
//							if (isAttacking == true && endAttack == false)
//							{
//								if (time_attack <= TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_1;
//								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_2;
//								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_3;
//								else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_4;
//								else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//									ani = MARIO_ANI_TAIL_ATTACK_1;
//							}
//						}
//						//  đi bộ qua trái
//						else
//						{
//							if (ChangeDirection == false)
//							{
//								ani = MARIO_ANI_TAIL_WALKING_LEFT;
//								if (isHolding == true)
//									ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
//								else if (isMaxRunning == true)
//									ani = MARIO_ANI_TAIL_RUNNING_LEFT;
//							}
//							else if (ChangeDirection == true)
//								ani = MARIO_ANI_TAIL_STOP_LEFT;
//
//							if (isAttacking == true && endAttack == false)
//							{
//								if (time_attack <= TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_3;
//								}
//								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_2;
//								}
//								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_1;
//								}
//								else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_4;
//								}
//								else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//								{
//									ani = MARIO_ANI_TAIL_ATTACK_3;
//								}
//							}
//						}
//					}
//					// rớt xuống
//					else
//					{
//						if (isHolding == true)
//						{
//							// phải 
//							if (nx == RIGHT)
//								ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
//							// trái
//							else
//								ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
//						}
//						else
//						{
//							// phải 
//							if (nx == RIGHT)
//							{
//								ani = MARIO_ANI_TAIL_FALLING_RIGHT;
//								if(isFlyingLow == true)
//									ani = MARIO_ANI_TAIL_FLYING_RIGHT;
//							}
//							// trái
//							else
//							{
//								ani = MARIO_ANI_TAIL_FALLING_LEFT;
//								if (isFlyingLow == true)
//									ani = MARIO_ANI_TAIL_FLYING_LEFT;
//							}
//						}
//					}
//				}
//				// nhảy
//				else
//				{
//					if (isHolding == true)
//					{
//						// phải 
//						if (nx == RIGHT)
//							ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
//						// trái
//						else
//							ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
//					}
//					else
//					{
//						// phải 
//						if (nx == RIGHT)
//						{
//							ani = MARIO_ANI_TAIL_JUMP_RIGHT;
//						}
//						// trái
//						else
//						{
//							ani = MARIO_ANI_TAIL_JUMP_LEFT;
//						}
//					}
//				}
//			}
//			
//			// đá
//			if (canKick == true)
//			{
//				if (nx == RIGHT)
//					ani = MARIO_ANI_TAIL_KICK_RIGHT;
//				else
//					ani = MARIO_ANI_TAIL_KICK_LEFT;
//			}
//		}
//		break;
//
//		case MARIO_LEVEL_FIRE:
//		{
//			// chưa nhảy // đang rớt xuống
//			if (vy >= 0)
//			{
//				if (OnGround == true)
//				{
//					// đứng yên
//					if (vx == 0)
//					{
//						if (ChangeDirection == false)
//						{
//							// phải 
//							if (nx == RIGHT)
//							{
//								ani = MARIO_ANI_FIRE_IDLE_RIGHT;
//								if (isSitDown == true)
//								{
//									ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
//								}
//								else if (isAttacking == true && endAttack == false)
//								{
//									if (time_attack <= TIME_ATTACK)
//									{
//										ani = MARIO_ANI_FIRE_ATTACK_RIGHT_1;
//									}
//									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_FIRE_ATTACK_RIGHT_2;
//									}
//								}
//							}
//
//							// trái
//							else
//							{
//								ani = MARIO_ANI_FIRE_IDLE_LEFT;
//								if (isSitDown == true)
//								{
//									ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
//								}
//								else if (isAttacking == true && endAttack == false)
//								{
//									if (time_attack <= TIME_ATTACK)
//									{
//										ani = MARIO_ANI_FIRE_ATTACK_LEFT_1;
//									}
//									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_FIRE_ATTACK_LEFT_2;
//									}
//								}
//							}
//
//						}
//						else
//						{
//							ani = MARIO_ANI_FIRE_STOP_RIGHT;
//						}
//
//					}
//					// đi bộ qua phải
//					else if (vx > 0)
//					{
//						// ani = MARIO_ANI_FIRE_WALKING_RIGHT;
//
//						if (ChangeDirection == false)
//						{
//							ani = MARIO_ANI_FIRE_WALKING_RIGHT;
//
//							if (isMaxRunning == true)
//								ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
//						}
//						else if (ChangeDirection == true)
//						{
//							ani = MARIO_ANI_FIRE_STOP_RIGHT;
//						}
//
//						if (isAttacking == true && endAttack == false)
//						{
//							if (time_attack <= TIME_ATTACK)
//							{
//								ani = MARIO_ANI_FIRE_ATTACK_RIGHT_1;
//							}
//							else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//							{
//								ani = MARIO_ANI_FIRE_ATTACK_RIGHT_2;
//							}
//							
//						}
//					}
//					//  đi bộ qua trái
//					else if(vx < 0)
//					{
//						if (ChangeDirection == false)
//						{
//							ani = MARIO_ANI_FIRE_WALKING_LEFT;
//
//							if (isMaxRunning == true)
//								ani = MARIO_ANI_FIRE_RUNNING_LEFT;
//						}
//						else if (ChangeDirection == true)
//						{
//							ani = MARIO_ANI_FIRE_STOP_LEFT;
//						}
//
//						if (isAttacking == true && endAttack == false)
//						{
//							if (time_attack <= TIME_ATTACK)
//							{
//								ani = MARIO_ANI_FIRE_ATTACK_LEFT_1;
//							}
//							else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//							{
//								ani = MARIO_ANI_FIRE_ATTACK_LEFT_2;
//							}
//							
//						}
//					}
//				}
//				else
//				{
//					// đứng yên
//					if (vx == 0)
//					{
//						// phải 
//						if (nx == RIGHT)
//							ani = MARIO_ANI_FIRE_JUMP_RIGHT;
//						// trái
//						else
//							ani = MARIO_ANI_FIRE_JUMP_LEFT;
//					}
//					// đi bộ qua phải
//					else if (vx > 0)
//						ani = MARIO_ANI_FIRE_JUMP_RIGHT;
//					else
//						ani = MARIO_ANI_FIRE_JUMP_LEFT;
//				}
//			}
//			// nhảy
//			else
//			{
//				// đứng yên
//				if (vx == 0)
//				{
//					// phải 
//					if (nx == RIGHT)
//						ani = MARIO_ANI_FIRE_JUMP_RIGHT;
//					// trái
//					else
//						ani = MARIO_ANI_FIRE_JUMP_LEFT;
//				}
//				// đi bộ qua phải
//				else if (vx > 0)
//				{
//					ani = MARIO_ANI_FIRE_JUMP_RIGHT;
//				}
//				//  đi bộ qua trái
//				else
//				{
//					ani = MARIO_ANI_FIRE_JUMP_LEFT;
//				}
//			}
//		}
//		break;
//		}
//	}
//	
//	/*if (ani >= 23 && state !=0)
//	{
//		if(isAttacking == false)
//			DebugOut(L"isAttacking == false\t");
//		else
//			DebugOut(L"isAttacking == true\t");
//		
//		DebugOut(L"\n5.Render	Ani: %i, State=%i, time_attack=%i\n", ani, state, time_attack);
//	}*/
//	
//	int alpha = 255;
//	if (untouchable) alpha = 128;
//
//	/*if (isHolding == false)
//		DebugOut(L"isHolding == false\t");
//	else
//		DebugOut(L"isHolding == true\t");
//
//	if (pressA == false)
//		DebugOut(L"pressA == false\t");
//	else
//		DebugOut(L"pressA == true\t");
//
//	if (canKick == false)
//		DebugOut(L"canKick == false\t");
//	else
//	{
//		DebugOut(L"canKick == true\t");
//		DebugOut(L"5.Render	Ani: %i, state=%i,vx=%f\t", ani,state,vx);
//	}*/
//	
//	//if (canFlyS == false)
//	//	DebugOut(L"canFlyS == false\t");
//	//else
//	//	DebugOut(L"canFlyS == true\t");
//	//if (canFlyX == false)
//	//	DebugOut(L"canFlyX == false\t");
//	//else
//	//	DebugOut(L"canFlyX == true\t");
//	//if (isFlyingHigh == false)
//	//	DebugOut(L"isFlyingHigh == false\t");
//	//else
//	//	DebugOut(L"isFlyingHigh == true\t");
//	//if (isMaxRunning == false)
//	//	DebugOut(L"isMaxRunning == false\t");
//	//else
//	//	DebugOut(L"isMaxRunning == true\t");
//
//	//DebugOut(L"5.Render	Ani: %i\n", ani);
//	//DebugOut(L"state =%i,vy = %f,level walk = %i, level running = %i, time fly = %i\n", state, vy, level_of_walking, level_of_running, time_fly);
//
//	animation_set->at(ani)->Render(x, y, alpha);
//	RenderBoundingBox();
//	//DebugOut(L"\n");
//}
//
//void Mario::SetState(int state)
//{
//	CGameObject::SetState(state);
//
//	switch (state)
//	{
//	case MARIO_STATE_WALKING_RIGHT:
//	{
//		// đi bộ
//		if (isRunning == false)
//		{
//			if (vx < 0)
//			{
//				ChangeDirection = true;
//				vx += 2 * GIA_TOC;
//
//				if (vx >= 0)
//					ChangeDirection = false;
//			}
//			else
//			{
//				if (isFlyingHigh == true)
//				{
//					vx = MARIO_FLY_MOVING_SPEED;
//				}
//				else
//				{
//					ChangeDirection = false;
//					vx = level_of_walking * GIA_TOC;
//					if (level_of_walking < MAX_LEVEL_OF_WALKING)
//						level_of_walking++;
//				}
//			}
//		}
//		// chạy
//		else
//		{
//			if (vx < 0 && isHolding!=true)
//			{
//				ChangeDirection = true;
//				vx += float(5 * GIA_TOC);
//
//				if (level_of_running > 1)
//					level_of_running--;
//
//				if (vx >= 0)
//					ChangeDirection = false;
//			}
//			else
//			{
//				ChangeDirection = false;
//				vx = level_of_running * GIA_TOC;
//
//				if (level_of_running < MAX_LEVEL_OF_RUNNING)
//					level_of_running++;
//				else
//					isMaxRunning = true;
//			}
//		}
//		if (isSitDown == true)
//		{
//			y = y - 10;
//			isSitDown = false;
//		}
//		nx = 1;
//	}
//	break;
//
//	case MARIO_STATE_WALKING_LEFT:
//	{
//		if (isRunning == false)
//		{
//			// đang đi qua bên phải (vx > 0) thì đột ngột qua trái -> khúc này xử lý vụ khựng lại r chuyển hướng
//			if (vx > 0)
//			{
//				// bắt đầu chuyển hướng di chuyển
//				ChangeDirection = true;
//
//				// trừ bớt vx chứ không gán trực tiếp để nó hiện animation khựng
//				// vx -= (level_of_walking * GIA_TOC); ->không nhân với level để vx giảm chậm để hiện animation
//				vx -= 2 * GIA_TOC;
//				
//				// vx đạt tới mức di chuyển sang trái thì hủy chuyển hướng để trở về animation WALKING_LEFT
//				// nếu không thì vẫn sẽ là animation STOP_RIGHT đi về bên trái
//				if (vx <= 0)
//					ChangeDirection = false;
//			}
//			else 
//			{
//				if (isFlyingHigh == true)
//				{
//					vx = -MARIO_FLY_MOVING_SPEED;
//				}
//				else
//				{
//					ChangeDirection = false;
//					vx = -(level_of_walking * GIA_TOC);
//
//					if (level_of_walking < MAX_LEVEL_OF_WALKING)
//						level_of_walking++;
//				}
//			}
//		}
//		else
//		{
//			if (vx > 0 && isHolding != true)
//			{
//				ChangeDirection = true;
//				vx -= float(5 * GIA_TOC);
//
//				if (level_of_running > 1)
//					level_of_running--;
//
//				if (vx <= 0)
//					ChangeDirection = false;
//			}
//			else
//			{
//				ChangeDirection = false;
//				
//				vx = -(level_of_running * GIA_TOC);
//
//				if (level_of_running < MAX_LEVEL_OF_RUNNING)
//					level_of_running++;
//				else
//					isMaxRunning = true;
//			}
//		}
//		if (isSitDown == true)
//		{
//			y = y - 10;
//			isSitDown = false;
//		}
//		nx = -1;
//	}
//	break;
//
//	case MARIO_STATE_JUMP:
//	{
//		if (OnGround == true)
//		{
//			vy = -MARIO_JUMP_SPEED_FAST;
//			OnGround = false;
//		}
//	}
//	break;
//
//	case MARIO_STATE_JUMP_LOW:
//	{
//		if (OnGround == true)
//		{
//			vy = -MARIO_JUMP_SPEED_SLOW;
//			OnGround = false;
//		}
//	}
//	break;
//
//	case MARIO_STATE_IDLE:
//	{
//		if (vx > 0)
//		{
//			if (level_of_running > 1)
//			{
//				vx -= 2 * GIA_TOC;
//			}
//			// đi bộ
//			else if (level_of_walking > 0)
//			{
//				vx -= GIA_TOC;
//				//level_of_walking--;
//			}
//
//			if (vx < 0)
//			{
//				vx = 0;
//			}
//
//		}
//		else if (vx < 0)
//		{
//			if (level_of_running > 1)
//			{
//				vx += 2 * GIA_TOC;
//			}
//			else if (level_of_walking > 0)
//			{
//				vx += GIA_TOC;
//				//level_of_walking--;
//			}
//			if (vx > 0)
//				vx = 0;
//		}
//	
//		// đang trong trạng thái bay thì k trừ 
//		if (canFlyX == false)
//		{
//			if (level_of_walking > 1)
//				level_of_walking--;
//			if (level_of_running > 1)
//				level_of_running--;
//		}
//		if (isSitDown == true)
//		{
//			y = y - 10;
//			isSitDown = false;
//		}
//		ChangeDirection = false;
//
//		if (isFlyingHigh != true)
//		{
//			//isMaxRunning = false;
//		}
//		if (pressA != true)
//		{
//			isHolding = isRunning = false;
//		}
//		
//	}
//	break;
//
//	case MARIO_STATE_DIE:
//	{
//		vx = 0;
//		if(vy >= 0)
//			vy = -MARIO_DIE_DEFLECT_SPEED;
//	}
//	break;
//
//	case MARIO_STATE_FLY_LOW:
//	{
//		/*if (canFlyX == true)
//		{
//			if (time_fly <= TIME_FLY)
//			{
//				if(isMaxRunning==true)
//					vy = -MARIO_FLY_SPEED;
//				isFlyingHigh = true;
//				time_fly++;
//			}
//			else
//			{
//				isMaxRunning = false;
//				level_of_running = 10;
//				if (vy < 0)
//				{
//					vy = 0;
//				}
//			}
//		}
//		else if (canFlyS == true)
//		{
//			DebugOut(L"time fly = %i\n",time_fly);
//			if (time_fly <= TIME_FLY_S)
//			{
//				time_fly++;
//				isFlyingHigh = true;
//			}
//			else
//			{
//				isMaxRunning = false;
//				level_of_running = 10;
//				if (vy < 0)
//				{
//					vy = 0;
//				}
//			}
//		}*/
//		isFlyingLow = true;
//		if (canFlyX == true)
//		{
//			
//		}
//		else if (canFlyS == true)
//		{
//
//		}
//	}
//	break;
//
//	case MARIO_STATE_FLY_HIGH:
//	{
//		isFlyingHigh = true;
//		vy = -MARIO_FLY_SPEED;
//		/*if (canFlyX == true)
//		{
//			if (time_fly <= TIME_FLY)
//			{
//				vy = -MARIO_FLY_SPEED;
//				time_fly++;
//			}
//			else
//			{
//				level_of_running = 10;
//				if (vy < 0)
//				{
//					vy = 0;
//				}
//			}
//		}
//		else if (canFlyS == true)
//		{
//			DebugOut(L"time fly = %i\n", time_fly);
//			if (time_fly <= TIME_FLY_S)
//			{
//				if (isMaxRunning == true)
//					vy = -MARIO_FLY_SPEED;
//				time_fly++;
//				isFlyingHigh = true;
//			}
//			else
//			{
//				isMaxRunning = false;
//				level_of_running = 10;
//				if (vy < 0)
//				{
//					vy = 0;
//				}
//			}
//		}*/
//	}
//	break;
//
//	case MARIO_STATE_SITDOWN:
//	{
//		if (isSitDown == false)
//		{
//			isSitDown = true;
//			// sprite ngồi heigh chỉ có 18 còn sprite idle thì heigh = 28 nên phải trừ bớt để k bị lỗi sprite
//			// sau khi +10 thì phải -10 lại bên idle để không bị lỗi mario thụt xuống dưới
//			if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
//				y = y + (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_BBOX_SITDOWN_HEIGHT);
//			else
//				y = y + (MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_BBOX_SITDOWN_HEIGHT);
//
//			vx = 0;
//		}
//	}
//	break;
//
//	case MARIO_STATE_ATTACK:
//	{
//		isAttacking = true;
//		//time_attack = 0;
//	}
//	break;
//	
//	}
//
//	// DebugOut(L"level of running=%i\n",level_of_running);
//}
//
//void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
//{
//	left = x;
//	top = y;
//	switch (level)
//	{
//	case MARIO_LEVEL_SMALL:
//	{
//		right = x + MARIO_SMALL_BBOX_WIDTH;
//		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
//	}
//	break;
//
//	case MARIO_LEVEL_BIG:
//	{
//		right = x + MARIO_BIG_BBOX_WIDTH;
//		bottom = y + MARIO_BIG_BBOX_HEIGHT;
//		if (isSitDown == true)
//		{
//			bottom = y + MARIO_BIG_BBOX_SITDOWN_HEIGHT;
//		}
//	}break;
//
//	case MARIO_LEVEL_TAIL:
//	{
//		/*if (nx == RIGHT)
//		{
//			left = x + (MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH + 1);
//			right = x + MARIO_TAIL_BBOX_WIDTH;
//		}
//		else
//		{
//			left = x;
//			right = x + (MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH + 4);
//		}*/
//		right = x + MARIO_TAIL_BBOX_WIDTH;
//		if (isAttacking == true && endAttack == false && nx==1)
//		{
//			//right = x + MARIO_TAIL_BBOX_WIDTH + (MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH);
//			//DebugOut(L"MARIO right= %f, x=% f\n", right, x);
//		}
//		else if  (isAttacking == true && endAttack == false && nx == -1)
//		{
//			//left = x - (MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH);
//			//DebugOut(L"MARIO left = %f, right= %f, x=% f\n", left, right, x);
//		}
//		if(isHolding==true)
//			right = x + MARIO_TAIL_BBOX_WIDTH;
//
//		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
//		if (isSitDown == true)
//		{
//			bottom = y + MARIO_TAIL_BBOX_SITDOWN_HEIGHT;
//		}
//	}
//	break;
//
//	case MARIO_LEVEL_FIRE:
//	{
//		right = x + MARIO_BIG_BBOX_WIDTH;
//		bottom = y + MARIO_BIG_BBOX_HEIGHT;
//		if (isSitDown == true)
//		{
//			bottom = y + MARIO_BIG_BBOX_SITDOWN_HEIGHT;
//		}
//	}break;
//	}
//	
//	//if(y<500)
//		//DebugOut(L"bottom= %f, y=% f\n",bottom, y);
//}
//
//
//void Mario::Reset()
//{
//	SetState(MARIO_STATE_IDLE);
//	SetLevel(MARIO_LEVEL_SMALL);
//	SetPosition(start_x, start_y);
//	SetSpeed(0, 0);
//	NumberBullet = 2;
//	canFlyS = canFlyX = false;
//	time_fly = 0;
//}
//
//void Mario::DownLevel()
//{
//	switch (level)
//	{
//	case MARIO_LEVEL_SMALL:
//	{
//		SetState(MARIO_STATE_DIE);
//	}break;
//	case MARIO_LEVEL_BIG:
//	{
//		y += MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
//	}break;
//	case MARIO_LEVEL_TAIL:
//	{
//		y += MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
//	}break;
//	case MARIO_LEVEL_FIRE:
//	{
//		y -= MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT + 1;
//	}break;
//
//	}
//	level--;
//	StartUntouchable();
//}

//void Mario::DownLevel()
//{
//	switch (level)
//	{
//	case MARIO_LEVEL_SMALL:
//	{
//		SetState(MARIO_STATE_DIE);
//	}break;
//	case MARIO_LEVEL_BIG:
//	{
//		y += MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
//	}break;
//	case MARIO_LEVEL_TAIL:
//	{
//		y += MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
//	}break;
//	case MARIO_LEVEL_FIRE:
//	{
//		y -= MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
//	}break;
//	}
//	level--;
//	StartUntouchable();
//}

//else
//	{
//	float min_tx, min_ty, nx = 0, ny;
//	float rdx = 0;
//	float rdy = 0;
//
//	// TODO: This is a very ugly designed function!!!!
//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
//
//	// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
//	/*if (rdx != 0 && rdx!=dx)
//		x += 2*nx*abs(5); */
//
//		// block every object first!
//	x += min_tx * dx + nx * 0.4f;
//	y += min_ty * dy + ny * 0.4f;
//	if (ny != 0) vy = 0;
//
//	/*x += min_tx * dx + nx * 0.4f;
//	y += min_ty * dy + ny * 0.4f;
//	if (nx != 0) vx = 0;
//	if (ny != 0) vy = 0;*/
//
//	for (UINT i = 0; i < coEventsResult.size(); i++)
//	{
//
//		LPCOLLISIONEVENT e = coEventsResult[i];
//
//		// xử lý tạm vụ chạm đất
//		if (e->ny < 0)
//		{
//			OnGround = true;
//			isFalling = false;
//			isFlyingLow = isFlyingHigh = false;
//		}
//
//		if (dynamic_cast<Ground*>(e->obj))
//		{
//			Ground* ground = dynamic_cast<Ground*>(e->obj);
//			/*if (y + min_ty * dy + ny * 0.4f + Height < ground->y)
//				y += min_ty * dy + ny * 0.4f;
//			else*/
//			vy = 0;
//		}
//		else if (dynamic_cast<WarpPipe*>(e->obj))
//		{
//			y += min_ty * dy + ny * 0.4f;
//		}
//		else if (dynamic_cast<Block*>(e->obj))
//		{
//			x += dx;
//			if (isFlyingHigh == true)
//			{
//				y += dy;
//			}
//		}
//		else if (dynamic_cast<Brick*>(e->obj))
//		{
//			Brick* brick = dynamic_cast<Brick*>(e->obj);
//			if (brick->Type == 1)
//			{
//				if (e->ny > 0 && isFlyingHigh == true)
//				{
//					x += dx;
//					y += dy;
//				}
//				else if (e->nx != 0)
//				{
//					vx = 0;
//					y += min_ty * dy + ny * 0.4f;
//				}
//			}
//			else
//			{
//				if (e->nx != 0)
//				{
//					vx = 0;
//					y += min_ty * dy + ny * 0.4f;
//				}
//			}
//		}
//		else if (dynamic_cast<QuestionBrick*>(e->obj))
//		{
//			// mario nhảy từ dưới lên va chạm gạch 
//			if (e->ny > 0)
//			{
//				// nếu state normal thì xử lý va chạm, nếu không thì k xử lý
//				// cả 2 đều làm cho mario k nhảy đươc tiếp + rớt xuống
//				if (e->obj->GetState() == BRICK_STATE_NORMAL)
//					e->obj->SetState(BRICK_STATE_COLLISION);
//				//vy = 0;
//			}
//			else if (e->nx != 0)
//			{
//				x -= min_tx * dx + nx * 0.4f; // không cho mario đi xuyên gạch
//				level_of_running = level_of_walking = 1; // không hiện vụ giảm vận tốc khi thả nút di chuyện
//			}
//		}
//		else if (dynamic_cast<CPortal*>(e->obj))
//		{
//			CPortal* p = dynamic_cast<CPortal*>(e->obj);
//			CGame::GetInstance()->SwitchScene(p->GetSceneId());
//		}
//
//		if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
//		{
//			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
//
//			// jump on top >> kill Goomba and deflect a bit 
//			if (e->ny < 0)
//			{
//				if (goomba->GetState() == GOOMBA_STATE_WALKING)
//				{
//					goomba->SetState(GOOMBA_STATE_DIE);
//					vy = -MARIO_JUMP_DEFLECT_SPEED;
//				}
//			}
//			else if (e->nx != 0)
//			{
//				if (untouchable == 0)
//				{
//					if (goomba->GetState() == GOOMBA_STATE_WALKING)
//					{
//						/*if (level > MARIO_LEVEL_SMALL)
//						{
//							if (level == MARIO_LEVEL_TAIL)
//							{
//								if (isAttacking == true)
//								{
//									goomba->SetState(GOOMBA_STATE_DIE_2);
//								}
//								else
//								{
//									level--;
//									StartUntouchable();
//								}
//
//							}
//							else
//							{
//								level--;
//								StartUntouchable();
//							}
//						}
//						else
//							SetState(MARIO_STATE_DIE);*/
//						if (level == MARIO_LEVEL_TAIL && isAttacking == true)
//							goomba->SetState(GOOMBA_STATE_DIE_2);
//						else
//							DownLevel();
//					}
//				}
//			}
//		}
//		else if (dynamic_cast<Koopas*>(e->obj)) // if e->obj is Goomba 
//		{
//			Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
//			if (e->ny < 0) // nhảy lên đầu rùa
//			{
//				//if (koopas->GetState() == KOOPAS_STATE_WALKING_RIGHT || koopas->GetState() == KOOPAS_STATE_WALKING_LEFT) // rùa đang di chuyển
//				if (koopas->vx != 0)
//				{
//					koopas->SetState(KOOPAS_STATE_SHELL);
//					koopas->ReviveTime = GetTickCount64();
//					vy = -MARIO_JUMP_DEFLECT_SPEED;
//				}
//				else if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_HOLD)
//				{
//					if (this->x <= koopas->x)
//						koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
//					else
//						koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
//				}
//				//else if (koopas->GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT || koopas->GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
//				//{
//				//	/*if (koopas->isShell==true)
//				//		koopas->SetState(KOOPAS_STATE_SHELL);
//				//	else
//				//		koopas->SetState(KOOPAS_STATE_SHELL_2);*/
//				//	DebugOut(L"111111111111\n");
//				//	//koopas->vx = 0;
//				//	koopas->SetState(KOOPAS_STATE_SHELL);
//
//				//}
//			}
//			else
//			{
//				// va chạm rùa
//				if (e->nx != 0)
//				{
//					if (untouchable == 0)
//					{
//						//if (koopas->GetState()== KOOPAS_STATE_WALKING_RIGHT || koopas->GetState()==KOOPAS_STATE_WALKING_LEFT)
//						if (koopas->vx != 0)
//						{
//							switch (level)
//							{
//							case MARIO_LEVEL_SMALL:
//							{
//								SetState(MARIO_STATE_DIE);
//							}break;
//
//							case MARIO_LEVEL_TAIL:
//							{
//								if (isAttacking == true)
//								{
//									koopas->SetState(KOOPAS_STATE_SHELL_2);
//									koopas->ReviveTime = GetTickCount64();
//								}
//								else
//								{
//									level--;
//									StartUntouchable();
//								}
//							}break;
//
//							default:
//							{
//								level--;
//								StartUntouchable();
//							}break;
//
//
//							}
//						}
//						else if ((koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2) && koopas->vy >= 0) // tránh trường hợp mai rùa đang trên trời vẫn đá được
//						{
//							if (e->nx < 0)
//							{
//								if (pressA == true)
//								{
//
//									isHolding = true;
//									koopas->nx = 1;
//									koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
//								}
//								else if (koopas->vy >= 0)
//								{
//									canKick = true;
//									koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
//								}
//
//							}
//							else if (e->nx == RIGHT)
//							{
//								if (pressA == true)
//								{
//									isHolding = true;
//									koopas->nx = -1;
//									koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
//								}
//								else if (koopas->vy >= 0)
//								{
//									canKick = true;
//									koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
//								}
//							}
//						}
//					}
//				}
//				// mario nhảy lên chạm chân rùa
//				else if (e->ny > 0)
//				{
//					y = y - 2;
//					if (untouchable == 0)
//					{
//						//if (koopas->GetState()== KOOPAS_STATE_WALKING_RIGHT || koopas->GetState()==KOOPAS_STATE_WALKING_LEFT)
//						if (koopas->vx != 0)
//						{
//							switch (level)
//							{
//							case MARIO_LEVEL_SMALL:
//							{
//								SetState(MARIO_STATE_DIE);
//							}break;
//
//							case MARIO_LEVEL_TAIL:
//							{
//								if (isAttacking == true)
//								{
//									koopas->SetState(KOOPAS_STATE_SHELL_2);
//								}
//								else
//								{
//									level--;
//									StartUntouchable();
//								}
//							}break;
//
//							default:
//							{
//								level--;
//								StartUntouchable();
//							}break;
//							}
//						}
//					}
//				}
//			}
//		} // if Goomba
//		else if (dynamic_cast<FireBullet*>(e->obj))
//		{
//			FireBullet* firebullet = dynamic_cast<FireBullet*>(e->obj);
//			// jump on top >> kill firebullet and deflect a bit 
//			//if ( firebullet->FireMario == false && (e->nx != 0 || e->ny != 0))
//			if (firebullet->FireMario == false)
//			{
//				if (untouchable == 0)
//				{
//					switch (level)
//					{
//					case MARIO_LEVEL_FIRE:
//					{
//						y = (int)(y - abs(MARIO_BIG_BBOX_HEIGHT - MARIO_TAIL_BBOX_HEIGHT) * 2);
//						level--;
//						StartUntouchable();
//
//					}
//					break;
//
//					case MARIO_LEVEL_TAIL:
//					{
//						level--;
//						StartUntouchable();
//
//					}
//					break;
//
//					case MARIO_LEVEL_BIG:
//					{
//
//						y = (int)(y + abs(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT - 1));
//						level--;
//						StartUntouchable();
//					}
//					break;
//
//					case MARIO_LEVEL_SMALL:
//					{
//						SetState(MARIO_STATE_DIE);
//					}
//					break;
//					}
//
//				}
//				else
//				{
//					if (level == MARIO_LEVEL_SMALL)
//						y -= 1;
//				}
//			}
//			else
//			{
//			}
//		}
//		else if (dynamic_cast<QuestionBrickItem*>(e->obj))
//		{
//			QuestionBrickItem* questionbrickitem = dynamic_cast<QuestionBrickItem*>(e->obj);
//
//			////va chạm nấm khi đi hoặc nấm rớt từ trên xuống
//			//if (e->nx != 0 || e->ny > 0)
//			//{
//			//	//if (questionbrickitem->GetState() == QUESTIONBRICKITEM_STATE_MOVE)
//			//	if (questionbrickitem->vx != 0)
//			//	{
//			//		questionbrickitem->isDie = true;
//			//		switch (level)
//			//		{
//			//		case MARIO_LEVEL_SMALL:
//			//		{
//			//			level++;
//			//			y -= static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) + 2;
//			//		}
//			//		break;
//			//
//			//		case MARIO_LEVEL_BIG:
//			//		{
//			//			level++;
//			//			y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
//			//		}
//			//		break;
//			//
//			//		case MARIO_LEVEL_TAIL:
//			//		{
//			//			level++;
//			//			y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
//			//		}
//			//		break;
//			//
//			//		default:
//			//		{
//			//			y -= 2;
//			//			break;
//			//		}
//			//		}
//			//	}
//			//}
//
//			questionbrickitem->isDie = true;
//			switch (level)
//			{
//			case MARIO_LEVEL_SMALL:
//			{
//				level++;
//				y -= static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) + 2;
//			}
//			break;
//
//			case MARIO_LEVEL_BIG:
//			{
//				level++;
//				y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
//			}
//			break;
//
//			case MARIO_LEVEL_TAIL:
//			{
//				level++;
//				y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
//			}
//			break;
//
//			default:
//			{
//				y -= 2;
//				break;
//			}
//
//			}
//		}
//	}
//	}