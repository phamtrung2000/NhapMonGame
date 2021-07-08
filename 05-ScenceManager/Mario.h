#pragma once
#include "GameObject.h"

#define MARIO_FLY_MOVING_SPEED	0.1f 
#define GIA_TOC					0.0019f//0.003f // gia tốc
#define GIA_TOC_RUNNING			0.0008f//0.003f // gia tốc
#define MAX_LEVEL_OF_WALKING	54
#define MAX_LEVEL_OF_RUNNING	98//91

#define MARIO_JUMP_SPEED_FAST		0.27f	//0.25f // chuẩn
#define MARIO_JUMP_SPEED_SLOW		0.18f // chuẩn
#define MARIO_FLY_SPEED				0.1f // chuẩn
#define MARIO_JUMP_DEFLECT_SPEED 0.17f
#define MARIO_GRAVITY			0.0005f
#define MARIO_GRAVITY_FLYING	0.00005f
#define MARIO_DIE_DEFLECT_SPEED	 0.2f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	1
#define MARIO_STATE_WALKING_LEFT	2
#define MARIO_STATE_JUMP			3
#define MARIO_STATE_DIE				4
#define MARIO_STATE_JUMP_LOW		5
#define MARIO_STATE_FLY_LOW			6
#define MARIO_STATE_FLY_HIGH		7

#define MARIO_STATE_SITDOWN			8
#define MARIO_STATE_ATTACK			9
#define MARIO_STATE_RUNNING			10
#define MARIO_STATE_HOLDING_IDLE	11
#define MARIO_STATE_FLYING_HIGH_RIGHT	12
#define MARIO_STATE_FLYING_HIGH_LEFT	13
#define MARIO_STATE_HOLDING_RIGHT		14
#define MARIO_STATE_HOLDING_LEFT		15

#define MARIO_STATE_ENDSCENE			100

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

#define MARIO_ANI_BIG_FALLING_RIGHT		77
#define MARIO_ANI_BIG_FALLING_LEFT		78

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
#define MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG		89
#define MARIO_ANI_UP_LEVEL_LEFT_SMALL_BIG		90

#define MARIO_ANI_FIRE_KICK_RIGHT	91
#define MARIO_ANI_FIRE_KICK_LEFT	92

#define MARIO_ANI_FIRE_HOLD_IDLE_RIGHT		93
#define MARIO_ANI_FIRE_HOLD_WALK_RIGHT		94

#define MARIO_ANI_FIRE_HOLD_IDLE_LEFT		95
#define MARIO_ANI_FIRE_HOLD_WALK_LEFT		96

#define MARIO_ANI_FIRE_HOLD_JUMP_RIGHT		97
#define MARIO_ANI_FIRE_HOLD_JUMP_LEFT		98

#define MARIO_ANI_TAIL_GOHIDDENWORLD		40
#define MARIO_ANI_SMALL_GOHIDDENWORLD		99
#define MARIO_ANI_BIG_GOHIDDENWORLD			100
#define MARIO_ANI_FIRE_GOHIDDENWORLD		101

#define MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_1	31
#define MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_2	40
#define MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_3	32
#define MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_4	42

#define MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_1	102
#define MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_2	103
#define MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_3	104

#define MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_1	105
#define MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_2	106
#define MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_3	107

#define MARIO_ANI_FIRE_FALLING_RIGHT	108
#define MARIO_ANI_FIRE_FALLING_LEFT		109

#define MARIO_ANI_SMALL_JUMP_MAX_SPEED_RIGHT	110
#define MARIO_ANI_SMALL_JUMP_MAX_SPEED_LEFT		111

#define MARIO_ANI_BIG_JUMP_MAX_SPEED_RIGHT		112
#define MARIO_ANI_BIG_JUMP_MAX_SPEED_LEFT		113

#define MARIO_ANI_FIRE_JUMP_MAX_SPEED_RIGHT		114
#define MARIO_ANI_FIRE_JUMP_MAX_SPEED_LEFT		115

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_TAIL	3
#define	MARIO_LEVEL_FIRE	4

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 14//15

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 27//27
#define MARIO_BIG_BBOX_SITDOWN_HEIGHT 17//18 

#define MARIO_TAIL_BBOX_WIDTH  16//21
#define MARIO_TAIL_BBOX_HEIGHT 27//28
#define MARIO_TAIL_BBOX_SITDOWN_HEIGHT 17//18

#define MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT 21//22

#define TIME_ATTACK 4//2
#define TIME_FLY_X 300
#define TIME_FLY_S 300
#define TIME_FLY_PER_S 30
#define MARIO_UNTOUCHABLE_TIME 3000
#define TIME_LEVEL_UP 3000
#define ITIME_LEVEL_UP 100//180
#define LEVEL_UP_DIVIDE	15

#define TIMEDELAYUSEBFIREBULLET_A	 500
#define TIMEDELAYUSEBFIREBULLET_Z	 35

class Mario : public CGameObject
{
private:
	static Mario* __instance;
public:
	// trạng thái bất tử
	bool untouchable; // trạng thái bất tử
	ULONGLONG untouchable_start; // thời điểm bắt đầu bất tử
	int UntouchtableTime;
	
	bool test;

	float start_x, start_y;		// vị trí bắt đầu
	float NewX, NewY; // vị trí mới sau khi rời khỏi map ẩn
	float MaxY; // xử lý TH y+ quá mức và mario rớt xuyên xuống đất/ vật thể

	bool OnGround; // chạm đất
	bool isRunning; 
	int	 level_of_walking, level_of_running; // phải chia ra làm 2 bởi vì sẽ bị TH sau khi thả nút A thì thanh speed của HUD giảm dần nhưng nếu vẫn di chuyển thì nó sẽ giảm còn 3 nấc thì k giảm nữa
	bool ChangeDirection; // chuyển hướng, để hiện ani khựng lại quay đầu
	bool StopRunning; // ban đầu = true, nếu vào state running thì = false, đến khi speed = 0 thì = true lại, mục đích là để xử lý vụ hiện thanh speed của HUD giảm dần sau khi thả nút A chứ không biến mất
	bool isMaxRunning;
	
	bool isFalling, // đang rớt xuống
		isSitDown, // đang ngồi
		GoHiddenWorld; // đi vào map ẩn để hiện ani xuống cống
	
	bool isAttacking, // đang ở state attack
		endAttack; // thực hiện xong động tác quật đuôi thì mới cho phép quật đuôi tiếp
	int time_attack; // biến đếm thời gian thực hiện động tác quật đuôi để hiện ani tương ứng
	int NumberBullet; // số lượng đạn ( 2 viên )
	ULONGLONG TimeUseFireBullet, // thời điểm dùng đạn lửa
		TimeDelayUseFireBullet; // khoảng thời gian chờ khi ném đạn lửa
	
	int ani;
	bool isHolding = false, pressA = false, canKick = false;
	int Height,Width;

	bool render_tail; // Đã vẽ đuôi hay chưa để thoát vòng lặp vẽ đuôi bên playscene
	// BAY (FLY)
	bool canFlyX, canFlyS, // mario có thể bay khi đạt max running và bay lên thì level running không trừ cho đến hết tgian bay
		isFlyingLow, isFlyingHigh; // biến để xác định mario đang ở state fly
	ULONGLONG TimeDelayFly; // khoảng thời gian chờ khi bay bằng S
	int time_fly, FlyTimePerS; // thời gian bay, hết thời gian thì rớt xuống
		 
	// TĂNG CẤP (UP LEVEL)
	ULONGLONG ChangeLevelTime; // thời điểm bắt đầu tăng cấp
	bool isLevelUp; // mario tăng cấp để hiện effect tăng cấp
	bool isLevelDown; // mario giảm cấp để hiện effect giảm cấp
	int iChangeLevelTime; // Thời gian tăng cấp + hiện effect tăng cấp
	int level; // cấp độ

	ULONGLONG TimeJumpS; // thời điểm nhảy bằng nút S để làm vụ thả S thì rớt xuống 
	bool pressS;

	// tăng điểm theo cấp số nhân khi mario nhảy chạm đầu giết quái liên tục + k chạm đất
	// 1: x1, 2: x2, 3: x4,4: x8,5: x10,6: x20,7 : x40, 8: x80,9 : +1M
	int nScore;
	
	// Kiểm tra trạng thái mario k điều khiển được khi Up/Down LV, khi hết scene,.....
	bool loseControl;
	ULONGLONG StartGoHiddenWorld;
	int Scene;

	ULONGLONG StartToDie; // khi mario chết có 1 khoảng tg mario đứng yên rồi mới nhảy lên

	bool isBlocked; // kiểm tra mario có đang bị chăn bởi object để tăng level of walking
	float RightOld; // lưu lại giá trị right trước khi up level để sau khi up level right k bị tăng lên dẫn đến lỗi đi xuyên vật thể

	int NumberSmallGoomba; // số lượng nấm nhỏ bu lên người
	int jump_count_X, jump_count_S; // đếm số lần nhảy để thoát khỏi mấy con nấm nhỏ bu lên người

	bool isPushed, canPush; // mario bị đẩy khi cụng cục hidden music brick
	bool hasFly; // vừa bay : mario vừa bay và bắt đầu rớt xuống, mục đích : xử lý bên camera vụ phân biệt khi mario rớt xuống vs nhảy tại chỗ
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
	void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny, float rdx, float rdy);
	void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny);
	float GetWidth(int level);
};

