#pragma once
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.12f//0.1f 
#define MARIO_RUNNING_MAX_SPEED	0.21f 
#define MARIO_FLY_MOVING_SPEED	0.1f 
#define GIA_TOC					0.003f//0.003125f // gia tốc
#define MAX_LEVEL_OF_WALKING	32
#define MAX_LEVEL_OF_RUNNING	70//98

#define MARIO_JUMP_SPEED_FAST		0.27f	//0.25f // chuẩn
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

#define MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG		89

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

#define MARIO_ANI_SMALL_STOP_RIGHT			14
#define MARIO_ANI_SMALL_STOP_LEFT			13

#define MARIO_ANI_SMALL_RUNNING_RIGHT		15
#define MARIO_ANI_SMALL_RUNNING_LEFT		16

#define MARIO_ANI_BIG_STOP_RIGHT			18
#define MARIO_ANI_BIG_STOP_LEFT				17

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

#define MARIO_ANI_FIRE_STOP_RIGHT			50
#define MARIO_ANI_FIRE_STOP_LEFT			49

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
#define MARIO_SMALL_BBOX_HEIGHT 14//15

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 26//27
#define MARIO_BIG_BBOX_SITDOWN_HEIGHT 18

#define MARIO_TAIL_BBOX_WIDTH  16//21
#define MARIO_TAIL_BBOX_HEIGHT 27//28
#define MARIO_TAIL_BBOX_SITDOWN_HEIGHT 18

#define MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT 21//22

#define LEFT -1
#define RIGHT 1

#define TIME_ATTACK 4
#define TIME_FLY 200
#define TIME_FLY_S 40
#define MARIO_UNTOUCHABLE_TIME 5000
#define TIME_LEVEL_UP 3000
#define ITIME_LEVEL_UP 180
#define LEVEL_UP_DIVIDE	15

class Mario : public CGameObject
{
private:
	static Mario* __instance;
public:
	
	bool untouchable; // trạng thái bất tử
	ULONGLONG untouchable_start; // thời điểm bắt đầu bất tử
	
	bool test;

	float start_x, start_y;		// vị trí bắt đầu
	float NewX, NewY; // vị trí mới sau khi rời khỏi map ẩn

	bool OnGround; // chạm đất
	bool isRunning; 
	int	 level_of_running;
	bool ChangeDirection; // chuyển hướng, để hiện ani khựng lại quay đầu
	int  level_of_walking;
	int  level_of_stopping;
	bool isMaxRunning;
	
	bool isFalling, // đang rớt xuống
		isSitDown, // đang ngồi
		GoHiddenWorld, // đi vào map ẩn để hiện ani xuống cống
		isAttacking, // đang ở state attack
		endAttack; // thực hiện xong động tác quật đuôi thì mới cho phép quật đuôi tiếp
	int time_attack; // biến đếm thời gian thực hiện động tác quật đuôi để hiện ani tương ứng
	int time_fly,FlyTimePer1; // thời gian bay, hết thời gian thì rớt xuống
	int NumberBullet; // số lượng đạn ( 2 viên )
	int ani;
	bool isHolding = false, pressA = false, canKick = false;
	int Height,Width;


	bool render_tail;
	// BAY (FLY)
	bool canFlyX, canFlyS, // mario có thể bay khi đạt max running và bay lên thì level running không trừ cho đến hết tgian bay
		isFlyingLow, isFlyingHigh; // biến để xác định mario đang ở state fly
		 
	// TĂNG CẤP (UP LEVEL)
	ULONGLONG LevelUpTime; // thời điểm bắt đầu tăng cấp
	bool isLevelUp; // mario tăng cấp để hiện effect tăng cấp
	int iLevelUpTime; // Thời gian tăng cấp + hiện effect tăng cấp
	int level; // cấp độ
public: 
	Mario(float x = 0.0f, float y = 0.0f);
	static Mario* GetInstance();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = true; untouchable_start = GetTickCount64(); }
	void Reset();
	void DownLevel();
	void UpLevel();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Debug();
	void Unload();
	void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
};

//case MARIO_LEVEL_TAIL:
//{
//	if (canFlyX == true)
//	{
//		// chưa nhảy // đang rớt xuống
//		if (vy >= 0)
//		{
//			// đang chạm đất
//			if (OnGround == true)
//			{
//				// đứng yên
//				if (level_of_walking == 0 && level_of_running == 0)
//				{
//					// phải 
//					if (nx == RIGHT)
//					{
//						ani = MARIO_ANI_TAIL_IDLE_RIGHT;
//						if (isHolding == true)
//							ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
//						else if (isSitDown == true)
//							ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
//					}
//					// trái
//					else
//					{
//						ani = MARIO_ANI_TAIL_IDLE_LEFT;
//						if (isHolding == true)
//							ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
//						else if (isSitDown == true)
//							ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
//					}
//				}
//				// đi bộ qua phải
//				else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
//				{
//					if (ChangeDirection == false)
//					{
//						ani = MARIO_ANI_TAIL_WALKING_RIGHT;
//						if (isHolding == true)
//							ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//					}
//					else if (ChangeDirection == true)
//						ani = MARIO_ANI_TAIL_STOP_RIGHT;
//				}
//				//  đi bộ qua trái
//				else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
//				{
//					if (ChangeDirection == false)
//					{
//						ani = MARIO_ANI_TAIL_WALKING_LEFT;
//						if (isHolding == true)
//							ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
//					}
//					else if (ChangeDirection == true)
//						ani = MARIO_ANI_TAIL_STOP_LEFT;
//				}
//			}
//			// rớt xuống
//			else
//			{
//				// phải 
//				if (nx == RIGHT)
//				{
//					ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//				}
//				// trái
//				else
//				{
//					ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//				}
//			}
//		}
//		// nhảy
//		else
//		{
//			// phải 
//			if (nx == RIGHT)
//			{
//				if (isMaxRunning == true)
//				{
//					if (isFlyingHigh == true)
//						ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT;
//					else
//						ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//				}
//			}
//			// trái
//			else
//			{
//				if (isMaxRunning == true)
//				{
//					if (isFlyingHigh == true)
//						ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT;
//					else
//						ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//				}
//			}
//		}
//	}
//	else if (canFlyS == true)
//	{
//		// đang rớt xuống
//		if (vy >= 0)
//		{
//			// phải 
//			if (nx == RIGHT)
//			{
//				if (OnGround == false)
//					ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//				else if (OnGround == true)
//				{
//					if (level_of_walking == 0 && level_of_running == 0)
//						ani = MARIO_ANI_TAIL_IDLE_RIGHT;
//					else if (level_of_walking > 0 || level_of_running > 0)
//					{
//						if (ChangeDirection == false)
//						{
//							ani = MARIO_ANI_TAIL_WALKING_RIGHT;
//							if (isHolding == true)
//								ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//							else if (isMaxRunning == true)
//								ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
//						}
//						else if (ChangeDirection == true)
//							ani = MARIO_ANI_TAIL_STOP_RIGHT;
//
//					}
//				}
//
//			}
//			// trái
//			else
//			{
//				ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//			}
//		}
//		// nhảy
//		else
//		{
//			// phải 
//			if (nx == RIGHT)
//			{
//				if (isMaxRunning == true)// && OnGround == false)
//				{
//					/*if (isFlyingHigh == true)
//						ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT;
//					else
//						ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;*/
//
//					ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT;
//				}
//			}
//			// trái
//			else
//			{
//				if (isMaxRunning == true)
//				{
//					if (isFlyingHigh == true)
//						ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT;
//					else
//						ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//				}
//			}
//		}
//	}
//	else
//	{
//		// chưa nhảy // đang rớt xuống
//		if (vy >= 0)
//		{
//			// đang chạm đất
//			if (OnGround == true)
//			{
//				// đứng yên
//				if (level_of_walking == 0 && level_of_running == 0)
//				{
//					if (ChangeDirection == false)
//					{
//						// phải 
//						if (nx == RIGHT)
//						{
//							ani = MARIO_ANI_TAIL_IDLE_RIGHT;
//							if (isHolding == true)
//								ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
//							else if (isSitDown == true)
//							{
//								ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
//							}
//						}
//						// trái
//						else
//						{
//							ani = MARIO_ANI_TAIL_IDLE_LEFT;
//							if (isHolding == true)
//								ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
//							else if (isSitDown == true)
//							{
//								ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
//							}
//						}
//					}
//					else
//					{
//						if (nx == LEFT)
//							ani = MARIO_ANI_TAIL_STOP_LEFT;
//						else
//							ani = MARIO_ANI_TAIL_STOP_RIGHT;
//					}
//
//
//				}
//				// đi bộ qua phải
//				else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
//				{
//					if (ChangeDirection == false)
//					{
//						ani = MARIO_ANI_TAIL_WALKING_RIGHT;
//						if (isHolding == true)
//							ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//						else if (isMaxRunning == true)
//							ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
//					}
//					else if (ChangeDirection == true)
//						ani = MARIO_ANI_TAIL_STOP_RIGHT;
//
//				}
//				//  đi bộ qua trái
//				else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
//				{
//					if (ChangeDirection == false)
//					{
//						ani = MARIO_ANI_TAIL_WALKING_LEFT;
//						if (isHolding == true)
//							ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
//						else if (isMaxRunning == true)
//							ani = MARIO_ANI_TAIL_RUNNING_LEFT;
//					}
//					else if (ChangeDirection == true)
//						ani = MARIO_ANI_TAIL_STOP_LEFT;
//
//				}
//			}
//			// rớt xuống
//			else
//			{
//				if (isHolding == true)
//				{
//					// phải 
//					if (nx == RIGHT)
//						ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
//					// trái
//					else
//						ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
//				}
//				else
//				{
//					// phải 
//					if (nx == RIGHT)
//					{
//						ani = MARIO_ANI_TAIL_FALLING_RIGHT;
//						if (isFlyingLow == true)
//							ani = MARIO_ANI_TAIL_FLYING_RIGHT;
//					}
//					// trái
//					else
//					{
//						ani = MARIO_ANI_TAIL_FALLING_LEFT;
//						if (isFlyingLow == true)
//							ani = MARIO_ANI_TAIL_FLYING_LEFT;
//					}
//				}
//			}
//		}
//		// nhảy
//		else
//		{
//			if (isHolding == true)
//			{
//				// phải 
//				if (nx == RIGHT)
//					ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
//				// trái
//				else
//					ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
//			}
//			else
//			{
//				// phải 
//				if (nx == RIGHT)
//				{
//					ani = MARIO_ANI_TAIL_JUMP_RIGHT;
//				}
//				// trái
//				else
//				{
//					ani = MARIO_ANI_TAIL_JUMP_LEFT;
//				}
//			}
//		}
//	}
//
//	// đá
//	if (canKick == true)
//	{
//		if (nx == RIGHT)
//			ani = MARIO_ANI_TAIL_KICK_RIGHT;
//		else
//			ani = MARIO_ANI_TAIL_KICK_LEFT;
//	}
//	else if (GoHiddenWorld == true)
//		ani = MARIO_ANI_TAIL_ATTACK_2;
//	else if (isAttacking == true && endAttack == false)
//	{
//		if (nx == RIGHT)
//		{
//			if (time_attack <= TIME_ATTACK)
//				ani = MARIO_ANI_TAIL_ATTACK_1;
//			else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//				ani = MARIO_ANI_TAIL_ATTACK_2;
//			else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//				ani = MARIO_ANI_TAIL_ATTACK_3;
//			else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//				ani = MARIO_ANI_TAIL_ATTACK_4;
//			else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//				ani = MARIO_ANI_TAIL_ATTACK_1;
//		}
//		else
//		{
//			if (time_attack <= TIME_ATTACK)
//			{
//				ani = MARIO_ANI_TAIL_ATTACK_3;
//			}
//			else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//			{
//				ani = MARIO_ANI_TAIL_ATTACK_2;
//			}
//			else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//			{
//				ani = MARIO_ANI_TAIL_ATTACK_1;
//			}
//			else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//			{
//				ani = MARIO_ANI_TAIL_ATTACK_4;
//			}
//			else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//			{
//				ani = MARIO_ANI_TAIL_ATTACK_3;
//			}
//		}
//	}
//
//}
//break;