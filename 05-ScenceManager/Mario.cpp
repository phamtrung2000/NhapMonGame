#include "Mario.h"
#include "PlayScence.h"
#include "FireBullet.h"
#include "Brick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include "Block.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "QuestionBrickItem.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "FirePiranhaPlant.h"
#include "BrickItem.h"
#include "ItemBrick.h"
#include "GreenFirePlant.h"
#include "GreenPlant.h"
#include "GreenFlyKoopas.h"
#include "Coin.h"
#include "GreenKoopas.h"
#include "EffectSmoke.h"
#include "MarioTail.h"
#include "ButtonP.h"
#include "EffectScore.h"
#include "Card.h"
#include "BoomerangWeapon.h"
#include "Notification.h"
#include "BoomerangEnemy.h"
#include "RedFlyKoopas.h"
#include "ListItemBrick.h"
#include "ListQuestionBrick.h"
#include "FireBullet.h"
#include "EffectSmoke.h"
#include "FlyWood.h"
#include "ListNormalBrick.h"
#include "HiddenMusicBrick.h"
#include "SmallGoomba.h"

Mario* Mario::__instance = NULL;

Mario* Mario::GetInstance()
{
	if (__instance == NULL)
		__instance = new Mario();
	return __instance;
}

Mario::Mario(float x, float y) : CGameObject()
{
	ObjType = OBJECT_TYPE_MARIO;
	Category = CATEGORY::PLAYER;
	level = MARIO_LEVEL_SMALL;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	GoHiddenWorld = untouchable = ChangeDirection = isRunning = isMaxRunning
	= isFlyingHigh = canFlyX = canFlyS = isFalling = isSitDown = isAttacking
	= endAttack = isLevelUp = test = render_tail = pressS = isLevelDown = loseControl
	= isBlocked = false;

	OnGround = IsMovingObject = StopRunning = true;

	level_of_walking = level_of_running = jump_count_X = jump_count_S = 0;
	nScore = 0;
	time_attack = time_fly = iChangeLevelTime = FlyTimePerS = 0;
	TimeJumpS = 0;
	ani = 0;
	NumberBullet = 2;
	untouchable_start = ChangeLevelTime = StartToDie = 0;
	NumberSmallGoomba = UntouchtableTime = 0;
	MaxY = RightOld = 0.0f;
	
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
	{
		Width = MARIO_SMALL_BBOX_WIDTH;
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		if (isLevelUp == true)
		{
			//if (GetTickCount64() - ChangeLevelTime < (TIME_LEVEL_UP / 2) - 1)
			if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
			{
				if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
					bottom = y + MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT;
			}
			else
			{
				if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
					bottom = y + MARIO_BIG_BBOX_HEIGHT;
				else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
					bottom = y + MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT;
			}
		}
		else if (isLevelDown == true)
		{
			//if (GetTickCount64() - ChangeLevelTime < (TIME_LEVEL_UP / 2) - 1)
			if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
			{
				if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
					bottom = y + MARIO_BIG_BBOX_HEIGHT;
				else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
					bottom = y + MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT;
			}
			else
			{
				if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
					bottom = y + MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT;
			}
		}

	}break;

	case MARIO_LEVEL_BIG:
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		Width = MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isSitDown == true && GoHiddenWorld == false)
		{
			bottom = y + MARIO_BIG_BBOX_SITDOWN_HEIGHT;
		}
	}break;

	case MARIO_LEVEL_TAIL:
	{
		if (nx == LEFT)
		{
			left = x;
			top = y;
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_TAIL_BBOX_HEIGHT;
		}
		else
		{
			left = x + MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH ;
			top = y;
			right = x + MARIO_TAIL_BBOX_WIDTH;
			bottom = y + MARIO_TAIL_BBOX_HEIGHT;
		}
		if (isSitDown == true && GoHiddenWorld == false)
		{
			bottom = y + MARIO_TAIL_BBOX_SITDOWN_HEIGHT;
		}
	}break;

	case MARIO_LEVEL_FIRE:
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		Width = MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isSitDown == true && GoHiddenWorld == false)
		{
			bottom = y + MARIO_BIG_BBOX_SITDOWN_HEIGHT;
		}
	}break;
	}
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isLevelUp == true)
	{
		iChangeLevelTime++;
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
		{
			if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
			{
				if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0 && test == false)
				{
					y -= static_cast<__int64>(MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
					test = true;
				}
				else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0 && test == true)
				{
					y += static_cast<__int64>(MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
					test = false;
				}
			}
			else
			{
				if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0 && test == false)
				{
					y += static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT);
					test = true;
				}
				else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0 && test == true)
				{
					y -= static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT);
					test = false;
				}
			}

			if (iChangeLevelTime >= ITIME_LEVEL_UP)
			{
				isLevelUp = test = false;
				_PlayScene->Stop = false;
				loseControl = false; // hủy trạng thái k điều khiển được sau hiệu ứng tăng LV
				iChangeLevelTime = 0;
				ChangeLevelTime = 0;
				level++;
			}
		}break;

		case MARIO_LEVEL_BIG:
		{
			if (ChangeLevelTime != 0 && GetTickCount64() - ChangeLevelTime >= EFFECTSMOKE_APPEARTIME) // hết thời gian hiệu ứng tăng cấp
			{
				_PlayScene->Stop = false;
				isLevelUp = test = false;
				loseControl = false; // hủy trạng thái k điều khiển được sau hiệu ứng tăng LV
				iChangeLevelTime = 0;
				ChangeLevelTime = 0;
				level++;
				y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
			}
		}break;

		case MARIO_LEVEL_TAIL:
		{
			if (ChangeLevelTime != 0 && GetTickCount64() - ChangeLevelTime >= EFFECTSMOKE_APPEARTIME) // hết thời gian hiệu ứng tăng cấp
			{
				_PlayScene->Stop = false;
				isLevelUp = test = false;
				loseControl = false; // hủy trạng thái k điều khiển được sau hiệu ứng tăng LV
				iChangeLevelTime = 0;
				ChangeLevelTime = 0;
				level++;
				y += static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);

			}
		}break;
		}
		y = (float)static_cast<int>(y);
		if (nx == RIGHT)
			x = RightOld - GetWidth(level);
	}
	else if (isLevelDown == true)
	{
		iChangeLevelTime++;
		switch (level)
		{
		case MARIO_LEVEL_BIG:
		{
			if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
			{
				if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0 && test == false)
				{
					y += static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT);
					test = true;

				}
				else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0 && test == true)
				{
					y -= static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT);
					test = false;
				}
			}
			else
			{
				if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0 && test == false)
				{
					y -= static_cast<__int64>(MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
					test = true;
				}
				else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0 && test == true)
				{
					y += static_cast<__int64>(MARIO_LEVEL_UP_SMALL_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
					test = false;
				}
			}

			if (iChangeLevelTime >= ITIME_LEVEL_UP)
			{
				isLevelDown = test = false;
				_PlayScene->Stop = false;
				loseControl = false; // hủy trạng thái k điều khiển được sau hiệu ứng giảm LV
				iChangeLevelTime = 0;
				ChangeLevelTime = 0;
				level--;
				StartUntouchable();
			}
		}break;

		case MARIO_LEVEL_TAIL:
		{
			if (ChangeLevelTime != 0 && GetTickCount64() - ChangeLevelTime >= EFFECTSMOKE_APPEARTIME) // hết thời gian hiệu ứng tăng cấp
			{
				_PlayScene->Stop = false;
				isLevelDown = test = false;
				loseControl = false; // hủy trạng thái k điều khiển được sau hiệu ứng giảm LV
				iChangeLevelTime = 0;
				ChangeLevelTime = 0;
				level = MARIO_LEVEL_BIG;
				y += static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
				StartUntouchable();
			}
		}break;

		case MARIO_LEVEL_FIRE:
		{
			if (ChangeLevelTime != 0 && GetTickCount64() - ChangeLevelTime >= EFFECTSMOKE_APPEARTIME) // hết thời gian hiệu ứng tăng cấp
			{
				_PlayScene->Stop = false;
				isLevelDown = test = false;
				loseControl = false; // hủy trạng thái k điều khiển được sau hiệu ứng giảm LV
				iChangeLevelTime = 0;
				ChangeLevelTime = 0;
				level = MARIO_LEVEL_BIG;
				y -= static_cast<float>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
				StartUntouchable();
			}
		}break;
		}
		y = (float)(static_cast<int>(y));
	}
	else
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

		if (GetState() == MARIO_STATE_DIE)
		{
			if (GetTickCount64() - StartToDie > 500 && StartToDie != 0)
			{
				vy = -MARIO_DIE_DEFLECT_SPEED;
				StartToDie = 0;
			}
			if (TimeToDie == 0 && vy != 0)
			{
				vy += MARIO_GRAVITY * dt;
				y += dy;
			}
			if (y > _Map->GetHeight())
			{
				CGame::GetInstance()->SwitchScene2(1);
				_HUD->MarioLife--;
				_PlayScene->Stop = false;
			}
			return;
		}
		

		if (canFlyX == true)
		{
			time_fly++;
			// hết thời gian bay
			if (time_fly > TIME_FLY_X)
			{
				time_fly = 0;
				canFlyX = isFlyingHigh = false;
				vy = 0;
				SetState(MARIO_STATE_IDLE);
			}
		}
		else if (canFlyS == true)
		{
			time_fly++;
			FlyTimePerS++;
			if (time_fly > TIME_FLY_S)
			{
				time_fly = 0;
				isMaxRunning = canFlyS = isFlyingHigh = false;
				vy = 0;
				SetState(MARIO_STATE_IDLE);
			
			}
		}

		// Gravity
		if (GoHiddenWorld == false) // để mario rớt từ từ khi đi vào map ẩn
		{
			if (level == MARIO_LEVEL_TAIL)
			{
				if (isFlyingLow == false && isFlyingHigh == false) // rớt bình thường
					vy += MARIO_GRAVITY * dt;
				else if (isFlyingLow == true)
				{
					// rớt chậm khi vẫy đuôi
					vy += MARIO_GRAVITY_FLYING * dt;
					if (TimeDelayFly != 0 && GetTickCount64() - TimeDelayFly > 150)
					{
						isFlyingLow = false;
						TimeDelayFly = 0;
					}
				}
				else if (isFlyingHigh == true && canFlyS == true)
				{
					if (FlyTimePerS > TIME_FLY_PER_S)
					{
						isFlyingHigh = false;
						vy += MARIO_GRAVITY * dt;
					}
				}
			}
			else
			{
				if (OnGround == false && isMaxRunning == true)
				{
					vy += 0.0004f * dt;
				}
				else
					vy += MARIO_GRAVITY * dt;
			}
		}
		else
		{
			if (StartGoHiddenWorld != 0 && GetTickCount64() - StartGoHiddenWorld > 1000)
			{
				CGame::GetInstance()->SwitchScene(Scene);
				return;
			}
		}

		if (OnGround == false)
		{
			if (pressS == false && TimeJumpS != 0 && GetTickCount64() - TimeJumpS > 100 && isFalling == false)
			{
				this->vy /= 2;
				TimeJumpS = 0;
			}
			if (nScore >= 9)
				nScore = 0;
		}
		else
		{
			// reset lại khi chạm đất
			if (TimeJumpS != 0)
				TimeJumpS = 0;
		}

	
		if (isAttacking == true) // reset lại isAttacking và hiện ani quất đuôi khi isAttacking=true
		{
			// khi ani quất đuôi hiện lên thì mới bắt đầu tính, tránh trường hợp ani chưa bằng MARIO_ANI_TAIL_ATTACK_1 nhưng time vẫn + lên bằng 1
			//if (ani >= MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_1)
			{
				if (level == MARIO_LEVEL_TAIL)
				{
					if (time_attack < 5 * TIME_ATTACK)
					{
						time_attack++;
					}
					else if (time_attack >= 5 * TIME_ATTACK)
					{
						time_attack = 0;
						isAttacking = false;
					}
				}
				else
				{
					if (OnGround == true)
					{
						if (time_attack < 2 * TIME_ATTACK)
							time_attack++;
						else if (time_attack >= 2 * TIME_ATTACK)
						{
							time_attack = 0;
							isAttacking = false;
						}
					}
					else if (OnGround == false)
					{
						if (time_attack < 3 * TIME_ATTACK)
							time_attack++;
						else if (time_attack >= 3 * TIME_ATTACK)
						{
							time_attack = 0;
							isAttacking = false;
						}
					}
				}
			}
		}

		if (TimeUseFireBullet != 0)
		{
			if (GetTickCount64() - TimeUseFireBullet > TimeDelayUseFireBullet)
			{
				TimeUseFireBullet = 0;
			}
		}

		if (canKick == true) // reset lại canKick và hiện ani đá khi canKick=true
		{
			time_attack++;
			if (time_attack >= 3 * TIME_ATTACK)
			{
				canKick = false;
				time_attack = 0;
			}
		}

		if (GetState() == MARIO_STATE_IDLE)
		{
			// level walking = 0 thì vx = 0 nhưng chưa  chắc level running = 0 nên vẫn phải xét tiếp level running
			if (level_of_walking == 0)
			{
				vx = 0.0f;
				ChangeDirection = false;
			}
			
			// đang trong trạng thái bay thì k trừ runing nhưng walking vẫn trừ
			if (level == MARIO_LEVEL_TAIL)
			{
				if ((nx == RIGHT && vx > 0.0f) || (nx == LEFT && vx < 0.0f))
				{
					// đi bộ
					if (level_of_walking > 0)
					{
						level_of_walking -= 2;
						vx += -nx * 2 * GIA_TOC;
						if (level_of_walking < 0)
							level_of_walking = 0;
					}
				}
				else if ((nx == RIGHT && vx < 0.0f) || (nx == LEFT && vx > 0.0f))
				{
					// đi bộ
					if (level_of_walking > 0)
					{
						level_of_walking -= 2;
						vx += nx * 2 * GIA_TOC;
						if (level_of_walking < 0)
							level_of_walking = 0;
					}
				}

				if (canFlyX == false && canFlyS == false)
				{
					if (level_of_running > 0)
					{
						level_of_running--;
					}
				}
			}
			else
			{
				// chạy k max thì dù nhảy hay k nhảy vẫn trừ level running
				if ((isMaxRunning == false) ||
					// đang max running : trên không thì k trừ
					// chạm đất + nếu k giữ nút A thì trừ
					(isMaxRunning == true  && isRunning == false))
				{
					if (level_of_running > 0)
					{
						level_of_running--;
					}
					if ((nx == RIGHT && vx > 0.0f) || (nx == LEFT && vx < 0.0f))
					{
						// đi bộ
						if (level_of_walking > 0)
						{
							level_of_walking-=2;
							vx += -nx * 2 * GIA_TOC;
							if (level_of_walking < 0)
								level_of_walking = 0;
						}
					}
					else if ((nx == RIGHT && vx < 0.0f) || (nx == LEFT && vx > 0.0f))
					{
						// đi bộ
						if (level_of_walking > 0)
						{
							level_of_walking -= 2;
							vx += nx * 2 * GIA_TOC;
							if (level_of_walking < 0)
								level_of_walking = 0;
						}
					}
				}
			}

			if (isSitDown == true && GoHiddenWorld == false)
			{
				isSitDown = false;
				if(level != MARIO_LEVEL_SMALL)
					y = y - 10;
			}

			if (isRunning == false && level_of_running == 0)
				StopRunning = true;


		}
		else if (GetState() == MARIO_STATE_HOLDING_IDLE)
		{
			if (level_of_walking == 0 )
			{
				vx = 0.0f;
				ChangeDirection = false;
				// đang trong trạng thái bay thì k trừ 
				if (level_of_running > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_running -= 2;
					if (level_of_running < 0)
						level_of_running = 0;
				}
				if (level_of_walking > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_walking--;
					if (level_of_walking < 0)
						level_of_walking = 0;
				}
			}
			else if (nx == RIGHT)
			{
				// đang trong trạng thái bay thì k trừ 
				if (level_of_running > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_running -= 2;
					if (level_of_running < 0)
						level_of_running = 0;

					if (OnGround == true)
					{
						vx -= GIA_TOC * 2;
					}

					if (vx < 0.0f)
						vx = 0.0f;
				}
				// đi bộ
				else if (level_of_walking > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_walking--;
					if (level_of_walking < 0)
						level_of_walking = 0;
					vx -= GIA_TOC;
					if (vx < 0.0f)
						vx = 0.0f;
				}
			}
			else if (nx == LEFT)
			{
				// đang trong trạng thái bay thì k trừ 
				if (level_of_running > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_running -= 2;
					if (level_of_running < 0)
						level_of_running = 0;

					if (OnGround == true)
					{
						vx += GIA_TOC * 2;
					}

					if (vx > 0.0f)
						vx = 0.0f;
				}
				// đi bộ
				else if (level_of_walking > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_walking--;
					if (level_of_walking < 0)
						level_of_walking = 0;
					vx += GIA_TOC;
					if (vx > 0.0f)
						vx = 0.0f;
				}
			}
		}
		else if (GetState() == MARIO_STATE_WALKING_RIGHT || GetState() == MARIO_STATE_WALKING_LEFT)
		{
			// đi bộ
			if (isRunning == false)
			{
				// chưa đạt max + k bị chặn bởi vật thể thì ++
				if (level_of_walking < MAX_LEVEL_OF_WALKING)
					level_of_walking++;

				// sau khi thả nút A thì trừ level_of_running, tránh trường hợp đợi đến state IDLE mới trừ
				if (level_of_running > 0)
				{
					// Đang tăng tốc mà level_of_running > MAX_LEVEL_OF_WALKING thì sau khi thả nút A level_of_walking = MAX_LEVEL_OF_WALKING để tránh TH bị chênh lệch tốc độ
					if (level_of_running > MAX_LEVEL_OF_WALKING)
						level_of_walking = MAX_LEVEL_OF_WALKING;

					if (canFlyX == false && canFlyS == false)
						level_of_running -= 2;
					// Phòng TH level of running = 1 thì sẽ thành -1
					if (level_of_running < 0)
						level_of_running = 0;
				}

				// đang đi qua bên trái (vx < 0) thì đột ngột qua phải -> khúc này xử lý vụ khựng lại r chuyển hướng
				if ((GetState() == MARIO_STATE_WALKING_RIGHT && vx < 0) ||
					(GetState() == MARIO_STATE_WALKING_LEFT && vx > 0))
				{
					// bắt đầu chuyển hướng di chuyển
					ChangeDirection = true;

					if (level_of_walking > 0)
					{
						level_of_walking -= 2;
						if (level_of_walking < 0)
							level_of_walking = 0;
					}

					// cộng thêm vx chứ không gán trực tiếp để nó hiện animation khựng
					// vx -= (level_of_speed * GIA_TOC); ->không nhân với level để vx giảm chậm để hiện animation
					vx += nx * (2 * GIA_TOC);

					// vx đạt tới mức di chuyển sang trái thì hủy chuyển hướng để trở về animation WALKING_LEFT
					// nếu không thì vẫn sẽ là animation STOP_RIGHT đi về bên trái
					if ((GetState() == MARIO_STATE_WALKING_RIGHT && vx >= 0) ||
						(GetState() == MARIO_STATE_WALKING_LEFT && vx <= 0))
						ChangeDirection = false;
				}
				// đang đi qua phải bình thường
				else
				{
					ChangeDirection = false;
					if (level_of_walking >= 4)
						vx = (float)(nx * level_of_walking * GIA_TOC);
				}
			}
			// chạy
			else
			{
				// chưa đạt max + k bị chặn bởi vật thể thì ++
				if (level_of_walking < MAX_LEVEL_OF_WALKING)
					level_of_walking++;
				
				// nhảy lên trời thì level_of_running giảm
				if (OnGround == false)
				{
					if (level_of_running > 0 && isMaxRunning == false)
						level_of_running--;
				}
				// dưới đất
				else
				{
					if (isBlocked == false)
					{
						if (level_of_running < MAX_LEVEL_OF_RUNNING)
							level_of_running++;
					}
					else
					{
						if (level_of_running > 0 && canFlyX == false && canFlyS == false)
						{
							level_of_running--;
						}
					}
				}

				// đang đi qua bên trái (vx < 0) thì đột ngột qua phải -> khúc này xử lý vụ khựng lại r chuyển hướng
				if ((GetState() == MARIO_STATE_WALKING_RIGHT && vx < 0) ||
					(GetState() == MARIO_STATE_WALKING_LEFT && vx > 0))
				{
					// đang cầm rùa thì không chuyển hướng vì không hiện ani khựng
					if (isHolding != true)
						ChangeDirection = true;


					if (level_of_running > 0 && canFlyX == false && canFlyS == false)
					{
						level_of_running -= 3;
					}

					// trừ bớt vx chứ không gán trực tiếp để nó hiện animation khựng
					// vx -= (level_of_speed * GIA_TOC); ->không nhân với level để vx giảm chậm để hiện animation
					vx +=  float(nx * 4 * GIA_TOC);

					// vx đạt tới mức di chuyển sang trái thì hủy chuyển hướng để trở về animation WALKING_LEFT
					// nếu không thì vẫn sẽ là animation STOP_RIGHT đi về bên trái
					if ((GetState() == MARIO_STATE_WALKING_RIGHT && vx >= 0) ||
						(GetState() == MARIO_STATE_WALKING_LEFT && vx <= 0))
						ChangeDirection = false;
				}
				// đang chạy qua phải bình thường
				else
				{
					ChangeDirection = false;
					vx = float(nx * (level_of_walking * GIA_TOC + level_of_running * GIA_TOC_RUNNING));
				}
			}

			if (isSitDown == true && GoHiddenWorld == false)
			{
				isSitDown = false;
				if (level != MARIO_LEVEL_SMALL)
					y = y - 10;
			}

		}
		else if (GetState() == MARIO_STATE_HOLDING_RIGHT)
		{
			if (OnGround == false)
			{
				// đang state running mà nhảy lên thì speed bị trừ
				if (level_of_running > MAX_LEVEL_OF_RUNNING / 2 && isMaxRunning == false)
					level_of_running--;
			}
			else
			{
				if (level_of_running < MAX_LEVEL_OF_RUNNING)
					level_of_running++;
			}

			// đang đi bên trái -> khựng
			if (vx < 0)
			{
				if (level_of_running > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_running -= 4;
					if (level_of_running < 0)
						level_of_running = 0;
				}

				// trừ bớt vx chứ không gán trực tiếp để nó hiện animation khựng
				// vx -= (level_of_speed * GIA_TOC); ->không nhân với level để vx giảm chậm để hiện animation
				vx += float(4 * GIA_TOC);
			}
			// đang đứng yên / di chuyển sang phải bình thường
			else
			{
				vx = (float)(nx * level_of_running * GIA_TOC);
			}
		}
		else if (GetState() == MARIO_STATE_HOLDING_LEFT)
		{
			if (OnGround == false)
			{
				// đang state running mà nhảy lên thì speed bị trừ
				if (level_of_running > MAX_LEVEL_OF_RUNNING / 2 && isMaxRunning == false)
					level_of_running--;
				else
					level_of_running++;
			}
			else
			{
				if (level_of_running < MAX_LEVEL_OF_RUNNING)
					level_of_running++;
			}

			// đang đi bên trái -> khựng
			if (vx > 0)
			{
				if (level_of_running > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_running -= 4;
					if (level_of_running < 0)
						level_of_running = 0;
				}

				// trừ bớt vx chứ không gán trực tiếp để nó hiện animation khựng
				// vx -= (level_of_speed * GIA_TOC); ->không nhân với level để vx giảm chậm để hiện animation
				vx -= float(4 * GIA_TOC);
			}
			// đang đứng yên / di chuyển sang phải bình thường
			else
			{
				vx = (float)(nx * level_of_running * GIA_TOC);
			}
		}
		else if (GetState() == MARIO_STATE_SITDOWN)
		{
			if (level_of_walking == 0 )
			{
				vx = 0;
				ChangeDirection = false;
			}
			else if (nx == RIGHT)
			{
				if (level_of_running > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_running -= 2;
					if (level_of_running < 0)
						level_of_running = 0;
					vx -= 2 * GIA_TOC;
					if (vx < 0.0f)
						vx = 0.0f;
				}
				// đi bộ
				else if (level_of_walking > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_walking--;
					if (level_of_walking < 0)
						level_of_walking = 0;
					vx -= GIA_TOC;
					if (vx < 0.0f)
						vx = 0.0f;
				}
			}
			else if (nx == LEFT)
			{
				if (level_of_running > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_running -= 2;
					if (level_of_running < 0)
						level_of_running = 0;
					vx += 2 * GIA_TOC;
					if (vx > 0.0f)
						vx = 0.0f;
				}
				else if (level_of_walking > 0 && canFlyX == false && canFlyS == false)
				{
					level_of_walking--;
					if (level_of_walking < 0)
						level_of_walking = 0;

					vx += GIA_TOC;
					if (vx > 0.0f)
						vx = 0.0f;
				}
			}
		}
		else if (GetState() == MARIO_STATE_ENDSCENE)
		{
			if (OnGround == true)
			{
				level_of_walking = 10;
				vx = level_of_walking * GIA_TOC;
				nx = RIGHT;
				if (this->x + Width >= _Map->GetWidth() - 20)
				{
					CGame::GetInstance()->SwitchScene(1);
					return;
				}
			}
		}

		if (level_of_running == MAX_LEVEL_OF_RUNNING) // đạt trạng thái max running
		{
			isMaxRunning = true;
		}
		else // đặt lại maxrunning = false khi level running k đủ max
		{
			if (isMaxRunning == true)
			{
				if (canFlyS == false && canFlyX == false)
					isMaxRunning = false;
			}
		}

		// Tạo đạn lửa khi _Mario bắn lửa
		if (this->level == MARIO_LEVEL_FIRE && this->isAttacking == true)
		{
			if ((this->OnGround == true && this->time_attack > TIME_ATTACK && this->time_attack <= 2 * TIME_ATTACK)
			|| (this->OnGround == false && this->time_attack <= TIME_ATTACK))
			{
				if (this->TimeDelayUseFireBullet == TIMEDELAYUSEBFIREBULLET_A)
				{
					// kiểm soát số đạn <=2
					if (this->NumberBullet <= 2 && this->NumberBullet > 0 && this->TimeUseFireBullet == 0)
					{
						//DebugOut(L"level=%i , number = %i \n", this->level, this->NumberBullet);
						FireBullet* fb = new FireBullet(this->x + 5, this->y);
						fb->FireMario = true;
						// chiều của viên đạn

						CAnimationSets* animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
						fb->SetAnimationSet(ani_set);
						_Grid->AddMovingObject(fb, this->x + 5, this->y);
						this->NumberBullet--;
						// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
						//this->isAttacking = false;
						this->TimeUseFireBullet = GetTickCount64();
						//	DebugOut(L"Them dan lua -> objects.size = %i \n", objects.size());
					}
				}
				else if (this->TimeDelayUseFireBullet == TIMEDELAYUSEBFIREBULLET_Z)
				{
					// kiểm soát số đạn <=2
					if (this->NumberBullet <= 2 && this->NumberBullet > 0 && (this->TimeUseFireBullet == 0 || GetTickCount64() - this->TimeUseFireBullet > this->TimeDelayUseFireBullet))
					{
						//DebugOut(L"level=%i , number = %i \n", this->level, this->NumberBullet);
						FireBullet* fb = new FireBullet(this->x + 5, this->y);
						fb->FireMario = true;
						// chiều của viên đạn

						CAnimationSets* animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
						fb->SetAnimationSet(ani_set);
						_Grid->AddMovingObject(fb, this->x + 5, this->y);
						this->NumberBullet--;
						// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
						//this->isAttacking = false;
						this->TimeUseFireBullet = GetTickCount64();

					}
				}
			}
		}

		// tạo object đuôi(MarioTail) khi this quật đuôi, xóa object khi thực hiện xong hành động quật đuôi
		else if (this->level == MARIO_LEVEL_TAIL && this->render_tail == false)
		{
			//DebugOut(L" ani = %i, time %i\n", this->ani, this->time_attack);
			MarioTail* tail = new MarioTail(this->x, this->y + 18);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(MARIOTAIL_ANI_SET_ID);
			tail->SetAnimationSet(ani_set);
			_Grid->AddMovingObject(tail, this->x, this->y + 18);
			this->render_tail = true;
		}


		switch (level)
		{
			case MARIO_LEVEL_SMALL:
			{
				Height = MARIO_SMALL_BBOX_HEIGHT;
				render_tail = canFlyS = canFlyX = isFlyingHigh = isFlyingLow = false;
			}break;
			case MARIO_LEVEL_BIG: case MARIO_LEVEL_FIRE:
			{
				Height = MARIO_BIG_BBOX_HEIGHT;
				render_tail = canFlyS = canFlyX = isFlyingHigh = isFlyingLow = false;
			}break;
			case MARIO_LEVEL_TAIL:
			{
				Height = MARIO_TAIL_BBOX_HEIGHT;
			}break;
		}

		if (vx < 0 && x < 0)
		{
			x = 0;
		}
		else
		{
			if (_Mario->x + _Mario->Width >= _Map->GetWidth() - 16)
				_Mario->x = (float)(_Map->GetWidth() - 16 - _Mario->Width);
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		// turn off collision when die 
		if (GetState() != MARIO_STATE_DIE)
			CalcPotentialCollisions(coObjects, coEvents);

		// reset untouchable timer if untouchable time has passed
		if (untouchable == true && untouchable_start != 0)
		{
			UntouchtableTime++;
			if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
			{
				untouchable_start = 0;
				UntouchtableTime = 0;
				untouchable = false;
			}
		}

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				switch (coObjects->at(i)->Category)
				{
					case CATEGORY::ITEM:
					{
						// lấy render box của 2 obj để kiểm tra xem chúng có nằm bên trong nhau hay không
						if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true)
						{

							if (coObjects->at(i)->ObjType == OBJECT_TYPE_COIN)
							{
								coObjects->at(i)->canDelete = true;
								_HUD->UpdateScore(coObjects->at(i), 0);
							}
							else if (coObjects->at(i)->ObjType == OBJECT_TYPE_QUESTIONBRICKITEM)
							{
								QuestionBrickItem* questionbrickitem = dynamic_cast<QuestionBrickItem*>(coObjects->at(i));
								questionbrickitem->canDelete = true;
								_HUD->UpdateScore(questionbrickitem, 0);
								if (questionbrickitem->Item >= this->level)
									UpLevel();
							}
							else if (coObjects->at(i)->ObjType == OBJECT_TYPE_BRICKITEM)
							{
								BrickItem* brickitem = dynamic_cast<BrickItem*>(coObjects->at(i));

								switch (brickitem->Item)
								{
								case MUSHROOM:
								{
									brickitem->canDelete = true;
									_HUD->UpdateScore(brickitem, nScore);
								}
								break;
								}
							}
						}
					}
					break;

					case CATEGORY::ENEMY:
					{
						// trường hợp cầm rùa và rùa hồi sinh di chuyển va chạm
						if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true)
						{
							Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));
							if (enemy->EnemyType == ENEMY_TYPE_KOOPAS)
							{
								Koopas* koopas = dynamic_cast<Koopas*>(enemy);
								if ((koopas->GetState() == ENEMY_STATE_WALKING_LEFT || koopas->GetState() == ENEMY_STATE_WALKING_RIGHT) && isHolding == true)
								{
									if (untouchable == false)
										DownLevel();
									isHolding = false;
								}
							}
							else if (enemy->ObjType == OBJECT_TYPE_SMALLGOOMBA)
							{
								SmallGoomba* goomba = dynamic_cast<SmallGoomba*>(enemy);
								if (goomba->isDie != true)
								{
									
									if (goomba->state != SMALLGOOMBA_STATE_FOLLOWMARIO)
									{
										NumberSmallGoomba++;
										goomba->SetState(SMALLGOOMBA_STATE_FOLLOWMARIO);
									}
										
								}
							}
						}
					}
					break;

					case CATEGORY::OBJECT:
					{
						if (coObjects->at(i)->ObjType != OBJECT_TYPE_BLOCK && OnGround == true && GoHiddenWorld == false)					
						{
							if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true)
							{
								RECT rect1 = this->GetRect();
								RECT rect2 = coObjects->at(i)->GetRect();
								{
									if (this->nx == RIGHT)
									{
										if (rect1.left < rect2.left) // mario ở bên trái vật thể
										{
											if (rect1.right > rect2.left && rect1.left < rect2.right)
											{
												x = x - 3;
											}
										}
										else // mario ở bên phải vật thể
										{
											if (rect1.left < rect2.right && rect1.right > rect2.left)
											{
												x = x + 3;
											}
										}
									}
									else
									{
										if (rect1.left < rect2.left) // mario ở bên trái vật thể
										{
											if (rect1.right > rect2.left && rect1.left < rect2.right)
											{
												x = x - 3;
											}
										}
									}
								}
							}
						}
					
					}
					break;

				}
			}
			x += dx;
			y += dy;
			// có đk này để tránh vụ đang rớt xuống khi đi hết vật thể nhưng vẫn nhảy được
			if (this->vy > 0.1f)
			{
				isFalling = true;
				OnGround = false;
			}
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

				if (e->obj)
				{
					switch (e->obj->Category)
					{
					case CATEGORY::OBJECT:
						if (e->obj->ObjType == OBJECT_TYPE_FLYWOOD)
						{
							if (ny != 0) vy = 0;
							FlyWood* flywood = dynamic_cast<FlyWood*>(e->obj);
							if (e->nx != 0)
							{
								if (rdx != 0 && rdx != dx)
									x += nx * abs(rdx);
								y += min_ty * dy + ny * 0.1f - 0.4f;
							}
							else if (e->ny < 0)
							{
								x += min_tx * dx + nx * 0.4f;
								if (OnGround == false)
								{
									y += min_ty * dy + ny * 0.1f - 0.3f;
									OnGround = true; // xử lý chạm đất
									isFalling = isFlyingLow = isFlyingHigh = false;
								}
								if (flywood->GetState() == FLYWOOD_STATE_MOVE)
									flywood->SetState(FLYWOOD_STATE_FALL);
							}
						}
						else
							CollisionWithObject(e, min_tx, min_ty, nx, ny, rdx, rdy);
						break;
					case CATEGORY::ENEMY:
						CollisionWithEnemy(e, min_tx, min_ty, nx, ny);
						break;

					case CATEGORY::ITEM:
						CollisionWithItem(e, min_tx, min_ty, nx, ny);
						break;

					case CATEGORY::WEAPON:
						CollisionWithWeapon(e, min_tx, min_ty, nx, ny);
						break;
					}
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


		if (x >= MAP_MAX_WIDTH - Width - 1)
		{
			x = static_cast<float>(MAP_MAX_WIDTH - Width - 1);
			vx = 0;
		}

		// Đang nhảy max running mà thả nút a thì khi chạm đất mới mất isrunning
		// nếu k thì khi thả nút A trên trời sẽ bị mất isrunning
		if (pressA == false)
		{
			if (OnGround == true)
				isRunning = false;
		}

		if (y > _Map->GetHeight())
		{
			CGame::GetInstance()->SwitchScene2(1);
			_HUD->MarioLife--;
			_PlayScene->Stop = false;
		}
	
	}
	Debug();
}

void Mario::Render()
{
	if (isLevelUp == true)
	{
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
		{
			if (nx == RIGHT)
			{
				if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
				{
					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0) // 0->14 30 60
						ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0) //15->29 45 75
						ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
				}
				else
				{
					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
						ani = MARIO_ANI_BIG_IDLE_RIGHT;
					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
						ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
				}
			}
			else if (nx == LEFT)
			{
				if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
				{
					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
						ani = MARIO_ANI_SMALL_IDLE_LEFT;
					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
						ani = MARIO_ANI_UP_LEVEL_LEFT_SMALL_BIG;
				}
				else
				{
					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
						ani = MARIO_ANI_BIG_IDLE_LEFT;
					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
						ani = MARIO_ANI_UP_LEVEL_LEFT_SMALL_BIG;
				}
			}
			int alpha = 128;
			animation_set->at(ani)->Render(x, y, alpha);
		}break;
		}
	}
	else if (isLevelDown == true)
	{
		switch (level)
		{
		case MARIO_LEVEL_BIG:
		{
			if (nx == RIGHT)
			{
				if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
				{
					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
						ani = MARIO_ANI_BIG_IDLE_RIGHT;
					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
						ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
				}
				else
				{
					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0) // 0->14 30 60
						ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0) //15->29 45 75
						ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
				}
			}
			else if (nx == LEFT)
			{
				if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
				{
					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
						ani = MARIO_ANI_BIG_IDLE_LEFT;
					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
						ani = MARIO_ANI_UP_LEVEL_LEFT_SMALL_BIG;
				}
				else
				{
					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
						ani = MARIO_ANI_SMALL_IDLE_LEFT;
					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
						ani = MARIO_ANI_UP_LEVEL_LEFT_SMALL_BIG;
				}
			}

			if (untouchable == true)
			{
				if (UntouchtableTime % 3 == 0)
					animation_set->at(ani)->Render(x, y, 32);
				else
					animation_set->at(ani)->Render(x, y, 255);
			}
			else
				animation_set->at(ani)->Render(x, y, 128);

		}
		break;
		}
	}
	else
	{
		ani = -1;
		if (state == MARIO_STATE_DIE)
			ani = MARIO_ANI_DIE;
		else
		{
			// chưa nhảy // đang rớt xuống
			if (vy >= 0)
			{
				// đang đứng trên đất
				if (OnGround == true)
				{
					//if (isAttacking == true && endAttack == false && untouchable == false)
					if (isAttacking == true && endAttack == false)
					{
						switch (level)
						{
						case MARIO_LEVEL_TAIL:
						{
							if (nx == RIGHT)
							{
								if (time_attack <= TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_1;
								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_2;
								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_3;
								else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_4;
								else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_1;
							}
							else
							{
								if (time_attack <= TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_3;
								}
								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_2;
								}
								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_1;
								}
								else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_4;
								}
								else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_3;
								}
							}
						}
						break;

						case MARIO_LEVEL_FIRE:
						{
							// phải 
							if (nx == RIGHT)
							{
								if (time_attack <= TIME_ATTACK)
								{
									ani = MARIO_ANI_FIRE_ATTACK_RIGHT_1;
								}
								else //if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
								{
									ani = MARIO_ANI_FIRE_ATTACK_RIGHT_2;
								}
							}
							// trái
							else
							{
								if (time_attack <= TIME_ATTACK)
								{
									ani = MARIO_ANI_FIRE_ATTACK_LEFT_1;
								}
								else //if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
								{
									ani = MARIO_ANI_FIRE_ATTACK_LEFT_2;
								}
							}
						}
						break;
						}
					}
					else if (canKick == true)
					{
						// phải
						if (nx == RIGHT)
						{
							switch (level)
							{
							case MARIO_LEVEL_SMALL:
								ani = MARIO_ANI_SMALL_KICK_RIGHT; break;
							case MARIO_LEVEL_BIG:
								ani = MARIO_ANI_BIG_KICK_RIGHT; break;
							case MARIO_LEVEL_TAIL:
								ani = MARIO_ANI_TAIL_KICK_RIGHT; break;
							case MARIO_LEVEL_FIRE:
								ani = MARIO_ANI_FIRE_KICK_RIGHT; break;
							}
						}
						// trái
						else
						{
							switch (level)
							{
							case MARIO_LEVEL_SMALL:
								ani = MARIO_ANI_SMALL_KICK_LEFT; break;
							case MARIO_LEVEL_BIG:
								ani = MARIO_ANI_BIG_KICK_LEFT; break;
							case MARIO_LEVEL_TAIL:
								ani = MARIO_ANI_TAIL_KICK_LEFT; break;
							case MARIO_LEVEL_FIRE:
								ani = MARIO_ANI_FIRE_KICK_LEFT; break;
							}
						}
					}
					else if (isSitDown == true && GoHiddenWorld == false)
					{
						if (nx == RIGHT)
						{
							switch (level)
							{
								case MARIO_LEVEL_SMALL:
									ani = MARIO_ANI_SMALL_IDLE_RIGHT; break;
								case MARIO_LEVEL_BIG:
									ani = MARIO_ANI_BIG_SITDOWN_RIGHT; break;
								case MARIO_LEVEL_TAIL:
									ani = MARIO_ANI_TAIL_SITDOWN_RIGHT; break;
								case MARIO_LEVEL_FIRE:
									ani = MARIO_ANI_FIRE_SITDOWN_RIGHT; break;
							}
						}
						else
						{
							switch (level)
							{
								case MARIO_LEVEL_SMALL:
									ani = MARIO_ANI_SMALL_IDLE_LEFT; break;
								case MARIO_LEVEL_BIG:
									ani = MARIO_ANI_BIG_SITDOWN_LEFT; break;
								case MARIO_LEVEL_TAIL:
									ani = MARIO_ANI_TAIL_SITDOWN_LEFT; break;
								case MARIO_LEVEL_FIRE:
									ani = MARIO_ANI_FIRE_SITDOWN_LEFT; break;
							}
						}
					}
					else if (canFlyX == true || canFlyS == true)
					{
						// đứng yên
						if (abs(vx) == 0.0f)
						{
							// phải 
							if (nx == RIGHT)
							{
								ani = MARIO_ANI_TAIL_IDLE_RIGHT;
							}
							// trái
							else
							{
								ani = MARIO_ANI_TAIL_IDLE_LEFT;
							}
						}
						// đi bộ qua phải
						else if (nx == RIGHT)
						{
							if (ChangeDirection == false)
							{
								ani = MARIO_ANI_TAIL_WALKING_RIGHT;
							}
							else if (ChangeDirection == true)
								ani = MARIO_ANI_TAIL_STOP_RIGHT;
						}
						//  đi bộ qua trái
						else if (nx == LEFT)
						{
							if (ChangeDirection == false)
							{
								ani = MARIO_ANI_TAIL_WALKING_LEFT;
							}
							else if (ChangeDirection == true)
								ani = MARIO_ANI_TAIL_STOP_LEFT;
						}
					}
					else if (GoHiddenWorld == true)
					{
						switch (level)
						{
						case MARIO_LEVEL_SMALL:
							ani = MARIO_ANI_SMALL_GOHIDDENWORLD; break;
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_GOHIDDENWORLD; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_GOHIDDENWORLD; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_GOHIDDENWORLD; break;
						}
					}
					else
					{
						// đứng yên
						if (level_of_walking == 0 && vx == 0.0f)
						{
							// bình thường
							if (ChangeDirection == false)
							{
								if (isBlocked == true)
								{
									// phải 
									if (nx == RIGHT)
									{
										if (isHolding == true)
										{
											if (GetState() == MARIO_STATE_HOLDING_RIGHT)
											{
												switch (level)
												{
													case MARIO_LEVEL_SMALL:
														ani = MARIO_ANI_SMALL_HOLD_WALK_RIGHT; break;
													case MARIO_LEVEL_BIG:
														ani = MARIO_ANI_BIG_HOLD_WALK_RIGHT; break;
													case MARIO_LEVEL_TAIL:
														ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT; break;
													case MARIO_LEVEL_FIRE:
														ani = MARIO_ANI_FIRE_HOLD_WALK_RIGHT; break;
												}
											}
											else
											{
												switch (level)
												{
													case MARIO_LEVEL_SMALL:
														ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT; break;
													case MARIO_LEVEL_BIG:
														ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT; break;
													case MARIO_LEVEL_TAIL:
														ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT; break;
													case MARIO_LEVEL_FIRE:
														ani = MARIO_ANI_FIRE_HOLD_IDLE_RIGHT; break;
												}
											}
										}
										else
										{
											switch (level)
											{
											case MARIO_LEVEL_SMALL:
												ani = MARIO_ANI_SMALL_WALKING_RIGHT; break;
											case MARIO_LEVEL_BIG:
												ani = MARIO_ANI_BIG_WALKING_RIGHT; break;
											case MARIO_LEVEL_TAIL:
												ani = MARIO_ANI_TAIL_WALKING_RIGHT; break;
											case MARIO_LEVEL_FIRE:
												ani = MARIO_ANI_FIRE_WALKING_RIGHT; break;
											}
										}
									}
									// trái
									else
									{
										if (isHolding == true)
										{
											if (GetState() == MARIO_STATE_HOLDING_LEFT)
											{
												switch (level)
												{
												case MARIO_LEVEL_SMALL:
													ani = MARIO_ANI_SMALL_HOLD_WALK_LEFT; break;
												case MARIO_LEVEL_BIG:
													ani = MARIO_ANI_BIG_HOLD_WALK_LEFT; break;
												case MARIO_LEVEL_TAIL:
													ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT; break;
												case MARIO_LEVEL_FIRE:
													ani = MARIO_ANI_FIRE_HOLD_WALK_LEFT; break;
												}
											}
											else
											{
												switch (level)
												{
												case MARIO_LEVEL_SMALL:
													ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT; break;
												case MARIO_LEVEL_BIG:
													ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT; break;
												case MARIO_LEVEL_TAIL:
													ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT; break;
												case MARIO_LEVEL_FIRE:
													ani = MARIO_ANI_FIRE_HOLD_IDLE_LEFT; break;
												}
											}
										}
										else
										{
											switch (level)
											{
											case MARIO_LEVEL_SMALL:
												ani = MARIO_ANI_SMALL_WALKING_LEFT; break;
											case MARIO_LEVEL_BIG:
												ani = MARIO_ANI_BIG_WALKING_LEFT; break;
											case MARIO_LEVEL_TAIL:
												ani = MARIO_ANI_TAIL_WALKING_LEFT; break;
											case MARIO_LEVEL_FIRE:
												ani = MARIO_ANI_FIRE_WALKING_LEFT; break;
											}
										}
									}
								}
								else
								{
									// phải 
									if (nx == RIGHT)
									{
										if (isHolding == true)
										{
											switch (level)
											{
												case MARIO_LEVEL_SMALL:
													ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT; break;
												case MARIO_LEVEL_BIG:
													ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT; break;
												case MARIO_LEVEL_TAIL:
													ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT; break;
												case MARIO_LEVEL_FIRE:
													ani = MARIO_ANI_FIRE_HOLD_IDLE_RIGHT; break;
											}
										}
										else
										{
											switch (level)
											{
											case MARIO_LEVEL_SMALL:
												ani = MARIO_ANI_SMALL_IDLE_RIGHT; break;
											case MARIO_LEVEL_BIG:
												ani = MARIO_ANI_BIG_IDLE_RIGHT; break;
											case MARIO_LEVEL_TAIL:
												ani = MARIO_ANI_TAIL_IDLE_RIGHT; break;
											case MARIO_LEVEL_FIRE:
												ani = MARIO_ANI_FIRE_IDLE_RIGHT; break;
											}
										}
									}
									// trái
									else
									{
										if (isHolding == true)
										{
											switch (level)
											{
											case MARIO_LEVEL_SMALL:
												ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT; break;
											case MARIO_LEVEL_BIG:
												ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT; break;
											case MARIO_LEVEL_TAIL:
												ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT; break;
											case MARIO_LEVEL_FIRE:
												ani = MARIO_ANI_FIRE_HOLD_IDLE_LEFT; break;
											}
										}
										else
										{
											switch (level)
											{
											case MARIO_LEVEL_SMALL:
												ani = MARIO_ANI_SMALL_IDLE_LEFT; break;
											case MARIO_LEVEL_BIG:
												ani = MARIO_ANI_BIG_IDLE_LEFT; break;
											case MARIO_LEVEL_TAIL:
												ani = MARIO_ANI_TAIL_IDLE_LEFT; break;
											case MARIO_LEVEL_FIRE:
												ani = MARIO_ANI_FIRE_IDLE_LEFT; break;
											}
										}
									}
								}
								
							}
							// hiện hiệu ứng khựng
							else
							{
								// phải 
								if (nx == RIGHT)
								{
									switch (level)
									{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_STOP_RIGHT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_STOP_RIGHT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_STOP_RIGHT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_STOP_RIGHT; break;
									}
								}
								// trái
								else
								{
									switch (level)
									{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_STOP_LEFT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_STOP_LEFT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_STOP_LEFT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_STOP_LEFT; break;
									}
								}
							}
						}
						// đi bộ qua phải
						else if (nx == RIGHT)
						{
							// bình thường
							if (ChangeDirection == false)
							{
								// đang cầm rùa
								if (isHolding == true)
								{
									switch (level)
									{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_HOLD_WALK_RIGHT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_HOLD_WALK_RIGHT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_HOLD_WALK_RIGHT; break;
									}
								}
								// chạy nhanh tối đa
								else if (isMaxRunning == true)
								{
									switch (level)
									{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_RUNNING_RIGHT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_RUNNING_RIGHT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_RUNNING_RIGHT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_RUNNING_RIGHT; break;
									}
								}
								// đi bộ bình thường
								else
								{
									switch (level)
									{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_WALKING_RIGHT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_WALKING_RIGHT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_WALKING_RIGHT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_WALKING_RIGHT; break;
									}
								}
							}
							// hiện hiệu ứng khựng
							else
							{
								switch (level)
								{
								case MARIO_LEVEL_SMALL:
									ani = MARIO_ANI_SMALL_STOP_RIGHT; break;
								case MARIO_LEVEL_BIG:
									ani = MARIO_ANI_BIG_STOP_RIGHT; break;
								case MARIO_LEVEL_TAIL:
									ani = MARIO_ANI_TAIL_STOP_RIGHT; break;
								case MARIO_LEVEL_FIRE:
									ani = MARIO_ANI_FIRE_STOP_RIGHT; break;
								}
							}
						}
						// đi bộ qua trái
						else if (nx == LEFT)
						{
							// bình thường
							if (ChangeDirection == false)
							{
								// đang cầm rùa
								if (isHolding == true)
								{
									switch (level)
									{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_HOLD_WALK_LEFT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_HOLD_WALK_LEFT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_HOLD_WALK_LEFT; break;
									}
								}
								// chạy nhanh tối đa
								else if (isMaxRunning == true)
								{
									switch (level)
									{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_RUNNING_LEFT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_RUNNING_LEFT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_RUNNING_LEFT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_RUNNING_LEFT; break;
									}
								}
								// đi bộ bình thường
								else
								{
									switch (level)
									{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_WALKING_LEFT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_WALKING_LEFT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_WALKING_LEFT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_WALKING_LEFT; break;
									}
								}
							}
							// hiện hiệu ứng khựng
							else
							{
								switch (level)
								{
								case MARIO_LEVEL_SMALL:
									ani = MARIO_ANI_SMALL_STOP_LEFT; break;
								case MARIO_LEVEL_BIG:
									ani = MARIO_ANI_BIG_STOP_LEFT; break;
								case MARIO_LEVEL_TAIL:
									ani = MARIO_ANI_TAIL_STOP_LEFT; break;
								case MARIO_LEVEL_FIRE:
									ani = MARIO_ANI_FIRE_STOP_LEFT; break;
								}
							}
						}
					}
				}
				// đang trên không
				else //if (isFalling == true)
				{
					if (isAttacking == true && endAttack == false)
					{
						switch (level)
						{
						case MARIO_LEVEL_TAIL:
						{
							if (nx == RIGHT)
							{
								if (time_attack <= TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_1;
								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_2;
								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_3;
								else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_4;
								else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_1;
							}
							else
							{
								if (time_attack <= TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_3;
								}
								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_2;
								}
								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_1;
								}
								else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_4;
								}
								else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
								{
									ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_3;
								}
							}
						}
						break;
						case MARIO_LEVEL_FIRE:
						{
							// phải 
							if (nx == RIGHT)
							{
								if (time_attack <= TIME_ATTACK)
									ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_1;
								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
									ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_2;
								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
									ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_3;
							}
							// trái
							else
							{
								if (time_attack <= TIME_ATTACK)
									ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_1;
								else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
									ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_2;
								else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
									ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_3;
							}
						}
						break;
						}
					}
					// đang cầm rùa
					else if (isHolding == true)
					{
						if (nx == RIGHT)
						{
							switch (level)
							{
							case MARIO_LEVEL_SMALL:
								ani = MARIO_ANI_SMALL_HOLD_JUMP_RIGHT; break;
							case MARIO_LEVEL_BIG:
								ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT; break;
							case MARIO_LEVEL_TAIL:
								ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT; break;
							case MARIO_LEVEL_FIRE:
								ani = MARIO_ANI_FIRE_HOLD_JUMP_RIGHT; break;
							}
						}
						else
						{
							switch (level)
							{
							case MARIO_LEVEL_SMALL:
								ani = MARIO_ANI_SMALL_HOLD_JUMP_LEFT; break;
							case MARIO_LEVEL_BIG:
								ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT; break;
							case MARIO_LEVEL_TAIL:
								ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT; break;
							case MARIO_LEVEL_FIRE:
								ani = MARIO_ANI_FIRE_HOLD_JUMP_LEFT; break;
							}
						}
					}
					// đá rùa
					else if (canKick == true)
					{
						if (nx == RIGHT)
						{
							switch (level)
							{
							case MARIO_LEVEL_SMALL:
								ani = MARIO_ANI_SMALL_KICK_RIGHT; break;
							case MARIO_LEVEL_BIG:
								ani = MARIO_ANI_BIG_KICK_RIGHT; break;
							case MARIO_LEVEL_TAIL:
								ani = MARIO_ANI_TAIL_KICK_RIGHT; break;
							case MARIO_LEVEL_FIRE:
								ani = MARIO_ANI_FIRE_KICK_RIGHT; break;
							}
						}
						else
						{
							switch (level)
							{
							case MARIO_LEVEL_SMALL:
								ani = MARIO_ANI_SMALL_KICK_LEFT; break;
							case MARIO_LEVEL_BIG:
								ani = MARIO_ANI_BIG_KICK_LEFT; break;
							case MARIO_LEVEL_TAIL:
								ani = MARIO_ANI_TAIL_KICK_LEFT; break;
							case MARIO_LEVEL_FIRE:
								ani = MARIO_ANI_FIRE_KICK_LEFT; break;
							}
						}
					}
					// trạng thái bay vẫy đuôi
					else if (canFlyX == true || canFlyS == true)
					{
						if (nx == RIGHT)
						{
							ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
						}
					}
					// ngồi
					else if (isSitDown == true && level > MARIO_LEVEL_SMALL)
					{
					if (nx == RIGHT)
					{
						switch (level)
						{
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_SITDOWN_RIGHT; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_SITDOWN_RIGHT; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_SITDOWN_RIGHT; break;
						}
					}
					else
					{
						switch (level)
						{
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_SITDOWN_LEFT; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_SITDOWN_LEFT; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_SITDOWN_LEFT; break;
						}
					}
					}
					// bình thường
					else
					{
						if (nx == RIGHT)
						{
							if (isMaxRunning == true)
							{
								switch (level)
								{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_JUMP_MAX_SPEED_RIGHT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_JUMP_MAX_SPEED_RIGHT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_JUMP_MAX_SPEED_RIGHT; break;
								}
							}
							else
							{
								switch (level)
								{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_JUMP_RIGHT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_FALLING_RIGHT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_FALLING_RIGHT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_FALLING_RIGHT; break;
								}
							}
						}
						else if (nx == LEFT)
						{
							if (isMaxRunning == true)
							{
								switch (level)
								{
								case MARIO_LEVEL_SMALL:
									ani = MARIO_ANI_SMALL_JUMP_MAX_SPEED_LEFT; break;
								case MARIO_LEVEL_BIG:
									ani = MARIO_ANI_BIG_JUMP_MAX_SPEED_LEFT; break;
								case MARIO_LEVEL_FIRE:
									ani = MARIO_ANI_FIRE_JUMP_MAX_SPEED_LEFT; break;
								}
							}
							else
							{
								switch (level)
								{
									case MARIO_LEVEL_SMALL:
										ani = MARIO_ANI_SMALL_JUMP_LEFT; break;
									case MARIO_LEVEL_BIG:
										ani = MARIO_ANI_BIG_FALLING_LEFT; break;
									case MARIO_LEVEL_TAIL:
										ani = MARIO_ANI_TAIL_FALLING_LEFT; break;
									case MARIO_LEVEL_FIRE:
										ani = MARIO_ANI_FIRE_FALLING_LEFT; break;
								}
							}
						}
					}
				}
			}
			// nhảy
			else
			{
				if (isAttacking == true && endAttack == false)
				{
					switch (level)
					{
					case MARIO_LEVEL_TAIL:
					{
						if (nx == RIGHT)
						{
							if (time_attack <= TIME_ATTACK)
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_1;
							else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_2;
							else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_3;
							else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_4;
							else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_1;
						}
						else
						{
							if (time_attack <= TIME_ATTACK)
							{
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_3;
							}
							else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
							{
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_2;
							}
							else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
							{
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_1;
							}
							else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
							{
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_4;
							}
							else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
							{
								ani = MARIO_ANI_TAIL_ATTACK_NOT_ONGROUND_3;
							}
						}
					}
					break;

					case MARIO_LEVEL_FIRE:
					{
						// phải 
						if (nx == RIGHT)
						{
							if (time_attack <= TIME_ATTACK)
								ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_1;
							else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
								ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_2;
							else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
								ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_RIGHT_3;
						}
						// trái
						else
						{
							if (time_attack <= TIME_ATTACK)
								ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_1;
							else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
								ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_2;
							else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
								ani = MARIO_ANI_FIRE_ATTACK_NOT_ONGROUND_LEFT_3;
						}
					}
					break;
					}
				}
				// cầm rùa + nhảy
				else if (isHolding == true)
				{
					// phải 
					if (nx == RIGHT)
					{
						switch (level)
						{
						case MARIO_LEVEL_SMALL:
							ani = MARIO_ANI_SMALL_HOLD_JUMP_RIGHT; break;
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_HOLD_JUMP_RIGHT; break;
						}
					}
					// trái
					else
					{
						switch (level)
						{
						case MARIO_LEVEL_SMALL:
							ani = MARIO_ANI_SMALL_HOLD_JUMP_LEFT; break;
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_HOLD_JUMP_LEFT; break;
						}
					}
				}
				// đá rùa
				else if (canKick == true)
				{
					// phải 
					if (nx == RIGHT)
					{
						switch (level)
						{
						case MARIO_LEVEL_SMALL:
							ani = MARIO_ANI_SMALL_KICK_RIGHT; break;
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_KICK_RIGHT; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_KICK_RIGHT; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_KICK_RIGHT; break;
						}
					}
					// trái
					else
					{
						switch (level)
						{
						case MARIO_LEVEL_SMALL:
							ani = MARIO_ANI_SMALL_KICK_LEFT; break;
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_KICK_LEFT; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_KICK_LEFT; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_KICK_LEFT; break;
						}
					}
				}
				// trạng thái bay vẫy đuôi
				else if (canFlyX == true || canFlyS == true)
				{
					// phải 
					if (nx == RIGHT)
					{
						if (isMaxRunning == true)
						{
							if (isFlyingHigh == true)
								ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT;
							else
								ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
						}
					}
					// trái
					else
					{
						if (isMaxRunning == true)
						{
							if (isFlyingHigh == true)
								ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT;
							else
								ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
						}
						else
							ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT;
					}
				}
				// ngồi
				else if (isSitDown == true && level > MARIO_LEVEL_SMALL)
					{
					if (nx == RIGHT)
					{
						switch (level)
						{
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_SITDOWN_RIGHT; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_SITDOWN_RIGHT; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_SITDOWN_RIGHT; break;
						}
					}
					else
					{
						switch (level)
						{
						case MARIO_LEVEL_BIG:
							ani = MARIO_ANI_BIG_SITDOWN_LEFT; break;
						case MARIO_LEVEL_TAIL:
							ani = MARIO_ANI_TAIL_SITDOWN_LEFT; break;
						case MARIO_LEVEL_FIRE:
							ani = MARIO_ANI_FIRE_SITDOWN_LEFT; break;
						}
					}
					}
				else
				{
					// phải 
					if (nx == RIGHT)
					{
						if (isMaxRunning == true)
						{
							switch (level)
							{
							case MARIO_LEVEL_SMALL:
								ani = MARIO_ANI_SMALL_JUMP_MAX_SPEED_RIGHT; break;
							case MARIO_LEVEL_BIG:
								ani = MARIO_ANI_BIG_JUMP_MAX_SPEED_RIGHT; break;
							case MARIO_LEVEL_FIRE:
								ani = MARIO_ANI_FIRE_JUMP_MAX_SPEED_RIGHT; break;
							}
						}
						else
						{
							switch (level)
							{
								case MARIO_LEVEL_SMALL:
									ani = MARIO_ANI_SMALL_JUMP_RIGHT; break;
								case MARIO_LEVEL_BIG:
									ani = MARIO_ANI_BIG_JUMP_RIGHT; break;
								case MARIO_LEVEL_TAIL:
									ani = MARIO_ANI_TAIL_JUMP_RIGHT; break;
								case MARIO_LEVEL_FIRE:
									ani = MARIO_ANI_FIRE_JUMP_RIGHT; break;
							}
						}
					}
					// trái
					else
					{
						if (isMaxRunning == true)
						{
							switch (level)
							{
								case MARIO_LEVEL_SMALL:
									ani = MARIO_ANI_SMALL_JUMP_MAX_SPEED_LEFT; break;
								case MARIO_LEVEL_BIG:
									ani = MARIO_ANI_BIG_JUMP_MAX_SPEED_LEFT; break;
								case MARIO_LEVEL_FIRE:
									ani = MARIO_ANI_FIRE_JUMP_MAX_SPEED_LEFT; break;
							}
						}
						else
						{
							switch (level)
							{
								case MARIO_LEVEL_SMALL:
									ani = MARIO_ANI_SMALL_JUMP_LEFT; break;
								case MARIO_LEVEL_BIG:
									ani = MARIO_ANI_BIG_JUMP_LEFT; break;
								case MARIO_LEVEL_TAIL:
									ani = MARIO_ANI_TAIL_JUMP_LEFT; break;
								case MARIO_LEVEL_FIRE:
									ani = MARIO_ANI_FIRE_JUMP_LEFT; break;
							}
						}
					}
				}
			}
		}

		if (untouchable == true)
		{
			if (UntouchtableTime % 3 == 0)
				animation_set->at(ani)->Render(x, y, 32);
			else
				animation_set->at(ani)->Render(x, y, 255);
		}
		else
		{
			// TH max running thì ani lùi về 1 tí vì cái tay dơ lên sẽ làm đẩy ani lên
			if(ani == MARIO_ANI_BIG_RUNNING_RIGHT || ani == MARIO_ANI_FIRE_RUNNING_RIGHT)
				animation_set->at(ani)->Render(x - 3, y, 255);
			else if(ani == MARIO_ANI_TAIL_RUNNING_RIGHT)
				animation_set->at(ani)->Render(x - 5, y, 255);
			else
				animation_set->at(ani)->Render(x, y, 255);
		}
	}
}

void Mario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case MARIO_STATE_WALKING_RIGHT:
		{
			if (nx == LEFT)
			{
				x -= 2.0f; // xử lý vụ đi sát gạch thì xuyên qua
			}
			nx = RIGHT;
		}
		break;

		case MARIO_STATE_WALKING_LEFT:
		{
			if (nx == RIGHT)
			{
				x += 2.0f; // xử lý vụ đi sát gạch thì xuyên qua
			}
			nx = LEFT;
		}
		break;

		case MARIO_STATE_JUMP:
		{
			if (OnGround == true)
			{
				if(NumberSmallGoomba == 0)
					vy = -MARIO_JUMP_SPEED_FAST;
				else
				{
					vy = -0.15f;
					jump_count_S++;
				}
					
				OnGround = false;
			}
		}
		break;

		case MARIO_STATE_JUMP_LOW:
		{
			if (OnGround == true)
			{
				if (NumberSmallGoomba == 0)
					vy = -MARIO_JUMP_SPEED_SLOW;
				else
				{
					vy = -0.1f;
					jump_count_X++;
				}
				OnGround = false;
			}
		}
		break;

		case MARIO_STATE_IDLE:
		{
			if (isFlyingHigh != true)
			{
				//isMaxRunning = false;
			}

			isHolding = isBlocked = false;
			
		}
		break;

		case MARIO_STATE_DIE:
		{
			vx = vy = 0.0f;
			StartToDie = GetTickCount64();
			_PlayScene->Stop = true;
		}
		break;

		case MARIO_STATE_FLY_LOW:
		{
			isFlyingLow = true;
			isFlyingHigh = false;
		}
		break;

		case MARIO_STATE_FLY_HIGH:
		{
			isFlyingLow = false;
			isFlyingHigh = true;
			vy = -MARIO_FLY_SPEED;
			//vx = nx * MARIO_FLY_MOVING_SPEED;
			isRunning = false;
		}
		break;

		case MARIO_STATE_SITDOWN:
		{
			if (isSitDown == false )
			{
				isSitDown = true;
				// sprite ngồi heigh chỉ có 18 còn sprite idle thì heigh = 28 nên phải trừ bớt để k bị lỗi sprite
				// sau khi +10 thì phải -10 lại bên idle để không bị lỗi mario thụt xuống dưới
				if (level > MARIO_LEVEL_SMALL)
				{
					if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
						y = y + (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_BBOX_SITDOWN_HEIGHT);
					else
						y = y + (MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_BBOX_SITDOWN_HEIGHT);
				}
			}
		}
		break;

		case MARIO_STATE_ATTACK:
		{
			isAttacking = true;
		}
		break;

		case MARIO_STATE_HOLDING_IDLE:
		{
			isHolding = true;
			

			if (pressA != true)
			{
				isRunning = false;
			}
		}
		break;

	case MARIO_STATE_HOLDING_RIGHT:
	{
		if (nx == LEFT)
		{
			x -= 2.0f; // xử lý vụ đi sát gạch thì xuyên qua
		}
		nx = RIGHT;
		isHolding = true;
	}
	break;

	case MARIO_STATE_HOLDING_LEFT:
	{
		if (nx == RIGHT)
		{
			x += 2.0f; // xử lý vụ đi sát gạch thì xuyên qua
		}
		nx = LEFT;
		isHolding = true;
	}
	break;

	case MARIO_STATE_FLYING_HIGH_RIGHT:
	{
		isFlyingLow = false;
		isFlyingHigh = true;
		vy = -MARIO_FLY_SPEED;
		vx = MARIO_FLY_MOVING_SPEED;
		isRunning = false;
	}
	break;

	case MARIO_STATE_FLYING_HIGH_LEFT:
	{
		isFlyingLow = false;
		isFlyingHigh = true;
		vy = -MARIO_FLY_SPEED;
		vx = -MARIO_FLY_MOVING_SPEED;
		isRunning = false;
	}
	break;

	case MARIO_STATE_ENDSCENE:
	{
		loseControl = true;

	}
	break;

	}
}

void Mario::Reset()
{
	SetSpeed(0, 0);
	canDelete = isDie = false;
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	render_tail = GoHiddenWorld = untouchable = ChangeDirection = isRunning = isMaxRunning = isFlyingHigh = canFlyX = canFlyS = isFalling = isSitDown = isAttacking = endAttack = false;
	OnGround = true;
	level_of_running = level_of_walking = 0;
	time_attack = time_fly = 0;
	ani = 0;
	NumberBullet = 2;
}

void Mario::DownLevel()
{
	if (level == MARIO_LEVEL_SMALL)
		SetState(MARIO_STATE_DIE);
	else
	{
		switch (level)
		{
		case MARIO_LEVEL_BIG:
		{
			if (isSitDown == true)
				y -= static_cast<FLOAT>(MARIO_BIG_BBOX_SITDOWN_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) + 5;
			_PlayScene->Stop = true;
		}break;

		case MARIO_LEVEL_TAIL:
		{
			if (isSitDown == true)
				y -=  2;

			ChangeLevelTime = GetTickCount64();
			auto effect = new EffectSmoke(this->x, this->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
			_Grid->AddStaticObject(effect, this->x, this->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
			_PlayScene->Stop = true;
		}break;

		case MARIO_LEVEL_FIRE:
		{
			// y -= static_cast<FLOAT>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
			ChangeLevelTime = GetTickCount64();
			auto effect = new EffectSmoke(this->x, this->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
			_Grid->AddStaticObject(effect, this->x, this->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
			_PlayScene->Stop = true;

		}break;

		}
		isLevelDown = true;
		isAttacking = false;
		endAttack = true;
	}
}

void Mario::UpLevel()
{
	if (level == MARIO_LEVEL_FIRE || level == MARIO_LEVEL_TAIL)
		return;
	

	RightOld = float(x + Width);
	loseControl = true;
	switch (level) // không xử lý level tail ở đây mà xử lý bên mario tail vì bị lỗi hiện smoke trước mà đuôi vẫn chưa xóa
	{
	case MARIO_LEVEL_BIG:
	{ 
		ChangeLevelTime = GetTickCount64();
		auto effect = new EffectSmoke(this->x, this->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
		_Grid->AddStaticObject(effect, this->x, this->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
		_PlayScene->Stop = true;
	}break;

	case MARIO_LEVEL_SMALL:
	{
		_PlayScene->Stop = true;
	}
	break;
	}
	isLevelUp = true;
}

void Mario::Debug()
{
	/*switch (state)
	{
	case MARIO_STATE_IDLE:
		DebugOut(L"State = IDLE\t"); break;
	case MARIO_STATE_WALKING_RIGHT:
		DebugOut(L"State = WALKING_RIGHT\t"); break;
	case MARIO_STATE_WALKING_LEFT:
		DebugOut(L"State = WALKING_LEFT\t"); break;
	case MARIO_STATE_JUMP:
		DebugOut(L"State = JUMP\t"); break;
	case MARIO_STATE_DIE:
		DebugOut(L"State = DIE\t"); break;
	case MARIO_STATE_JUMP_LOW:
		DebugOut(L"State = JUMP_LOW\t"); break;
	case MARIO_STATE_FLY_LOW:
		DebugOut(L"State = FLY_LOW\t"); break;
	case MARIO_STATE_FLY_HIGH:
		DebugOut(L"State = FLY_HIGH\t"); break;
	case MARIO_STATE_SITDOWN:
		DebugOut(L"State = SITDOWN\t"); break;
	case MARIO_STATE_ATTACK:
		DebugOut(L"State = ATTACK\t"); break;
	case MARIO_STATE_RUNNING:
		DebugOut(L"State = RUNNING\t"); break;
	case MARIO_STATE_HOLDING_IDLE:
		DebugOut(L"State = HOLDING_IDLE\t"); break;
	case MARIO_STATE_HOLDING_RIGHT:
		DebugOut(L"State = HOLDING RIGHT\t"); break;
	case MARIO_STATE_HOLDING_LEFT:
		DebugOut(L"State = HOLDING LEFT\t"); break;
	case MARIO_STATE_FLYING_HIGH_RIGHT:
		DebugOut(L"State = FLYING_HIGH_RIGHT\t"); break;
	case MARIO_STATE_FLYING_HIGH_LEFT:
		DebugOut(L"State = FLYING_HIGH_LEFT\t"); break;
	case MARIO_STATE_ENDSCENE:
		DebugOut(L"State = MARIO_STATE_ENDSCENE\t"); break;
	}*/

	//DebugOut(L"\nMario vy = %f, level_of_walking = %i\n", vy, level_of_walking);
	DebugOut(L" NumberSmallGoomba = %i \n", NumberSmallGoomba);
	//DebugOut(L"\n");
}

void Mario::Unload()
{
	__instance = NULL;
}

void Mario::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
	if (enemy != NULL && enemy->isDie == false && this->state != MARIO_STATE_DIE)
	{
		if (untouchable == true)
		{
			if (e->nx != 0)
			{
				x += dx;
				y += min_ty * dy + ny * 0.4f;
			}
			else if (e->ny > 0) // dưới lên trên
			{
				x += min_tx * dx + nx * 0.4f;
				//isFalling = isFlyingLow = isFlyingHigh = false;
				y += dy;
			}
			else if (e->ny < 0) // dưới lên trên
			{
				x += min_tx * dx + nx * 0.4f;
				y += dy;
				//isFalling = isFlyingLow = isFlyingHigh = false;
			}
		}
		else
		{
			if (e->nx != 0)
			{
				y += min_ty * dy + ny * 0.4f;
			}
			else if (e->ny < 0)
			{
				x += min_tx * dx + nx * 0.4f;
				
			}
		}

		// va chạm từ trên xuống
		if (e->ny < 0)
		{
			switch (enemy->EnemyType)
			{
			case ENEMY_TYPE_GOOMBA:
			{
				this->vy = -MARIO_JUMP_DEFLECT_SPEED;
				this->nScore++;
				_HUD->UpdateScore(enemy, nScore);
				enemy->Health--;
				if (enemy->Health == 0)
				{
					enemy->SetState(ENEMY_STATE_DIE_IS_JUMPED);
				}
			}
			break;

			case ENEMY_TYPE_KOOPAS:
			{
				this->vy = -MARIO_JUMP_DEFLECT_SPEED;
				Koopas* koopas = dynamic_cast<Koopas*>(enemy);
				if (koopas->Health > 1)
				{
					koopas->Health--;
					this->nScore++;
					_HUD->UpdateScore(e->obj, nScore);
				}
				else
				{
					if (koopas->vx != 0 && koopas->isShell_2 == false) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
					{
						koopas->SetState(KOOPAS_STATE_SHELL);
						koopas->ReviveTime = GetTickCount64();
						this->nScore++;
						_HUD->UpdateScore(koopas, nScore);
					}
					else if (koopas->vx != 0 && koopas->isShell_2 == true) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
					{
						koopas->SetState(KOOPAS_STATE_SHELL_2);
						koopas->vy = 0;
						koopas->ReviveTime = GetTickCount64();
						this->nScore++;
						_HUD->UpdateScore(koopas, nScore);
					}
					else if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2 || koopas->GetState() == KOOPAS_STATE_SHELL_HOLD)
					{
						if (this->x <= koopas->x)
						{
							if (koopas->isShell == true)
								koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
							else
								koopas->SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
						}
							
						else
						{
							if (koopas->isShell == true)
								koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
							else
								koopas->SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
						}
						koopas->isKicked = true;
						this->nScore++;
						_HUD->UpdateScore(koopas, nScore);
					}
				}
			}
			break;

			case ENEMY_TYPE_PLANT:
			{
				if (dynamic_cast<FirePiranhaPlant*>(enemy))
				{
					FirePiranhaPlant* plant = dynamic_cast<FirePiranhaPlant*>(enemy);
					if (e->ny < 0)
					{
						if (INT(plant->Startposy - plant->y) <= (plant->WarpPipeHeight - FIREPIRANHAPLANT_BBOX_HEIGHT))
						{
							// cho cây thụt xuống ngang với miệng cống
							plant->y += 1;
							plant->Stop = true;
						}
						else
						{
							if (untouchable == false)
								DownLevel();
						}
					}
				}
				else if (dynamic_cast<GreenFirePlant*>(enemy))
				{
					GreenFirePlant* plant = dynamic_cast<GreenFirePlant*>(enemy);

					if (INT(plant->Startposy - plant->y) <= (plant->WarpPipeHeight - GREENFIREPLANT_BBOX_HEIGHT))
					{
						// cho cây thụt xuống ngang với miệng cống
						plant->y += 1;
						plant->Stop = true;
					}
					else
					{
						if (untouchable == false)
							DownLevel();
					}
				}
				else if (dynamic_cast<GreenPlant*>(enemy))
				{
					GreenPlant* plant = dynamic_cast<GreenPlant*>(enemy);
					if (INT(plant->StartY - plant->y) <= (plant->WarpPipeHeight - GREENPLANT_BBOX_HEIGHT))
					{
						// cho cây thụt xuống ngang với miệng cống
						plant->y += 1;
						plant->isBlocked = true;
					}
					else
					{
						if (untouchable == false)
							DownLevel();
					}
				}
			}
			break;

			case ENEMY_TYPE_BOOMERANG_ENEMY:
			{
				this->vy = -MARIO_JUMP_DEFLECT_SPEED;
				enemy->Health--;
				if (enemy->Health == 0)
				{
					enemy->SetState(ENEMY_STATE_DIE_IS_JUMPED);
				}
				this->nScore++;
				BoomerangEnemy* boom = dynamic_cast<BoomerangEnemy*>(enemy);
				_HUD->UpdateScore(boom, nScore);
			}
			break;

			default:
			{
				enemy->canDelete = enemy->isDie = true;
				_HUD->UpdateScore(enemy, nScore);
			}
			break;
			}
		}
		else if (e->nx != 0)
		{
			switch (enemy->EnemyType)
			{
				case ENEMY_TYPE_GOOMBA: case ENEMY_TYPE_PLANT:
				{
					if (e->obj->ObjType == OBJECT_TYPE_SMALLGOOMBA)
					{
						SmallGoomba* goomba = dynamic_cast<SmallGoomba*>(enemy);
						if (goomba->state != SMALLGOOMBA_STATE_FOLLOWMARIO)
						{
							NumberSmallGoomba++;
							goomba->SetState(SMALLGOOMBA_STATE_FOLLOWMARIO);
						}
							
					}
					else
					{
						if (untouchable == false)
							DownLevel();
					}
				}
				break;

				case ENEMY_TYPE_KOOPAS:
				{
					if (enemy->ObjType == OBJECT_TYPE_REDFLYKOOPAS)
					{
						RedFlyKoopas* koopas = dynamic_cast<RedFlyKoopas*>(enemy);
						if (koopas->isHold == false)
						{
							if (koopas->Health == 2)
							{
								if (untouchable == false)
									DownLevel();
							}
							else
							{
								if (koopas->vx != 0 )
								{
									if (untouchable == false)
										DownLevel();
								}
							}
						}
						else if ((koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2) && koopas->vy >= 0) // tránh trường hợp mai rùa đang trên trời vẫn đá được
						{
							if (e->nx == LEFT)
							{
								if (pressA == true)
								{
									isHolding = true;
									koopas->nx = 1;
									koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
								}
								else if (koopas->vy >= 0)
								{
									this->canKick = koopas->isKicked = true;
									koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
								}
							}
							else if (e->nx == RIGHT)
							{
								if (pressA == true)
								{
									isHolding = true;
									koopas->nx = -1;
									koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
								}
								else if (koopas->vy >= 0)
								{
									this->canKick = koopas->isKicked = true;
									koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
								}
							}
						}

					}
					else
					{
						Koopas* koopas = dynamic_cast<Koopas*>(enemy);
						// k ở dạng mai rùa thì giảm cấp
						if (koopas->isShell == false && koopas->isShell_2 == false)
						{
							if (untouchable == false)
								DownLevel();
						}
						// ở dạng mai rùa
						else
						{
							if (koopas->vx != 0 && koopas->isKicked == true)
							{
								if (untouchable == false)
									DownLevel();
							}
							else
							{
								if (e->nx == LEFT)
								{
									if (pressA == true)
									{
										isHolding = true;
										koopas->nx = RIGHT;
										if (koopas->isShell == true)
											koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
										else
											koopas->SetState(KOOPAS_STATE_SHELL_2_HOLD);
									}
									else if (koopas->vy >= 0)
									{
										this->canKick = koopas->isKicked = true;
										if (koopas->isShell == true)
											koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
										else
											koopas->SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
									}
								}
								else if (e->nx == RIGHT)
								{
									if (pressA == true)
									{
										isHolding = true;
										koopas->nx = LEFT;
										if (koopas->isShell == true)
											koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
										else
											koopas->SetState(KOOPAS_STATE_SHELL_2_HOLD);
									}
									else if (koopas->vy >= 0)
									{
										this->canKick = koopas->isKicked = true;
										if (koopas->isShell == true)
											koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
										else
											koopas->SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
									}
								}
							}
						}
					}
				}
				break;

				default:
				{
					if (untouchable == false)
						DownLevel();
				}
				break;
			}
		}
		// quái rớt lên đầu mario
		else if (e->ny > 0)
		{
			switch (enemy->EnemyType)
			{
				case ENEMY_TYPE_KOOPAS:
				{
					Koopas* koopas = dynamic_cast<Koopas*>(enemy);
					if (koopas->isShell == false && koopas->isShell_2 == false)
					{
						if (untouchable == false)
							DownLevel();
					}
					else
					{
						if (OnGround == false)
						{
							x += dx;
							y += dy;
						}
						else
						{
							if (ny != 0) vy = 0;
						}

						if (pressA == true)
						{
							isHolding = true;
							if(koopas->isShell == true)
								koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
							else
								koopas->SetState(KOOPAS_STATE_SHELL_2_HOLD);
						}
						else
						{
							this->canKick = koopas->isKicked = true;
							if (this->nx == RIGHT)
							{
								if (koopas->isShell == true)
									koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
								else
									koopas->SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
							}
							else if (this->nx == LEFT)
							{
								if (koopas->isShell == true)
									koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
								else
									koopas->SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
							}
							koopas->vy -= 0.05f;
						}
					}
				}
				break;

				case ENEMY_TYPE_GOOMBA:
				{
					if (e->obj->ObjType == OBJECT_TYPE_SMALLGOOMBA)
					{
						SmallGoomba* goomba = dynamic_cast<SmallGoomba*>(enemy);
						
						if (goomba->state != SMALLGOOMBA_STATE_FOLLOWMARIO)
						{
							NumberSmallGoomba++;

							goomba->SetState(SMALLGOOMBA_STATE_FOLLOWMARIO);
						}
					}
					else
					{
						if (untouchable == false)
							DownLevel();
					}
				}
				break;

				default:
				{
					if (untouchable == false)
						DownLevel();
				}
				break;
			}

		}
	}
}

void Mario::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny, float rdx, float rdy)
{
	this->nScore = 0;

	if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
	{
		if (e->nx != 0)
		{
			if (ny != 0) vy = 0;
			x += dx;
		}
		else if (e->ny < 0)
		{
			if (ny != 0) vy = 0;
			if (OnGround == false)
				y += min_ty * dy + ny * 0.1f - 0.3f;
			OnGround = true; // xử lý chạm đất
			isFalling = isFlyingLow = isFlyingHigh = false;
			x += min_tx * dx + nx * 0.4f;
		}
		else if (e->ny > 0)
		{
			y += dy;
		}
	}
	else
	{
		if (e->nx != 0)
		{
			if (nx != 0)
			{
				if (isRunning == true || StopRunning == false)
				{
					if (level_of_running > 0 && canFlyS == false && canFlyX == false)
						level_of_running--;
					level_of_walking = 0;
					vx = 0;
				}
				else
				{
					level_of_walking = 0;
				}
			}
			// bị chặn -> thanh năng lượng giảm dần nhưng vẫn đang trong trạng thái isRunning
			isBlocked = true;
		}
		else if (e->ny > 0)
		{
			isFalling = true;
			vy = 0;
		}

		if (e->obj->ObjType == OBJECT_TYPE_GROUND)
		{
			Ground* ground = dynamic_cast<Ground*>(e->obj);
			this->nScore = 0;
			if (e->ny < 0)
			{
				x += min_tx * dx + nx * 0.4f;
				//  có thêm đk GetState() != MARIO_STATE_DIE để tránh lỗi mario chết nhưng k nhảy lên mà rớt thẳng xuống vì gán vy = 0 thay vì -0.2
				if (ny != 0 && GetState() != MARIO_STATE_DIE) vy = 0;
				if (OnGround == false)
					y += min_ty * dy + ny * 0.2f;
				OnGround = true; // xử lý chạm đất
				isFalling = isFlyingLow = isFlyingHigh = false;
			}
			else if (e->nx != 0)
			{
				y += min_ty * dy + ny * 0.2f;
				if (ny != 0) vy = 0;
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_QUESTIONBRICK || e->obj->ObjType == OBJECT_TYPE_ITEMBRICK)
		{
			if (ny != 0) vy = 0;
			if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
				
				// mario nhảy từ dưới lên va chạm gạch 
				if (e->ny > 0)
				{
					isFalling = true;
					y += min_ty * dy + ny * 0.1f - 0.4f;
					// nếu state normal thì xử lý va chạm, nếu không thì k xử lý
					if (brick->hasItem == true)
					{
						brick->SetState(BRICK_STATE_COLLISION);
						brick->hasItem = false;
						if (brick->Item > MONEY)
						{
							switch (_Mario->level)
							{
							case MARIO_LEVEL_SMALL:
							{
								brick->Item = MUSHROOM;
							}break;

							default:
								brick->Item = LEAF;
								break;
							}
							QuestionBrickItem* questionbrickitem = new QuestionBrickItem(brick->Item, brick->x, brick->y - 3);
							_Grid->AddStaticObject(questionbrickitem, brick->x, brick->y - 3);
						}
						else
						{
							QuestionBrickItem* questionbrickitem = new QuestionBrickItem(brick->Item, brick->x + 1, brick->y - 3);
							_Grid->AddStaticObject(questionbrickitem, brick->x, brick->y - 3);
						}
					}
				}
				else if (e->ny < 0) // mario đi trên gạch "?"
				{
					if (OnGround == false)
						y += min_ty * dy + ny * 0.1f - 0.3f;
					OnGround = true; // xử lý chạm đất
					isFalling = isFlyingLow = isFlyingHigh = false;
					this->MaxY = brick->y;
					x += min_tx * dx + nx * 0.4f;
				}
				else if (e->nx != 0)
				{
					if (this->OnGround == true)
						y += min_ty * dy + ny * 0.1f - 0.4f;
					else
						y += min_ty * dy + ny * 0.4f;
				}
			}
			else if (dynamic_cast<ItemBrick*>(e->obj))
			{
				ItemBrick* brick = dynamic_cast<ItemBrick*>(e->obj);
				// mario nhảy từ dưới lên va chạm gạch 
				if (e->ny > 0)
				{
					vy = 0;
					isFalling = true;
					// nếu state normal thì xử lý va chạm, nếu không thì k xử lý
					// cả 2 đều làm cho mario k nhảy đươc tiếp + rớt xuống
					if (brick->hasItem == true)
					{
						brick->SetState(BRICK_STATE_COLLISION);
						switch (brick->Item)
						{
						case BUTTONP:
						{
							BrickItem* brickitem = new BrickItem(BUTTONP, brick->x, brick->y - 16);
							_Grid->AddStaticObject(brickitem, brick->x, brick->y - 16);
							auto effect = new EffectSmoke(brick->x, brick->y - 16);
							_Grid->AddStaticObject(brickitem, brick->x, brick->y - 16);
						}
						break;

						case MUSHROOM:
						{
							BrickItem* brickitem = new BrickItem(MUSHROOM, brick->x, brick->y - 3);
							_Grid->AddMovingObject(brickitem, brick->x, brick->y - 3);
						}
						break;

						case NORMAL:
						{
							brick->SetState(ITEMBRICK_STATE_DIE);
							vy = 0;
						}
						break;

						case MONEYX10:
						{
							BrickItem* brickitem = new BrickItem(MONEYX10, brick->x, brick->y - 3);
							_Grid->AddMovingObject(brickitem, brick->x, brick->y - 3);
							brick->CountMoney--;
						}

						}
					}
					
				}
				else if (e->ny < 0) // mario đi trên gạch "?"
				{
					OnGround = true; // xử lý chạm đất
					isFalling = isFlyingLow = isFlyingHigh = false;

					x += min_tx * dx + nx * 0.4f;
				}
				else if (e->nx != 0)
				{
					
					if (ny != 0)vy = 0;
					y += min_ty * dy + ny * 0.4f;
					
				}
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_WARPPIPE)
		{
			if (dynamic_cast<WarpPipe*>(e->obj))
			{
				if (ny != 0) vy = 0;
				WarpPipe* pipe = dynamic_cast<WarpPipe*>(e->obj);
				if (e->ny != 0)
				{
					if (OnGround == false)
						y += min_ty * dy + ny * 0.1f - 0.3f;
					OnGround = true; // xử lý chạm đất
					isFalling = isFlyingLow = isFlyingHigh = false;
					x += min_tx * dx + nx * 0.4f;
					if (pipe->HiddenWorld == true)
					{
						if (e->ny < 0)
						{
							/*if (isSitDown == true &&
								this->x + Width <= pipe->x + pipe->Width / 2 &&
								this->x + Width >= pipe->x + pipe->Width / 2 - 14)*/
							if (isSitDown == true)
							{
								GoHiddenWorld = true;
								Scene = pipe->SceneID;
								vy = 0.03f;
								StartGoHiddenWorld = GetTickCount64();
								y += dy;
							}
						}
						else if (e->ny > 0)
						{
							GoHiddenWorld = true;
							vy = -0.03f;
							Scene = pipe->SceneID;
							y += dy;
						}
					}
				}
				else if (e->nx != 0)
				{
					float a = min_ty * dy + ny * 0.2f - 0.4f;
					if (OnGround == false)
					{
						y += min_ty * dy + ny * 0.2f - 0.4f;
					}
					else
					{
						y += min_ty * dy + ny * 0.2f;
					}

				}
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_BRICK)
		{
			if (ny != 0) vy = 0;
			Brick* brick = dynamic_cast<Brick*>(e->obj);
			if (brick->Type == BRICK_CLOUD) // mây
			{
				if (isFlyingHigh == true) // đang bay thì xuyên qua
				{
					x += dx;
					y += dy;
				}
			}
			if (e->nx != 0)
			{
				if (nx != 0) vx = 0;
				if (ny != 0)vy = 0;
				y += min_ty * dy + ny * 0.1f - 0.4f;
			}
			else if (e->ny < 0)
			{
				OnGround = true; // xử lý chạm đất
				isFalling = isFlyingLow = isFlyingHigh = false;
				x += min_tx * dx + nx * 0.4f;
			}
			else if (e->ny > 0)
			{
				if (ny != 0)vy = 0;
				isFalling = true;
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_LISTITEMBRICK)
		{
			if (ny != 0) vy = 0;
			int vitrilist2 = 0;
			ListItemBrick* listbrick = dynamic_cast<ListItemBrick*>(e->obj);
			// mario nhảy từ dưới lên va chạm gạch 
			if (e->ny > 0)
			{
				vy = 0;
				isFalling = true;
				if (listbrick->Bricks.size() == 1)
				{
					listbrick->DeleteBrick(0);
				}
				else if (listbrick->Bricks.size() == 2)
				{
					float l, t, r, b;
					int vitri = 0;
					GetBoundingBox(l, t, r, b);
					if ((l <= listbrick->Bricks.at(0)->x) || ( l + Width/2 <= listbrick->Bricks.at(0)->x + 16 && l + Width / 2 > listbrick->Bricks.at(0)->x))// sure đụng viên đầu tiên
					{
						vitri = 0;
						listbrick->DeleteBrick(vitri);
						//listbrick->Bricks.erase(listbrick->Bricks.begin());
					}
					else //if (r >= listbrick->Bricks.at(1)->x + 16) // sure đụng viên cuối
					{
						vitri = 1;
						listbrick->DeleteBrick(vitri);
						//listbrick->Bricks.erase(listbrick->Bricks.begin() + vitri);
					}
				}
				else
				{
					float l, t, r, b;
					int vitri = 0;
					GetBoundingBox(l, t, r, b);
					float aa1 = listbrick->Bricks.at(listbrick->Bricks.size() - 1)->x;
					if ( l <= listbrick->Bricks.at(0)->x || 
					(l > listbrick->Bricks.at(0)->x && r < listbrick->Bricks.at(1)->x + 8)) // sure đụng viên đầu tiên
					{
						vitri = 0;
						listbrick->DeleteBrick(vitri);
						
						//listbrick->Bricks.erase(listbrick->Bricks.begin());
					}
					// sure đụng viên cuối
					else if ((l <= aa1 && l + 8 >= aa1) ||
							 (l >= aa1))
					{
						vitri = listbrick->Bricks.size() - 1;
						listbrick->DeleteBrick(vitri);
						//listbrick->Bricks.erase(listbrick->Bricks.begin() + vitri);
					}
					else
					{
						int vitri = (int)(r - listbrick->Bricks.at(0)->x) / 16;
						//if (vitri > listbrick->Bricks.size()) // TH đặc biệt : 4 viên, 64/16 = 4 = vitri trong khi vitri = 3
						//	vitri--;
						float tempx = listbrick->Bricks.at(vitri)->x;
						if (l < tempx && INT16(tempx - l) > 8)
							vitri--;
						if (listbrick->Bricks.at(vitri)->Item == NORMAL)
						{
							int a1 = vitri + 1;
							int a2 = listbrick->Bricks.size() - 1;
							ListItemBrick* listbrick1 = new ListItemBrick(listbrick->Bricks, 0, vitri - 1);
							ListItemBrick* listbrick2 = new ListItemBrick(listbrick->Bricks, a1, a2);
							
							listbrick->canDelete = true;
							_Grid->AddStaticObject(listbrick1, listbrick1->Bricks.at(0)->x, listbrick1->Bricks.at(0)->y);
							_Grid->AddStaticObject(listbrick2, listbrick2->Bricks.at(0)->x, listbrick2->Bricks.at(0)->y);
						}
						else
						{

						}
						listbrick->DeleteBrick(vitri);
						//listbrick->Bricks.erase(listbrick->Bricks.begin() + vitri);
					}
				}
			}
			else if (e->ny < 0) // mario đi trên gạch
			{
				if (OnGround == false)
					y += min_ty * dy + ny * 0.2f;
				OnGround = true; // xử lý chạm đất
				isFalling = isFlyingLow = isFlyingHigh = false;
				x += min_tx * dx + nx * 0.4f;
			}
			else if (e->nx != 0)
			{
				y += min_ty * dy + ny * 0.4f;

			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_LISTQUESTIONBRICK)
		{
			int vitrilist2 = 0;
			ListQuestionBrick* listbrick = dynamic_cast<ListQuestionBrick*>(e->obj);
			// mario nhảy từ dưới lên va chạm gạch 
			if (e->ny > 0)
			{
				vy = 0;
				isFalling = true;
				if (listbrick->Bricks.size() == 1)
				{
					listbrick->DeleteBrick(0);
				}
				else if (listbrick->Bricks.size() == 2)
				{
					float l, t, r, b;
					int vitri = 0;
					GetBoundingBox(l, t, r, b);
					if ((l <= listbrick->Bricks.at(0)->x) || (l + Width / 2 <= listbrick->Bricks.at(0)->x + 16 && l + Width / 2 > listbrick->Bricks.at(0)->x))// sure đụng viên đầu tiên
					{
						vitri = 0;
						listbrick->DeleteBrick(vitri);
					}
					else //if (r >= listbrick->Bricks.at(1)->x + 16) // sure đụng viên cuối
					{
						vitri = 1;
						listbrick->DeleteBrick(vitri);
					}
				}
				else
				{
					float l, t, r, b;
					int vitri = 0;
					GetBoundingBox(l, t, r, b);
					if (l <= listbrick->Bricks.at(0)->x) // sure đụng viên đầu tiên
					{
						vitri = 0;
						listbrick->DeleteBrick(vitri);

						//listbrick->Bricks.erase(listbrick->Bricks.begin());
					}
					else if (r >= listbrick->Bricks.at(listbrick->Bricks.size() - 1)->x + 16) // sure đụng viên cuối
					{
						vitri = listbrick->Bricks.size() - 1;
						listbrick->DeleteBrick(vitri);
						//listbrick->Bricks.erase(listbrick->Bricks.begin() + vitri);
					}
					else
					{
						int vitri = (int)(r - listbrick->Bricks.at(0)->x) / 16;
						//if (vitri > listbrick->Bricks.size()) // TH đặc biệt : 4 viên, 64/16 = 4 = vitri trong khi vitri = 3
						//	vitri--;
						float tempx = listbrick->Bricks.at(vitri)->x;
						if (l < listbrick->Bricks.at(vitri)->x && tempx - l > 8)
							vitri--;
						listbrick->DeleteBrick(vitri);
						//listbrick->Bricks.erase(listbrick->Bricks.begin() + vitri);
					}
				}
			}
			else if (e->ny < 0) // mario đi trên gạch "?"
			{
				OnGround = true; // xử lý chạm đất
				isFalling = isFlyingLow = isFlyingHigh = false;

				x += min_tx * dx + nx * 0.4f;
				if (ny != 0) vy = 0;
			}
			else if (e->nx != 0)
			{
				y += min_ty * dy + ny * 0.2f;
				if (ny != 0) vy = 0;
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_LISTNORMALBRICK)
		{
			ListNormalBrick* listbrick = dynamic_cast<ListNormalBrick*>(e->obj);
			if (listbrick->TypeListNormalBrick == BRICK_CLOUD) // mây
			{
				if (isFlyingHigh == true) // đang bay thì xuyên qua
				{
					x += dx;
					y += dy;
				}
			}
			if (e->nx != 0)
			{
				y += min_ty * dy + ny * 0.2f;
				if (ny != 0)vy = 0;
			}
			else if (e->ny < 0)// mario đi trên gạch
			{
				OnGround = true; // xử lý chạm đất
				isFalling = isFlyingLow = isFlyingHigh = false;
				x += min_tx * dx + nx * 0.4f;
				if (ny != 0)vy = 0;
			}
			else if (e->ny > 0)
			{
				isFalling = true;
				if (ny != 0)vy = 0;
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_PORTAL)
		{
			if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);

				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_MUSICBRICK)
		{
			if (ny != 0) vy = 0;
			MusicBrick* brick = dynamic_cast<MusicBrick*>(e->obj);
		
			if (e->nx != 0)
			{
				if (nx != 0) vx = 0;
				if (ny != 0)vy = 0;
				y += min_ty * dy + ny * 0.1f - 0.4f;
			}
			else if (e->ny < 0) // tren xuong duoi
			{
				if(brick->GetState() != MUSICBRICK_STATE_COLLISION)
					brick->SetState(MUSICBRICK_STATE_COLLISION);
			}
			else if (e->ny > 0) // duoi len tren
			{
				if (ny != 0)vy = 0;
				isFalling = true;
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_HIDDENMUSICBRICK)
		{
			HiddenMusicBrick* brick = dynamic_cast<HiddenMusicBrick*>(e->obj);
			if (brick->isHidden == true)
			{
				if (e->ny > 0) // duoi len tren
				{
					if (ny != 0)vy = 0;
					isFalling = true;
					brick->SetState(MUSICBRICK_STATE_APPEAR);
				}
				else
				{
					x += dx;
					y += dy;
				}
			}
			else
			{
				if (e->nx != 0)
				{
					if (ny != 0) vy = 0;
					y += min_ty * dy + ny * 0.1f - 0.4f;
					if (e->nx < 0) // trai -> phai
					{
						brick->direction = 0;
					}
					else // phai -> trai
					{
						brick->direction = 2;
					}

					if (brick->GetState() != MUSICBRICK_STATE_COLLISION)
					{
						brick->SetState(MUSICBRICK_STATE_COLLISION);
						vx = -vx * 2 / 3;
					}
				}
				else if (e->ny < 0) // tren xuong duoi
				{
					if (ny != 0) vy = 0;
					brick->direction = 1;
					if (brick->GetState() != MUSICBRICK_STATE_COLLISION)
						brick->SetState(MUSICBRICK_STATE_COLLISION);
				}
				else if (e->ny > 0) // duoi len tren
				{
					if (ny != 0)vy = 0;
					isFalling = true;
					brick->direction = 3;
					if (brick->GetState() != MUSICBRICK_STATE_COLLISION)
						brick->SetState(MUSICBRICK_STATE_COLLISION);
				}
			}
		}
	}
}

void Mario::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	this->nScore = 0;

	if (e->obj->ObjType == OBJECT_TYPE_QUESTIONBRICKITEM)
	{
		if (dynamic_cast<QuestionBrickItem*>(e->obj))
		{
			QuestionBrickItem* questionbrickitem = dynamic_cast<QuestionBrickItem*>(e->obj);
			questionbrickitem->canDelete = true;
			_HUD->UpdateScore(questionbrickitem, 0);
			if (level <= MARIO_LEVEL_BIG)
			{
				if (questionbrickitem->Item >= this->level)
					UpLevel();
			}
			else // level tail/fire
			{
				if (questionbrickitem->Item != this->level - 1)
				{
					this->level = MARIO_LEVEL_BIG;
					UpLevel();
				}
			}
		}
	}
	else if (e->obj->ObjType == OBJECT_TYPE_BRICKITEM)
	{
		BrickItem* brickitem = dynamic_cast<BrickItem*>(e->obj);

		switch (brickitem->Item)
		{
		case MUSHROOM:
		{
			brickitem->canDelete = true;
			_HUD->UpdateScore(e->obj, nScore);

		}
		break;

		case BUTTONP:
		{
			if (ny != 0) vy = 0;
			if (e->nx != 0)
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
			}
			else if (e->ny < 0)
			{
				OnGround = true; // xử lý chạm đất
				isFalling = isFlyingLow = isFlyingHigh = false;
				brickitem->SetState(BRICKITEM_STATE_COLLISION);
			}
			else
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.1f - 0.4f;
			}

		}
		break;
		}
	}
	else if (e->obj->ObjType == OBJECT_TYPE_COIN)
	{
		if (dynamic_cast<Coin*>(e->obj))
		{
			Coin* coin = dynamic_cast<Coin*>(e->obj);
			coin->canDelete = true;
			_HUD->UpdateScore(coin, 0);
			/*if (OnGround == false)
				y += dy;*/
		}
	}
	else if (e->obj->ObjType == OBJECT_TYPE_CARD)
	{
		if (ny != 0) vy = 0;
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		Card* card = dynamic_cast<Card*>(e->obj);
		if (card->GetState() == CARD_STATE_NORMAL)
		{
			card->SetState(CARD_STATE_EMPTY);
			this->SetState(MARIO_STATE_ENDSCENE);
			_HUD->UpdateItem(card->TypeItem);
			float a = _Mario->x;
			float b = _Mario->y;
			float c = a - 100;
			Notification* noti = new Notification(c, b + 50 , card->TypeItem);
			_Grid->AddMovingObject(noti, c, b + 50);
			_PlayScene->CourseClear = true;
		}
	}
}

void Mario::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj->ObjType == OBJECT_TYPE_FIREBULLET)
	{
		x += dx;
		if (e->ny < 0)
			y += dy;
		FireBullet* firebullet = dynamic_cast<FireBullet*>(e->obj);
		if (firebullet->FireMario == false)
		{
			if (untouchable == false)
			{
				DownLevel();
			}
		}
	}
	else if (e->obj->ObjType == OBJECT_TYPE_BOOMERANG)
	{
		x += dx;
		if (e->ny < 0)
			y += dy;
		BoomerangWeapon* boomerang = dynamic_cast<BoomerangWeapon*>(e->obj);
		if (boomerang->isMarioWeapon == false && boomerang->vx != 0)
		{
			if (untouchable == false)
			{
				DownLevel();
			}
		}
	}
}

float Mario::GetWidth(int level)
{
	switch (level)
	{
		case MARIO_LEVEL_SMALL:
		{
			return MARIO_SMALL_BBOX_WIDTH;
		}break;

		case MARIO_LEVEL_BIG: case MARIO_LEVEL_FIRE:
		{
			return MARIO_BIG_BBOX_WIDTH;
		}break;

		case MARIO_LEVEL_TAIL:
		{
			return MARIO_TAIL_BBOX_WIDTH;
		}break;
	}
	return 0.0f;
}
