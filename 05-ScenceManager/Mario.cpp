#include "Mario.h"
#include "FireBullet.h"
#include "Brick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include "Block.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "QuestionBrickItem.h"
#include "FireBullet.h"
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
#include "PlayScence.h"
#include "Card.h"
#include "FlyWood.h"
#include "BoomerangWeapon.h"
#include "Notification.h"
#include "BoomerangEnemy.h"
#include "RedFlyKoopas.h"
#include "ListBrick.h"

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
		= endAttack = isLevelUp = test = render_tail = pressS = isLevelDown = loseControl = false;
	OnGround = true;
	level_of_walking = level_of_running = 0;
	nScore = 0;
	time_attack = time_fly = iChangeLevelTime = FlyTimePer1 = 0;
	TimeJumpS = 0;
	ani = 0;
	NumberBullet = 2;
	untouchable_start = ChangeLevelTime = UntouchtableTime = 0;
	MaxY = 0;
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	ULONGLONG a = GetTickCount64() - ChangeLevelTime - ((GetTickCount64() - ChangeLevelTime) % LEVEL_UP_DIVIDE);
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
			left = x + MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH;
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
				iChangeLevelTime = 0;
				ChangeLevelTime = 0;
				level++;
				y += static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);

			}
		}break;
		}
		y = (float)static_cast<int>(y);
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
		loseControl = false;
		// Calculate dx, dy 
		CGameObject::Update(dt);

		if (canFlyX == true)
		{
			time_fly++;
			// hết thời gian bay
			if (time_fly > TIME_FLY)
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
			FlyTimePer1++;
			if (FlyTimePer1 > TIME_FLY_S)
			{
				FlyTimePer1 = 0;
				isFlyingHigh = false;
			}
			if (time_fly > TIME_FLY)
			{
				time_fly = 0;
				isMaxRunning = canFlyS = isFlyingHigh = false;
				vy = 0;
				SetState(MARIO_STATE_IDLE);
			}
		}

		if (GoHiddenWorld == false) // để mario rớt từ từ khi đi vào map ẩn
		{
			if (isFlyingLow == false) // rớt bình thường
				vy += MARIO_GRAVITY * dt;
			else
			{
				// rớt chậm khi vẫy đuôi
				vy += MARIO_GRAVITY_FLYING * dt;
				if (TimeDelayFly != 0 && GetTickCount64() - TimeDelayFly > 150)
				{
					isFlyingLow = false;
					TimeDelayFly = 0;
				}
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
			if (level_of_running > 0 && isMaxRunning == false)
				level_of_running--;
			if (pressS == false && TimeJumpS != 0 && GetTickCount64() - TimeJumpS > 100 && isFalling == false)
			{
				this->vy /= 2;
				TimeJumpS = 0;
			}
			if (vy > 0)
				isFalling = true;
			if (nScore >= 9)
				nScore = 0;
		}

		if (isAttacking == true) // reset lại isAttacking và hiện ani quất đuôi khi isAttacking=true
		{
			// khi ani quất đuôi hiện lên thì mới bắt đầu tính, tránh trường hợp ani chưa bằng MARIO_ANI_TAIL_ATTACK_1 nhưng time vẫn + lên bằng 1
			if (ani >= MARIO_ANI_TAIL_ATTACK_1)
			{
				time_attack++;
				if (time_attack >= 5 * TIME_ATTACK)
				{
					time_attack = 0;
					isAttacking = false;
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

		if (GetState() == MARIO_STATE_WALKING_RIGHT)
		{
			if (level_of_walking < MAX_LEVEL_OF_WALKING)
				level_of_walking++;
			// đi bộ
			if (isRunning == false)
			{
				if (level_of_running > 0) // sau khi thả nút A thì trừ level_of_running, tránh trường hợp đợi đến state IDLE mới trừ
				{
					if (canFlyX == false && canFlyS == false)
						level_of_running--;
				}
				if (vx < 0)
				{
					ChangeDirection = true;
					vx += 2 * GIA_TOC;

					if (vx >= 0)
						ChangeDirection = false;
				}
				else
				{
					if (isFlyingHigh == true)
					{
						//vx = MARIO_FLY_MOVING_SPEED;
					}
					else
					{
						ChangeDirection = false;
						if (level_of_walking >= 4)
							vx = level_of_walking * GIA_TOC;
						if (level_of_walking < MAX_LEVEL_OF_WALKING)
							level_of_walking++;
					}
				}
			}
			// chạy
			else
			{
				// đang đi bên trái -> khựng
				if (vx < 0 && isHolding != true)
				{
					ChangeDirection = true;
					vx += float(4 * GIA_TOC);

					if (level_of_running > 2 && canFlyX == false && canFlyS == false)
						level_of_running -= 2;

					if (vx >= 0)
						ChangeDirection = false;
				}
				// đang đứng yên / di chuyển sang phải bình thường
				else
				{
					ChangeDirection = false;
					if (OnGround == true)
					{
						if (level_of_running < MAX_LEVEL_OF_RUNNING)
							level_of_running++;
						vx = (level_of_running * GIA_TOC);
						if (abs(vx) > abs(MARIO_RUNNING_MAX_SPEED))
							vx = MARIO_RUNNING_MAX_SPEED;
					}
					else
					{
						if (isMaxRunning == false)
							vx = level_of_walking * GIA_TOC;
						else
							vx = MARIO_RUNNING_MAX_SPEED;
					}
				}
			}
			if (isSitDown == true && level != MARIO_LEVEL_SMALL) // TH vừa ngồi vừa đi thì ưu tiên ani đi bộ
			{
				y = y - 10;
				isSitDown = false;
			}
		}
		else if (GetState() == MARIO_STATE_WALKING_LEFT)
		{
			if (level_of_walking < MAX_LEVEL_OF_WALKING)
				level_of_walking++;
			if (isRunning == false)
			{
				if (level_of_running > 0) // sau khi thả nút A thì trừ level_of_running, tránh trường hợp đợi đến state IDLE mới trừ
				{
					if (canFlyX == false && canFlyS == false)
						level_of_running--;
				}
				// đang đi qua bên phải (vx > 0) thì đột ngột qua trái -> khúc này xử lý vụ khựng lại r chuyển hướng
				if (vx > 0)
				{
					// bắt đầu chuyển hướng di chuyển
					ChangeDirection = true;

					// trừ bớt vx chứ không gán trực tiếp để nó hiện animation khựng
					// vx -= (level_of_walking * GIA_TOC); ->không nhân với level để vx giảm chậm để hiện animation
					vx -= 2 * GIA_TOC;

					if (level_of_walking > 0)
						level_of_walking--;

					// vx đạt tới mức di chuyển sang trái thì hủy chuyển hướng để trở về animation WALKING_LEFT
					// nếu không thì vẫn sẽ là animation STOP_RIGHT đi về bên trái
					if (vx <= 0)
						ChangeDirection = false;
				}
				else
				{
					if (isFlyingHigh == true)
					{
						//vx = -MARIO_FLY_MOVING_SPEED;
					}
					else
					{
						ChangeDirection = false;
						if (level_of_walking >= 4)
							vx = -(level_of_walking * GIA_TOC);
						if (level_of_walking < MAX_LEVEL_OF_WALKING)
							level_of_walking++;
					}
				}
			}
			else
			{
				if (vx > 0 && isHolding != true)
				{
					ChangeDirection = true;
					vx -= float(4 * GIA_TOC);

					if (level_of_running > 2 && canFlyX == false && canFlyS == false)
						level_of_running -= 2;

					if (vx <= 0)
						ChangeDirection = false;
				}
				else
				{
					ChangeDirection = false;
					if (OnGround == true)
					{
						if (level_of_running < MAX_LEVEL_OF_RUNNING)
							level_of_running++;
						vx = -(level_of_running * GIA_TOC);
						if (abs(vx) > abs(MARIO_RUNNING_MAX_SPEED))
							vx = -MARIO_RUNNING_MAX_SPEED;
					}
					else
					{
						vx = -(level_of_walking * GIA_TOC);
					}
				}
			}
			if (isSitDown == true && level != MARIO_LEVEL_SMALL)
			{
				y = y - 10;
				isSitDown = false;
			}
		}
		else if (GetState() == MARIO_STATE_SITDOWN)
		{
			if (level_of_walking == 0 && level_of_running == 0)
			{
				vx = 0;
				ChangeDirection = false;
			}
			else
			{
				if (vx > 0)
				{
					if (level_of_running > 0)
					{
						//vx -= 2 * GIA_TOC;
						vx -= 1 * GIA_TOC;
						if (vx < 0.0f)
							vx = 0.0f;
					}
					// đi bộ
					else if (level_of_walking > 0)
					{
						vx -= GIA_TOC;
					}
				}
				else if (vx < 0)
				{
					if (level_of_running > 0)
					{
						//vx += 2 * GIA_TOC;
						vx += 1 * GIA_TOC;
						if (vx > 0.0f)
							vx = 0.0f;
					}
					else if (level_of_walking > 0)
					{
						vx += GIA_TOC;
					}
				}
			}

			// đang trong trạng thái bay thì k trừ 
			if (canFlyX == false && canFlyS == false)
			{
				if (level_of_walking > 0)
					level_of_walking--;
				/*else if (level_of_walking == 0)
					vx = 0;*/

				if (level_of_running > 0)
					level_of_running--;
				/*else if (level_of_running == 0)
					vx = 0;*/
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

		if (isMaxRunning == true && abs(vx) > MARIO_RUNNING_MAX_SPEED)
		{
			if (nx == RIGHT)
			{
				vx = MARIO_RUNNING_MAX_SPEED;
				if (OnGround == false)
				{
					vx = MARIO_RUNNING_MAX_SPEED;
					//vy += 0.0003 * dt;
				}

			}

			else
				vx = -MARIO_RUNNING_MAX_SPEED;
		}

		if (level_of_running == MAX_LEVEL_OF_RUNNING) // đạt trạng thái max running
			isMaxRunning = true;
		else
		{
			if (isMaxRunning == true)
			{
				if (canFlyS == false && canFlyX == false)
					isMaxRunning = false;
			}
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

		if (y > _Map->GetHeight())
		{
			SetState(MARIO_STATE_DIE);
			CGame::GetInstance()->SwitchScene2(1);
			_HUD->MarioLife--;
			return;
		}
		if (vx < 0 && x < 0)
		{
			x = 0;
		}
		else
		{
			/*float a = _Mario->x + _Mario->Width;
				float b = _Map->GetWidth() - 16;*/
			if (_Mario->x + _Mario->Width >= _Map->GetWidth() - 16)
				_Mario->x = (float)(_Map->GetWidth() - 16 - _Mario->Width);
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		
		// turn off collision when die 
		if (state != MARIO_STATE_DIE)
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
			x += dx;
			y += dy;
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
							}
						}

					}
				}
				break;
				}

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
					case CATEGORY::GROUND:
					{
						Ground* ground = dynamic_cast<Ground*>(e->obj);
						this->nScore = 0;
						if (e->ny < 0)
						{
							x += min_tx * dx + nx * 0.4f;
							if (ny != 0) vy = 0;
							this->MaxY = ground->y - this->Height;
							if (OnGround == false)
							{
								float temp = min_ty * dy + ny * 0.1f - 0.3f;
								if (y + temp < this->MaxY)
									y += temp;
								else
									y = float(MaxY - 0.5);

								OnGround = true; // xử lý chạm đất
								isFalling = isFlyingLow = isFlyingHigh = false;
							}
						}
						else if (e->nx != 0)
						{
							y += min_ty * dy + ny * 0.1f - 0.3f;

						}
					}
					break;

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
							CollisionWithObject(e, min_tx, min_ty, nx, ny);
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

					case CATEGORY::PORTAL:
					{
						if (dynamic_cast<CPortal*>(e->obj))
						{
							CPortal* p = dynamic_cast<CPortal*>(e->obj);

							CGame::GetInstance()->SwitchScene(p->GetSceneId());
						}
					}
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

		
	}
	//Debug();
}

//void Mario::Render()
//{
//	if (isLevelUp == true)
//	{
//		switch (level)
//		{
//		case MARIO_LEVEL_SMALL:
//		{
//			//ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
//			if (nx == RIGHT)
//			{
//				/*if (GetTickCount64() - LevelUpTime - ((GetTickCount64() - LevelUpTime)%LEVEL_UP_DIVIDE) <= (TIME_LEVEL_UP / 2))
//				{
//					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
//						ani = MARIO_ANI_SMALL_IDLE_RIGHT;
//					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
//						ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
//				}
//				else
//				{
//					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
//						ani = MARIO_ANI_BIG_IDLE_RIGHT;
//					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
//						ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
//				}*/
//				if (iChangeLevelTime > 0 && iChangeLevelTime < ITIME_LEVEL_UP / 2)
//				{
//					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0) // 0->14 30 60
//						ani = MARIO_ANI_SMALL_IDLE_RIGHT;
//					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0) //15->29 45 75
//						ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
//				}
//				else
//				{
//					if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 == 0)
//						ani = MARIO_ANI_BIG_IDLE_RIGHT;
//					else if ((iChangeLevelTime / LEVEL_UP_DIVIDE) % 2 != 0)
//						ani = MARIO_ANI_UP_LEVEL_RIGHT_SMALL_BIG;
//				}
//			}
//			int alpha = 128;
//			animation_set->at(ani)->Render(x, y, alpha);
//		}break;
//		}
//	}
//	else
//	{
//		ani = -1;
//		if (state == MARIO_STATE_DIE)
//			ani = MARIO_ANI_DIE;
//		else
//		{
//			switch (level)
//			{
//			case MARIO_LEVEL_SMALL:
//			{
//				// chưa nhảy // đang rớt xuống
//				if (vy >= 0)
//				{
//					// đứng yên
//					if (level_of_walking == 0 && level_of_running == 0)
//					{
//						if (ChangeDirection == false)
//						{
//							// phải 
//							if (nx == RIGHT)
//							{
//								ani = MARIO_ANI_SMALL_IDLE_RIGHT;
//								if (isHolding == true)
//									ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
//							}
//							// trái
//							else
//							{
//								ani = MARIO_ANI_SMALL_IDLE_LEFT;
//								if (isHolding == true)
//									ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
//							}
//						}
//						else
//						{
//							if (nx == LEFT)
//								ani = MARIO_ANI_SMALL_STOP_LEFT;
//							else
//								ani = MARIO_ANI_SMALL_STOP_RIGHT;
//						}
//					}
//					// đi bộ qua phải
//					else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
//					{
//						if (ChangeDirection == false)
//						{
//							ani = MARIO_ANI_SMALL_WALKING_RIGHT;
//							if (isHolding == true)
//								ani = MARIO_ANI_SMALL_HOLD_WALK_RIGHT;
//							else if (isMaxRunning == true)
//								ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
//						}
//						else if (ChangeDirection == true)
//							ani = MARIO_ANI_SMALL_STOP_RIGHT;
//					}
//					//  đi bộ qua trái
//					else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
//					{
//						if (ChangeDirection == false)
//						{
//							ani = MARIO_ANI_SMALL_WALKING_LEFT;
//							if (isHolding == true)
//								ani = MARIO_ANI_SMALL_HOLD_WALK_LEFT;
//							else if (isMaxRunning == true)
//								ani = MARIO_ANI_SMALL_RUNNING_LEFT;
//						}
//						else if (ChangeDirection == true)
//							ani = MARIO_ANI_SMALL_STOP_LEFT;
//					}
//				}
//				// nhảy
//				else
//				{
//					// cầm rùa + nhảy
//					if (isHolding == true)
//					{
//						// phải 
//						if (nx == RIGHT)
//							ani = MARIO_ANI_SMALL_HOLD_JUMP_RIGHT;
//						// trái
//						else
//							ani = MARIO_ANI_SMALL_HOLD_JUMP_LEFT;
//					}
//					else
//					{
//						// phải 
//						if (nx == RIGHT)
//							ani = MARIO_ANI_SMALL_JUMP_RIGHT;
//						// trái
//						else
//							ani = MARIO_ANI_SMALL_JUMP_LEFT;
//					}
//				}
//
//				// đá
//				if (canKick == true)
//				{
//					if (nx == RIGHT)
//						ani = MARIO_ANI_SMALL_KICK_RIGHT;
//					else
//						ani = MARIO_ANI_SMALL_KICK_LEFT;
//				}
//			}
//			break;
//
//			case MARIO_LEVEL_BIG:
//			{
//				// đá
//				if (canKick == true)
//				{
//					if (nx == RIGHT)
//						ani = MARIO_ANI_BIG_KICK_RIGHT;
//					else
//						ani = MARIO_ANI_BIG_KICK_LEFT;
//				}
//				else
//				{
//					if (isSitDown == true)
//					{
//						if (nx == RIGHT)
//							ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
//						else
//							ani = MARIO_ANI_BIG_SITDOWN_LEFT;
//					}
//					else
//					{
//						// chưa nhảy / đang rớt xuống
//						if (vy >= 0)
//						{
//							// đang chạm đất
//							if (OnGround == true)
//							{
//								// đứng yên
//								if (level_of_walking == 0 && level_of_running == 0)
//								{
//									// đi thẳng k chuyển hướng
//									if (ChangeDirection == false)
//									{
//										// phải 
//										if (nx == RIGHT)
//										{
//											ani = MARIO_ANI_BIG_IDLE_RIGHT;
//											if (isHolding == true)
//												ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
//										}
//										// trái
//										else
//										{
//											ani = MARIO_ANI_BIG_IDLE_LEFT;
//											if (isHolding == true)
//												ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
//										}
//									}
//									else
//									{
//										if (nx == LEFT)
//											ani = MARIO_ANI_BIG_STOP_LEFT;
//										else
//											ani = MARIO_ANI_BIG_STOP_RIGHT;
//									}
//								}
//								// đi bộ qua phải
//								else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
//								{
//									if (ChangeDirection == false)
//									{
//										ani = MARIO_ANI_BIG_WALKING_RIGHT;
//										if (isHolding == true)
//											ani = MARIO_ANI_BIG_HOLD_WALK_RIGHT;
//										else if (isMaxRunning == true)
//											ani = MARIO_ANI_BIG_RUNNING_RIGHT;
//									}
//									else if (ChangeDirection == true)
//									{
//										ani = MARIO_ANI_BIG_STOP_RIGHT;
//									}
//								}
//								//  đi bộ qua trái
//								else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
//								{
//									if (ChangeDirection == false)
//									{
//										ani = MARIO_ANI_BIG_WALKING_LEFT;
//										if (isHolding == true)
//											ani = MARIO_ANI_BIG_HOLD_WALK_LEFT;
//										else if (isMaxRunning == true)
//											ani = MARIO_ANI_BIG_RUNNING_LEFT;
//									}
//									else if (ChangeDirection == true)
//									{
//										if (isHolding != true)
//											ani = MARIO_ANI_BIG_STOP_LEFT;
//									}
//								}
//							}
//							// rớt xuống
//							else
//							{
//								if (isHolding == true)
//								{
//									// phải 
//									if (nx == RIGHT)
//										ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;
//									// trái
//									else
//										ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;
//								}
//								else
//								{
//									// phải 
//									if (nx == RIGHT)
//										ani = MARIO_ANI_BIG_FALL_RIGHT;
//									// trái
//									else
//										ani = MARIO_ANI_BIG_FALL_LEFT;
//								}
//							}
//						}
//						// nhảy
//						else
//						{
//							if (isHolding == true)
//							{
//								// phải 
//								if (nx == RIGHT)
//									ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;
//								// trái
//								else
//									ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;
//							}
//							else
//							{
//								// phải 
//								if (nx == RIGHT)
//									ani = MARIO_ANI_BIG_JUMP_RIGHT;
//								// trái
//								else
//									ani = MARIO_ANI_BIG_JUMP_LEFT;
//							}
//						}
//					}
//				}
//			}
//			break;
//
//			case MARIO_LEVEL_TAIL:
//			{
//				if (canFlyX == true)
//				{
//					// chưa nhảy // đang rớt xuống
//					if (vy >= 0)
//					{
//						// đang chạm đất
//						if (OnGround == true)
//						{
//							// đứng yên
//							if (vx == 0)
//							{
//								// phải 
//								if (nx == RIGHT)
//								{
//									ani = MARIO_ANI_TAIL_IDLE_RIGHT;
//								}
//								// trái
//								else
//								{
//									ani = MARIO_ANI_TAIL_IDLE_LEFT;
//									
//								}
//							}
//							// đi bộ qua phải
//							else if ( vx > 0.f && nx == RIGHT)
//							{
//								if (ChangeDirection == false)
//								{
//									ani = MARIO_ANI_TAIL_WALKING_RIGHT;
//									if (isHolding == true)
//										ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//								}
//								else if (ChangeDirection == true)
//									ani = MARIO_ANI_TAIL_STOP_RIGHT;
//							}
//							//  đi bộ qua trái
//							else if ( vx < 0.f && nx == LEFT)
//							{
//								if (ChangeDirection == false)
//								{
//									ani = MARIO_ANI_TAIL_WALKING_LEFT;
//									if (isHolding == true)
//										ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
//								}
//								else if (ChangeDirection == true)
//									ani = MARIO_ANI_TAIL_STOP_LEFT;
//							}
//						}
//						// rớt xuống
//						else
//						{
//							// phải 
//							if (nx == RIGHT)
//							{
//								ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//							}
//							// trái
//							else
//							{
//								ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//							}
//						}
//					}
//					// nhảy
//					else
//					{
//						// phải 
//						if (nx == RIGHT)
//						{
//							if (isMaxRunning == true)
//							{
//								if (isFlyingHigh == true)
//									ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT;
//								else
//									ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//							}
//						}
//						// trái
//						else
//						{
//							if (isMaxRunning == true)
//							{
//								if (isFlyingHigh == true)
//									ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT;
//								else
//									ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//							}
//						}
//					}
//				}
//				else if (canFlyS == true)
//				{
//					// đang rớt xuống
//					if (vy >= 0)
//					{
//						// phải 
//						if (nx == RIGHT)
//						{
//							if (OnGround == false)
//								ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//							else if (OnGround == true)
//							{
//								if (level_of_walking == 0 && level_of_running == 0 || vx == 0)
//									ani = MARIO_ANI_TAIL_IDLE_RIGHT;
//								else if (level_of_walking > 0 || level_of_running > 0 && vx > 0)
//								{
//									if (ChangeDirection == false)
//									{
//										ani = MARIO_ANI_TAIL_WALKING_RIGHT;
//										if (isHolding == true)
//											ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//										else if (isMaxRunning == true)
//											ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
//									}
//									else if (ChangeDirection == true)
//										ani = MARIO_ANI_TAIL_STOP_RIGHT;
//
//								}
//							}
//
//						}
//						// trái
//						else
//						{
//							if (OnGround == false)
//								ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//							else if (OnGround == true)
//							{
//								if (level_of_walking == 0 && level_of_running == 0 && vx == 0)
//									ani = MARIO_ANI_TAIL_IDLE_LEFT;
//								else if (level_of_walking > 0 || level_of_running > 0 && vx < 0)
//								{
//									if (ChangeDirection == false)
//									{
//										ani = MARIO_ANI_TAIL_WALKING_LEFT;
//										if (isHolding == true)
//											ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
//										else if (isMaxRunning == true)
//											ani = MARIO_ANI_TAIL_RUNNING_LEFT;
//									}
//									else if (ChangeDirection == true)
//										ani = MARIO_ANI_TAIL_STOP_LEFT;
//
//								}
//							}
//						}
//					}
//					// nhảy
//					else
//					{
//						// phải 
//						if (nx == RIGHT)
//						{
//							//if (isMaxRunning == true)// && OnGround == false)
//							//{
//							//	if (isFlyingHigh == true)
//							//		ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT;
//							//	else
//							//		ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
//							//}
//							ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_RIGHT;
//						}
//						// trái
//						else
//						{
//							/*if (isMaxRunning == true)
//							{
//								if (isFlyingHigh == true)
//									ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT;
//								else
//									ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
//							}*/
//							ani = MARIO_ANI_TAIL_FLYING_MAX_RUNNING_LEFT;
//						}
//					}
//				}
//				else
//				{
//					// chưa nhảy // đang rớt xuống
//					if (vy >= 0)
//					{
//						// đang chạm đất
//						if (OnGround == true)
//						{
//							// đứng yên
//							if ( vx == 0.f)
//							{
//								if (ChangeDirection == false)
//								{
//									// phải 
//									if (nx == RIGHT)
//									{
//										ani = MARIO_ANI_TAIL_IDLE_RIGHT;
//										if (isHolding == true)
//											ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
//									}
//									// trái
//									else
//									{
//										ani = MARIO_ANI_TAIL_IDLE_LEFT;
//										if (isHolding == true)
//											ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
//									}
//								}
//								else
//								{
//									if (nx == LEFT)
//										ani = MARIO_ANI_TAIL_STOP_LEFT;
//									else
//										ani = MARIO_ANI_TAIL_STOP_RIGHT;
//								}
//							}
//							// đi bộ qua phải
//							else if ( vx > 0.f && nx == RIGHT)
//							{
//								if (ChangeDirection == false)
//								{
//									ani = MARIO_ANI_TAIL_WALKING_RIGHT;
//									if (isHolding == true)
//										ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//									else if (isMaxRunning == true)
//										ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
//								}
//								else if (ChangeDirection == true)
//									ani = MARIO_ANI_TAIL_STOP_RIGHT;
//
//							}
//							//  đi bộ qua trái
//							else if ( vx < 0.f && nx == LEFT)
//							{
//								if (ChangeDirection == false)
//								{
//									ani = MARIO_ANI_TAIL_WALKING_LEFT;
//									if (isHolding == true)
//										ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
//									else if (isMaxRunning == true)
//										ani = MARIO_ANI_TAIL_RUNNING_LEFT;
//								}
//								else if (ChangeDirection == true)
//									ani = MARIO_ANI_TAIL_STOP_LEFT;
//
//							}
//						}
//						// rớt xuống
//						else
//						{
//							if (isHolding == true)
//							{
//								// phải 
//								if (nx == RIGHT)
//									ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
//								// trái
//								else
//									ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
//							}
//							else
//							{
//								// phải 
//								if (nx == RIGHT)
//								{
//									ani = MARIO_ANI_TAIL_FALLING_RIGHT;
//									if (isFlyingLow == true)
//										ani = MARIO_ANI_TAIL_FLYING_RIGHT;
//								}
//								// trái
//								else
//								{
//									ani = MARIO_ANI_TAIL_FALLING_LEFT;
//									if (isFlyingLow == true)
//										ani = MARIO_ANI_TAIL_FLYING_LEFT;
//								}
//							}
//						}
//					}
//					// nhảy
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
//								ani = MARIO_ANI_TAIL_JUMP_RIGHT;
//							}
//							// trái
//							else
//							{
//								ani = MARIO_ANI_TAIL_JUMP_LEFT;
//							}
//						}
//					}
//				}
//
//				// đá
//				if (canKick == true)
//				{
//					if (nx == RIGHT)
//						ani = MARIO_ANI_TAIL_KICK_RIGHT;
//					else
//						ani = MARIO_ANI_TAIL_KICK_LEFT;
//				}
//				else if (GoHiddenWorld == true)
//					ani = MARIO_ANI_TAIL_ATTACK_2;
//				else if (isAttacking == true && endAttack == false)
//				{
//					if (nx == RIGHT)
//					{
//						if (time_attack <= TIME_ATTACK)
//							ani = MARIO_ANI_TAIL_ATTACK_1;
//						else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//							ani = MARIO_ANI_TAIL_ATTACK_2;
//						else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//							ani = MARIO_ANI_TAIL_ATTACK_3;
//						else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//							ani = MARIO_ANI_TAIL_ATTACK_4;
//						else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//							ani = MARIO_ANI_TAIL_ATTACK_1;
//					}
//					else
//					{
//						if (time_attack <= TIME_ATTACK)
//						{
//							ani = MARIO_ANI_TAIL_ATTACK_3;
//						}
//						else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//						{
//							ani = MARIO_ANI_TAIL_ATTACK_2;
//						}
//						else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
//						{
//							ani = MARIO_ANI_TAIL_ATTACK_1;
//						}
//						else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
//						{
//							ani = MARIO_ANI_TAIL_ATTACK_4;
//						}
//						else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
//						{
//							ani = MARIO_ANI_TAIL_ATTACK_3;
//						}
//					}
//				}
//				else if (isSitDown == true)
//				{
//					if (nx == RIGHT)
//						ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
//					else
//						ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
//				}
//				else if (isHolding == true)
//				{
//					if (nx == RIGHT)
//						ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
//					else
//						ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
//				}
//
//			}
//			break;
//
//			case MARIO_LEVEL_FIRE:
//			{
//				if (isSitDown == true)
//				{
//					if (nx == RIGHT)
//						ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
//					else
//						ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
//				}
//				else
//				{
//					// chưa nhảy // đang rớt xuống
//					if (vy >= 0)
//					{
//						if (OnGround == true)
//						{
//							// đứng yên
//							if (level_of_walking == 0 && level_of_running == 0)
//							{
//								if (ChangeDirection == false)
//								{
//									// phải 
//									if (nx == RIGHT)
//									{
//										ani = MARIO_ANI_FIRE_IDLE_RIGHT;
//										if (isAttacking == true && endAttack == false)
//										{
//											if (time_attack <= TIME_ATTACK)
//											{
//												ani = MARIO_ANI_FIRE_ATTACK_RIGHT_1;
//											}
//											else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//											{
//												ani = MARIO_ANI_FIRE_ATTACK_RIGHT_2;
//											}
//										}
//									}
//
//									// trái
//									else
//									{
//										ani = MARIO_ANI_FIRE_IDLE_LEFT;
//										if (isAttacking == true && endAttack == false)
//										{
//											if (time_attack <= TIME_ATTACK)
//											{
//												ani = MARIO_ANI_FIRE_ATTACK_LEFT_1;
//											}
//											else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//											{
//												ani = MARIO_ANI_FIRE_ATTACK_LEFT_2;
//											}
//										}
//									}
//
//								}
//								else
//								{
//									if (nx == LEFT)
//										ani = MARIO_ANI_FIRE_STOP_LEFT;
//									else
//										ani = MARIO_ANI_FIRE_STOP_RIGHT;
//								}
//
//							}
//							// đi bộ qua phải
//							else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
//							{
//								// ani = MARIO_ANI_FIRE_WALKING_RIGHT;
//
//								if (ChangeDirection == false)
//								{
//									ani = MARIO_ANI_FIRE_WALKING_RIGHT;
//
//									if (isMaxRunning == true)
//										ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
//								}
//								else if (ChangeDirection == true)
//								{
//									ani = MARIO_ANI_FIRE_STOP_RIGHT;
//								}
//
//								if (isAttacking == true && endAttack == false)
//								{
//									if (time_attack <= TIME_ATTACK)
//									{
//										ani = MARIO_ANI_FIRE_ATTACK_RIGHT_1;
//									}
//									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_FIRE_ATTACK_RIGHT_2;
//									}
//
//								}
//							}
//							//  đi bộ qua trái
//							else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
//							{
//								if (ChangeDirection == false)
//								{
//									ani = MARIO_ANI_FIRE_WALKING_LEFT;
//
//									if (isMaxRunning == true)
//										ani = MARIO_ANI_FIRE_RUNNING_LEFT;
//								}
//								else if (ChangeDirection == true)
//								{
//									ani = MARIO_ANI_FIRE_STOP_LEFT;
//								}
//
//								if (isAttacking == true && endAttack == false)
//								{
//									if (time_attack <= TIME_ATTACK)
//									{
//										ani = MARIO_ANI_FIRE_ATTACK_LEFT_1;
//									}
//									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//									{
//										ani = MARIO_ANI_FIRE_ATTACK_LEFT_2;
//									}
//
//								}
//							}
//						}
//						else
//						{
//							// đứng yên
//							if (vx == 0)
//							{
//								// phải 
//								if (nx == RIGHT)
//									ani = MARIO_ANI_FIRE_JUMP_RIGHT;
//								// trái
//								else
//									ani = MARIO_ANI_FIRE_JUMP_LEFT;
//							}
//							// đi bộ qua phải
//							else if (vx > 0)
//								ani = MARIO_ANI_FIRE_JUMP_RIGHT;
//							else
//								ani = MARIO_ANI_FIRE_JUMP_LEFT;
//						}
//					}
//					// nhảy
//					else
//					{
//						// đứng yên
//						if (vx == 0)
//						{
//							// phải 
//							if (nx == RIGHT)
//								ani = MARIO_ANI_FIRE_JUMP_RIGHT;
//							// trái
//							else
//								ani = MARIO_ANI_FIRE_JUMP_LEFT;
//						}
//						// đi bộ qua phải
//						else if (vx > 0)
//						{
//							ani = MARIO_ANI_FIRE_JUMP_RIGHT;
//						}
//						//  đi bộ qua trái
//						else
//						{
//							ani = MARIO_ANI_FIRE_JUMP_LEFT;
//						}
//					}
//
//					if (isAttacking == true && endAttack == false)
//					{
//						if (nx == RIGHT)
//						{
//							if (time_attack <= TIME_ATTACK)
//							{
//								ani = MARIO_ANI_FIRE_ATTACK_RIGHT_1;
//							}
//							else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//							{
//								ani = MARIO_ANI_FIRE_ATTACK_RIGHT_2;
//							}
//						}
//						else
//						{
//							if (time_attack <= TIME_ATTACK)
//							{
//								ani = MARIO_ANI_FIRE_ATTACK_LEFT_1;
//							}
//							else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
//							{
//								ani = MARIO_ANI_FIRE_ATTACK_LEFT_2;
//							}
//						}
//					}
//				}
//			}
//			break;
//			}
//		}
//
//		int alpha = 255;
//		if (untouchable) alpha = 128;
//
//		animation_set->at(ani)->Render(x, y, alpha);
//
//		DebugOut(L"RENDER ani = %i\n", ani);
//	}
//	//RenderBoundingBox();
//}

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
			switch (level)
			{
			case MARIO_LEVEL_SMALL:
			{
				// chưa nhảy // đang rớt xuống
				if (vy >= 0)
				{
					// đứng yên
					if (level_of_walking == 0 && level_of_running == 0)
					{
						if (ChangeDirection == false)
						{
							// phải 
							if (nx == RIGHT)
							{
								ani = MARIO_ANI_SMALL_IDLE_RIGHT;
								if (isHolding == true)
									ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
							}
							// trái
							else
							{
								ani = MARIO_ANI_SMALL_IDLE_LEFT;
								if (isHolding == true)
									ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
							}
						}
						else
						{
							if (nx == LEFT)
								ani = MARIO_ANI_SMALL_STOP_LEFT;
							else
								ani = MARIO_ANI_SMALL_STOP_RIGHT;
						}
					}
					// đi bộ qua phải
					else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
					{
						if (ChangeDirection == false)
						{
							ani = MARIO_ANI_SMALL_WALKING_RIGHT;
							if (isHolding == true)
								ani = MARIO_ANI_SMALL_HOLD_WALK_RIGHT;
							else if (isMaxRunning == true)
								ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
						}
						else if (ChangeDirection == true)
							ani = MARIO_ANI_SMALL_STOP_RIGHT;
					}
					//  đi bộ qua trái
					else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
					{
						if (ChangeDirection == false)
						{
							ani = MARIO_ANI_SMALL_WALKING_LEFT;
							if (isHolding == true)
								ani = MARIO_ANI_SMALL_HOLD_WALK_LEFT;
							else if (isMaxRunning == true)
								ani = MARIO_ANI_SMALL_RUNNING_LEFT;
						}
						else if (ChangeDirection == true)
							ani = MARIO_ANI_SMALL_STOP_LEFT;
					}

					if (OnGround == false && isFalling == true)
					{
						if (nx == RIGHT)
							ani = MARIO_ANI_SMALL_JUMP_RIGHT;
						else
							ani = MARIO_ANI_SMALL_JUMP_LEFT;
					}
						
				}
				// nhảy
				else
				{
					// cầm rùa + nhảy
					if (isHolding == true)
					{
						// phải 
						if (nx == RIGHT)
							ani = MARIO_ANI_SMALL_HOLD_JUMP_RIGHT;
						// trái
						else
							ani = MARIO_ANI_SMALL_HOLD_JUMP_LEFT;
					}
					else
					{
						// phải 
						if (nx == RIGHT)
							ani = MARIO_ANI_SMALL_JUMP_RIGHT;
						// trái
						else
							ani = MARIO_ANI_SMALL_JUMP_LEFT;
					}
				}

				// đá
				if (canKick == true)
				{
					if (nx == RIGHT)
						ani = MARIO_ANI_SMALL_KICK_RIGHT;
					else
						ani = MARIO_ANI_SMALL_KICK_LEFT;
				}
			}
			break;

			case MARIO_LEVEL_BIG:
			{
				// đá
				if (canKick == true)
				{
					if (nx == RIGHT)
						ani = MARIO_ANI_BIG_KICK_RIGHT;
					else
						ani = MARIO_ANI_BIG_KICK_LEFT;
				}
				else
				{
					if (isSitDown == true)
					{
						if (nx == RIGHT)
							ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
						else
							ani = MARIO_ANI_BIG_SITDOWN_LEFT;
					}
					else
					{
						// chưa nhảy / đang rớt xuống
						if (vy >= 0)
						{
							// đang chạm đất
							if (OnGround == true)
							{
								// đứng yên
								if (level_of_walking == 0 && level_of_running == 0)
								{
									// đi thẳng k chuyển hướng
									if (ChangeDirection == false)
									{
										// phải 
										if (nx == RIGHT)
										{
											ani = MARIO_ANI_BIG_IDLE_RIGHT;
											if (isHolding == true)
												ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
										}
										// trái
										else
										{
											ani = MARIO_ANI_BIG_IDLE_LEFT;
											if (isHolding == true)
												ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
										}
									}
									else
									{
										if (nx == LEFT)
											ani = MARIO_ANI_BIG_STOP_LEFT;
										else
											ani = MARIO_ANI_BIG_STOP_RIGHT;
									}
								}
								// đi bộ qua phải
								else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
								{
									if (ChangeDirection == false)
									{
										ani = MARIO_ANI_BIG_WALKING_RIGHT;
										if (isHolding == true)
											ani = MARIO_ANI_BIG_HOLD_WALK_RIGHT;
										else if (isMaxRunning == true)
											ani = MARIO_ANI_BIG_RUNNING_RIGHT;
									}
									else if (ChangeDirection == true)
									{
										ani = MARIO_ANI_BIG_STOP_RIGHT;
									}
								}
								//  đi bộ qua trái
								else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
								{
									if (ChangeDirection == false)
									{
										ani = MARIO_ANI_BIG_WALKING_LEFT;
										if (isHolding == true)
											ani = MARIO_ANI_BIG_HOLD_WALK_LEFT;
										else if (isMaxRunning == true)
											ani = MARIO_ANI_BIG_RUNNING_LEFT;
									}
									else if (ChangeDirection == true)
									{
										if (isHolding != true)
											ani = MARIO_ANI_BIG_STOP_LEFT;
										else
											ani = MARIO_ANI_BIG_HOLD_WALK_LEFT;
									}
								}
							}
							// rớt xuống
							else
							{
								if (isHolding == true)
								{
									// phải 
									if (nx == RIGHT)
										ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;
									// trái
									else
										ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;
								}
								else
								{
									// phải 
									if (nx == RIGHT)
										ani = MARIO_ANI_BIG_FALL_RIGHT;
									// trái
									else
										ani = MARIO_ANI_BIG_FALL_LEFT;
								}
							}
						}
						// nhảy
						else
						{
							if (isHolding == true)
							{
								// phải 
								if (nx == RIGHT)
									ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;
								// trái
								else
									ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;
							}
							else
							{
								// phải 
								if (nx == RIGHT)
									ani = MARIO_ANI_BIG_JUMP_RIGHT;
								// trái
								else
									ani = MARIO_ANI_BIG_JUMP_LEFT;
							}
						}
					}
				}
			}
			break;

			case MARIO_LEVEL_TAIL:
			{
				if (canFlyX == true || canFlyS == true)
				{
					// chưa nhảy // đang rớt xuống
					if (vy >= 0)
					{
						// đang chạm đất
						if (OnGround == true)
						{
							// đứng yên
							if (level_of_walking == 0 && level_of_running == 0 || vx == 0)
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
							else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
							{
								if (ChangeDirection == false)
								{
									ani = MARIO_ANI_TAIL_WALKING_RIGHT;
								}
								else if (ChangeDirection == true)
									ani = MARIO_ANI_TAIL_STOP_RIGHT;
							}
							//  đi bộ qua trái
							else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
							{
								if (ChangeDirection == false)
								{
									ani = MARIO_ANI_TAIL_WALKING_LEFT;
								}
								else if (ChangeDirection == true)
									ani = MARIO_ANI_TAIL_STOP_LEFT;
							}
						}
						// rớt xuống
						else
						{
							// phải 
							if (nx == RIGHT)
							{
								ani = MARIO_ANI_TAIL_FLY_FALL_RIGHT;
							}
							// trái
							else
							{
								ani = MARIO_ANI_TAIL_FLY_FALL_LEFT;
							}
						}

					}
					// nhảy
					else
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

					// đá
					if (canKick == true)
					{
						if (nx == RIGHT)
							ani = MARIO_ANI_TAIL_KICK_RIGHT;
						else
							ani = MARIO_ANI_TAIL_KICK_LEFT;
					}
					else if (GoHiddenWorld == true)
						ani = MARIO_ANI_TAIL_ATTACK_2;
					else if (isAttacking == true && endAttack == false)
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
					else if (isSitDown == true)
					{
						if (nx == RIGHT)
							ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
						else
							ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
					}
					else if (isHolding == true)
					{
						if (nx == RIGHT)
						{
							if (vx == 0)
							{
								ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
							}
							else
								ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
							if (OnGround == false)
								ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
						}
						else
						{
							if (vx == 0)
							{
								ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
							}
							else
								ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
							if (OnGround == false)
								ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
						}
					}
				}
				else
				{
					// chưa nhảy // đang rớt xuống
					if (vy >= 0)
					{
						// đang chạm đất
						if (OnGround == true)
						{
							// đứng yên
							if (level_of_walking == 0 && level_of_running == 0 || vx == 0)
							{
								if (ChangeDirection == false)
								{
									// phải 
									if (nx == RIGHT)
									{
										ani = MARIO_ANI_TAIL_IDLE_RIGHT;
										if (isHolding == true)
											ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
									}
									// trái
									else
									{
										ani = MARIO_ANI_TAIL_IDLE_LEFT;
										if (isHolding == true)
											ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
									}
								}
								else
								{
									if (nx == LEFT)
										ani = MARIO_ANI_TAIL_STOP_LEFT;
									else
										ani = MARIO_ANI_TAIL_STOP_RIGHT;
								}


							}
							// đi bộ qua phải
							else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
							{
								if (ChangeDirection == false)
								{
									ani = MARIO_ANI_TAIL_WALKING_RIGHT;
									if (isHolding == true)
										ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
									else if (isMaxRunning == true)
										ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
								}
								else if (ChangeDirection == true)
									ani = MARIO_ANI_TAIL_STOP_RIGHT;

							}
							//  đi bộ qua trái
							else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
							{
								if (ChangeDirection == false)
								{
									ani = MARIO_ANI_TAIL_WALKING_LEFT;
									if (isHolding == true)
										ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
									else if (isMaxRunning == true)
										ani = MARIO_ANI_TAIL_RUNNING_LEFT;
								}
								else if (ChangeDirection == true)
									ani = MARIO_ANI_TAIL_STOP_LEFT;

							}
						}
						// rớt xuống
						else
						{
							if (isHolding == true)
							{
								// phải 
								if (nx == RIGHT)
									ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
								// trái
								else
									ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
							}
							else
							{
								// phải 
								if (nx == RIGHT)
								{
									ani = MARIO_ANI_TAIL_FALLING_RIGHT;
									if (isFlyingLow == true)
										ani = MARIO_ANI_TAIL_FLYING_RIGHT;
								}
								// trái
								else
								{
									ani = MARIO_ANI_TAIL_FALLING_LEFT;
									if (isFlyingLow == true)
										ani = MARIO_ANI_TAIL_FLYING_LEFT;
								}
							}
						}
					}
					// nhảy
					else
					{
						if (isHolding == true)
						{
							// phải 
							if (nx == RIGHT)
								ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
							// trái
							else
								ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
						}
						else
						{
							// phải 
							if (nx == RIGHT)
							{
								ani = MARIO_ANI_TAIL_JUMP_RIGHT;
							}
							// trái
							else
							{
								ani = MARIO_ANI_TAIL_JUMP_LEFT;
							}
						}
					}

					// đá
					if (canKick == true)
					{
						if (nx == RIGHT)
							ani = MARIO_ANI_TAIL_KICK_RIGHT;
						else
							ani = MARIO_ANI_TAIL_KICK_LEFT;
					}
					else if (GoHiddenWorld == true)
						ani = MARIO_ANI_TAIL_ATTACK_2;
					else if (isAttacking == true && endAttack == false)
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
					else if (isSitDown == true)
					{
						if (nx == RIGHT)
							ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
						else
							ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
					}
					else if (isHolding == true)
					{
						if (nx == RIGHT)
						{
							if (vx == 0)
							{
								ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
							}
							else
								ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
							if (OnGround == false)
								ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
						}
						else
						{
							if (vx == 0)
							{
								ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
							}
							else
								ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
							if (OnGround == false)
								ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
						}
					}
				}

			}
			break;

			case MARIO_LEVEL_FIRE:
			{
				if (isSitDown == true)
				{
					if (nx == RIGHT)
						ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
					else
						ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
				}
				else
				{
					// chưa nhảy // đang rớt xuống
					if (vy >= 0)
					{
						if (OnGround == true)
						{
							// đứng yên
							if (level_of_walking == 0 && level_of_running == 0)
							{
								if (ChangeDirection == false)
								{
									// phải 
									if (nx == RIGHT)
									{
										ani = MARIO_ANI_FIRE_IDLE_RIGHT;
										if (isAttacking == true && endAttack == false)
										{
											if (time_attack <= TIME_ATTACK)
											{
												ani = MARIO_ANI_FIRE_ATTACK_RIGHT_1;
											}
											else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
											{
												ani = MARIO_ANI_FIRE_ATTACK_RIGHT_2;
											}
										}
									}

									// trái
									else
									{
										ani = MARIO_ANI_FIRE_IDLE_LEFT;
										if (isAttacking == true && endAttack == false)
										{
											if (time_attack <= TIME_ATTACK)
											{
												ani = MARIO_ANI_FIRE_ATTACK_LEFT_1;
											}
											else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
											{
												ani = MARIO_ANI_FIRE_ATTACK_LEFT_2;
											}
										}
									}

								}
								else
								{
									if (nx == LEFT)
										ani = MARIO_ANI_FIRE_STOP_LEFT;
									else
										ani = MARIO_ANI_FIRE_STOP_RIGHT;
								}

							}
							// đi bộ qua phải
							else if ((level_of_walking > 0 || level_of_running > 0) && nx == RIGHT)
							{
								// ani = MARIO_ANI_FIRE_WALKING_RIGHT;

								if (ChangeDirection == false)
								{
									ani = MARIO_ANI_FIRE_WALKING_RIGHT;

									if (isMaxRunning == true)
										ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
								}
								else if (ChangeDirection == true)
								{
									ani = MARIO_ANI_FIRE_STOP_RIGHT;
								}

								if (isAttacking == true && endAttack == false)
								{
									if (time_attack <= TIME_ATTACK)
									{
										ani = MARIO_ANI_FIRE_ATTACK_RIGHT_1;
									}
									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
									{
										ani = MARIO_ANI_FIRE_ATTACK_RIGHT_2;
									}

								}
							}
							//  đi bộ qua trái
							else if ((level_of_walking > 0 || level_of_running > 0) && nx == LEFT)
							{
								if (ChangeDirection == false)
								{
									ani = MARIO_ANI_FIRE_WALKING_LEFT;

									if (isMaxRunning == true)
										ani = MARIO_ANI_FIRE_RUNNING_LEFT;
								}
								else if (ChangeDirection == true)
								{
									ani = MARIO_ANI_FIRE_STOP_LEFT;
								}

								if (isAttacking == true && endAttack == false)
								{
									if (time_attack <= TIME_ATTACK)
									{
										ani = MARIO_ANI_FIRE_ATTACK_LEFT_1;
									}
									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
									{
										ani = MARIO_ANI_FIRE_ATTACK_LEFT_2;
									}

								}
							}
						}
						else
						{
							// đứng yên
							if (vx == 0)
							{
								// phải 
								if (nx == RIGHT)
									ani = MARIO_ANI_FIRE_JUMP_RIGHT;
								// trái
								else
									ani = MARIO_ANI_FIRE_JUMP_LEFT;
							}
							// đi bộ qua phải
							else if (vx > 0)
								ani = MARIO_ANI_FIRE_JUMP_RIGHT;
							else
								ani = MARIO_ANI_FIRE_JUMP_LEFT;
						}
					}
					// nhảy
					else
					{
						// đứng yên
						if (vx == 0)
						{
							// phải 
							if (nx == RIGHT)
								ani = MARIO_ANI_FIRE_JUMP_RIGHT;
							// trái
							else
								ani = MARIO_ANI_FIRE_JUMP_LEFT;
						}
						// đi bộ qua phải
						else if (vx > 0)
						{
							ani = MARIO_ANI_FIRE_JUMP_RIGHT;
						}
						//  đi bộ qua trái
						else
						{
							ani = MARIO_ANI_FIRE_JUMP_LEFT;
						}
					}
				}
			}
			break;
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
			animation_set->at(ani)->Render(x, y, 255);
		/*if (level == MARIO_LEVEL_TAIL)
		{
			if (nx == RIGHT && ani >= MARIO_ANI_TAIL_ATTACK_2)
				animation_set->at(ani)->Render(x + 7, y, alpha);
			else if (nx == LEFT && ani == MARIO_ANI_TAIL_ATTACK_1)
				animation_set->at(ani)->Render(x - 7, y, alpha);
			else
				animation_set->at(ani)->Render(x, y, alpha);
		}
		else*/


		////RenderBoundingBox();
	}
	//DebugOut(L"RENDER ani = %i\n", ani);
	RenderBoundingBox();
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
			vy = -MARIO_JUMP_SPEED_FAST;
			OnGround = false;
		}
	}
	break;

	case MARIO_STATE_JUMP_LOW:
	{
		if (OnGround == true)
		{
			vy = -MARIO_JUMP_SPEED_SLOW;
			OnGround = false;
		}
	}
	break;

	case MARIO_STATE_IDLE:
	{
		if (level_of_walking == 0 && level_of_running == 0)
		{
			vx = 0;
			ChangeDirection = false;
		}
		else
		{
			if (vx > 0)
			{
				if (level_of_running > 0)
				{
					if (OnGround == true)
						vx -= GIA_TOC;
					else
						vx -= GIA_TOC / 2;
					if (vx < 0.0f)
						vx = 0.0f;
				}
				// đi bộ
				else if (level_of_walking > 0)
				{
					vx -= GIA_TOC;
				}
			}
			else if (vx < 0)
			{
				if (level_of_running > 0)
				{
					if (OnGround == true)
						vx += GIA_TOC;
					else
						vx += GIA_TOC / 2;
					if (vx > 0.0f)
						vx = 0.0f;
				}
				else if (level_of_walking > 0)
				{
					vx += GIA_TOC;
				}
			}
		}


		// đang trong trạng thái bay thì k trừ 
		//if (canFlyX == false && canFlyS == false)
		{
			if (level_of_walking > 0)
				level_of_walking--;
			/*else if (level_of_walking == 0)
				vx = 0;*/

			if (level_of_running > 0)
				level_of_running--;
			/*else if (level_of_running == 0)
				vx = 0;*/
		}
		if (isSitDown == true && level != MARIO_LEVEL_SMALL)
		{
			y = y - 10;
			//y = y - 9;
			isSitDown = false;
		}

		if (isFlyingHigh != true)
		{
			//isMaxRunning = false;
		}

		if (pressA != true)
		{
			isRunning = false;
		}
		isHolding = false;
	}
	break;

	case MARIO_STATE_DIE:
	{
		vx = 0.0f;
		vy = -MARIO_DIE_DEFLECT_SPEED;
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
		if (level_of_walking == 0 && level_of_running == 0)
		{
			vx = 0;
			ChangeDirection = false;
		}
		else
		{
			if (vx > 0)
			{
				if (level_of_running > 0)
				{
					if (OnGround == true)
						vx -= GIA_TOC;
					else
						vx -= GIA_TOC / 2;
					if (vx < 0.0f)
						vx = 0.0f;
				}
				// đi bộ
				else if (level_of_walking > 0)
				{
					vx -= GIA_TOC;
				}
			}
			else if (vx < 0)
			{
				if (level_of_running > 0)
				{
					if (OnGround == true)
						vx += GIA_TOC;
					else
						vx += GIA_TOC / 2;
					if (vx > 0.0f)
						vx = 0.0f;
				}
				else if (level_of_walking > 0)
				{
					vx += GIA_TOC;
				}
			}
		}


		// đang trong trạng thái bay thì k trừ 
		//if (canFlyX == false && canFlyS == false)
		{
			if (level_of_walking > 0)
				level_of_walking--;
			/*else if (level_of_walking == 0)
				vx = 0;*/

			if (level_of_running > 0)
				level_of_running--;
			/*else if (level_of_running == 0)
				vx = 0;*/
		}
		if (isSitDown == true)
		{
			y = y - 10;
			isSitDown = false;
		}

		if (isFlyingHigh != true)
		{
			//isMaxRunning = false;
		}

		if (pressA != true)
		{
			isRunning = false;
		}
	}
	break;

	case MARIO_STATE_HOLDING_RIGHT:
	{
		isHolding = true;
		if (level_of_walking < MAX_LEVEL_OF_WALKING)
			level_of_walking++;

		// đang đi bên trái -> khựng
		if (vx < 0)
		{
			vx += float(4 * GIA_TOC);
			if (level_of_running > 2 && canFlyX == false && canFlyS == false)
				level_of_running -= 2;
		}
		// đang đứng yên / di chuyển sang phải bình thường
		else
		{
			if (OnGround == true)
			{
				if (level_of_running < MAX_LEVEL_OF_RUNNING)
					level_of_running++;
				vx = (level_of_running * GIA_TOC);
				if (abs(vx) > abs(MARIO_RUNNING_MAX_SPEED))
					vx = MARIO_RUNNING_MAX_SPEED;
			}
			else
			{
				vx = level_of_walking * GIA_TOC;
			}
		}
	}
	break;

	case MARIO_STATE_HOLDING_LEFT:
	{
		isHolding = true;
		if (level_of_walking < MAX_LEVEL_OF_WALKING)
			level_of_walking++;

		if (vx > 0)
		{
			vx -= float(4 * GIA_TOC);
			if (level_of_running > 2 && canFlyX == false && canFlyS == false)
				level_of_running -= 2;
		}
		else
		{
			if (OnGround == true)
			{
				if (level_of_running < MAX_LEVEL_OF_RUNNING)
					level_of_running++;
				vx = -(level_of_running * GIA_TOC);
				if (abs(vx) > abs(MARIO_RUNNING_MAX_SPEED))
					vx = -MARIO_RUNNING_MAX_SPEED;
			}
			else
			{
				vx = -(level_of_walking * GIA_TOC);
			}
		}
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
	level_of_walking = level_of_running = 0;
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
		loseControl = true;
		
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
		}break;

		case MARIO_LEVEL_FIRE:
		{
			// y -= static_cast<FLOAT>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
			ChangeLevelTime = GetTickCount64();
			auto effect = new EffectSmoke(this->x, this->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
			_PlayScene->objects.push_back(effect);
			_PlayScene->Stop = true;

		}break;

		}
		/*level--;
		StartUntouchable();*/
		isLevelDown = true;
	}
}

void Mario::UpLevel()
{
	if (level < MARIO_LEVEL_FIRE)
	{
		loseControl = true;
		switch (level) // không xử lý level tail ở đây mà xử lý bên mario tail vì bị lỗi hiện smoke trước mà đuôi vẫn chưa xóa
		{
		case MARIO_LEVEL_BIG:
		{
			ChangeLevelTime = GetTickCount64();
			auto effect = new EffectSmoke(this->x, this->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
			_PlayScene->objects.push_back(effect);
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
}

void Mario::Debug()
{
	switch (state)
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

	}

	/*if (GoHiddenWorld == true)
		DebugOut(L"GoHiddenWorld == true\t");
	else
		DebugOut(L"GoHiddenWorld == false\t");*/


	//DebugOut(L"vx = %f, vy = %f, level_of_walking = %i, level_of_running = %i\n", vx, vy, level_of_walking, level_of_running);
	DebugOut(L" vx = %f, level_of_walking = %i, x = %f\n", vx, level_of_walking, x);
}

void Mario::Unload()
{
	__instance = NULL;
}

void Mario::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
	if (enemy != NULL && enemy->isDie == false)
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
				isFalling = isFlyingLow = isFlyingHigh = false;
				y += dy;
			}
			else if (e->ny < 0) // dưới lên trên
			{
				x += min_tx * dx + nx * 0.4f;
				y += dy;
				isFalling = isFlyingLow = isFlyingHigh = false;
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
				isFalling = isFlyingLow = isFlyingHigh = false;
			}
		}

		if (e->ny < 0) // va chạm từ trên xuống
		{
			switch (enemy->EnemyType)
			{
			case ENEMY_TYPE_GOOMBA:
			{
				this->vy = -MARIO_JUMP_DEFLECT_SPEED;
				enemy->SetState(ENEMY_STATE_DIE_IS_JUMPED);
				this->nScore++;
				_HUD->UpdateScore(enemy, nScore);
			}
			break;

			//case ENEMY_TYPE_KOOPAS:
			//{
			//	this->vy = -MARIO_JUMP_DEFLECT_SPEED;
			//	this->y -= 1;
			//	Koopas* koopas = dynamic_cast<Koopas*>(enemy);
			//	if (koopas->Health > 1)
			//	{
			//		koopas->Health--;
			//		if (koopas->ObjType == OBJECT_TYPE_REDFLYKOOPAS)
			//			koopas->SetState(ENEMY_STATE_WALKING_LEFT);
			//		this->nScore++;
			//		_HUD->UpdateScore(e->obj, nScore);
			//	}
			//	else
			//	{
			//		if (koopas->vx != 0 && koopas->isShell_2 == false) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
			//		{
			//			koopas->SetState(KOOPAS_STATE_SHELL);
			//			koopas->ReviveTime = GetTickCount64();
			//			this->nScore++;
			//			_HUD->UpdateScore(koopas, nScore);
			//		}
			//		else if (koopas->vx != 0 && koopas->isShell_2 == true) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
			//		{
			//			koopas->SetState(KOOPAS_STATE_SHELL_2);
			//			koopas->vy = 0;
			//			koopas->ReviveTime = GetTickCount64();
			//			this->nScore++;
			//			_HUD->UpdateScore(koopas, nScore);
			//		}
			//		else if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2 || koopas->GetState() == KOOPAS_STATE_SHELL_HOLD)
			//		{
			//			if (this->x <= koopas->x)
			//				koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
			//			else
			//				koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			//			koopas->isKicked = true;
			//			this->nScore++;
			//			_HUD->UpdateScore(koopas, nScore);
			//		}
			//	}
			//}
			//break;

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
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
						else
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
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
				BoomerangEnemy* boom = dynamic_cast<BoomerangEnemy*>(enemy);
				boom->SetState(BOOMERANGENEMY_STATE_DIE);
				this->nScore++;
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
				if (untouchable == false)
					DownLevel();
			}
			break;

			//case ENEMY_TYPE_KOOPAS:
			//{
			//	Koopas* koopas = dynamic_cast<Koopas*>(enemy);
			//	if (koopas->ObjType == OBJECT_TYPE_REDFLYKOOPAS)
			//	{
			//		if (koopas->isHold == false)
			//		{
			//			if (koopas->isShell == true || koopas->isShell_2 == true)
			//			{
			//				if (koopas->OnGroud == false)
			//				{
			//					this->canKick = koopas->isKicked = true;
			//					if (this->nx == RIGHT)
			//						koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
			//					else if (this->nx == LEFT)
			//						koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			//					koopas->vy -= 0.05f;
			//				}
			//				else
			//				{
			//					if (untouchable == false)
			//						DownLevel();
			//				}
			//			}
			//			else
			//			{
			//				if (untouchable == false)
			//					DownLevel();
			//			}
			//			/*if (koopas->Health == 1)
			//			{
			//				if (koopas->OnGroud == true)
			//				{
			//					if (untouchable == false)
			//						DownLevel();
			//				}
			//				else
			//				{
			//					this->canKick = koopas->isKicked = true;
			//					if (this->nx == RIGHT)
			//						koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
			//					else if (this->nx == LEFT)
			//						koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			//					koopas->vy -= 0.05f;
			//				}
			//			}
			//			else
			//			{
			//				if (untouchable == false)
			//					DownLevel();
			//			}*/
			//
			//		}
			//		else if ((koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2) && koopas->vy >= 0) // tránh trường hợp mai rùa đang trên trời vẫn đá được
			//		{
			//			if (e->nx == LEFT)
			//			{
			//				if (pressA == true)
			//				{
			//					isHolding = true;
			//					koopas->nx = 1;
			//					koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
			//				}
			//				else if (koopas->vy >= 0)
			//				{
			//					this->canKick = koopas->isKicked = true;
			//					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
			//				}
			//			}
			//			else if (e->nx == RIGHT)
			//			{
			//				if (pressA == true)
			//				{
			//					isHolding = true;
			//					koopas->nx = -1;
			//					koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
			//				}
			//				else if (koopas->vy >= 0)
			//				{
			//					this->canKick = koopas->isKicked = true;
			//					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			//				}
			//			}
			//		}
			//	}
			//	else
			//	{
			//		if (abs(koopas->vx) > 0 && koopas->isHold == false)
			//		{
			//			if (koopas->isShell == true || koopas->isShell_2 == true)
			//			{
			//				if (koopas->OnGroud == false)
			//				{
			//					this->canKick = koopas->isKicked = true;
			//					if (this->nx == RIGHT)
			//						koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
			//					else if (this->nx == LEFT)
			//						koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			//					koopas->vy -= 0.05f;
			//				}
			//				else
			//				{
			//					if (untouchable == false)
			//						DownLevel();
			//				}
			//			}
			//			else
			//			{
			//				if (untouchable == false)
			//					DownLevel();
			//			}
			//		}
			//		else if ((koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2) && koopas->vy >= 0) // tránh trường hợp mai rùa đang trên trời vẫn đá được
			//		{
			//			if (e->nx == LEFT)
			//			{
			//				if (pressA == true)
			//				{
			//					isHolding = true;
			//					koopas->nx = 1;
			//					koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
			//				}
			//				else if (koopas->vy >= 0)
			//				{
			//					this->canKick = koopas->isKicked = true;
			//					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
			//				}
			//			}
			//			else if (e->nx == RIGHT)
			//			{
			//				if (pressA == true)
			//				{
			//					isHolding = true;
			//					koopas->nx = -1;
			//					koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
			//				}
			//				else if (koopas->vy >= 0)
			//				{
			//					this->canKick = koopas->isKicked = true;
			//					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			//				}
			//			}
			//		}
			//	}
			//}
			//break;

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
					if (koopas->vx != 0 && koopas->isHold == false)
					{
						if (untouchable == false)
							DownLevel();
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
						koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
					}
					else
					{
						this->canKick = koopas->isKicked = true;
						if (this->nx == RIGHT)
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
						else if (this->nx == LEFT)
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						koopas->vy -= 0.05f;
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
	}
}

void Mario::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	this->nScore = 0;

	if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
	{
		if (ny != 0) vy = 0;
		if (e->nx != 0)
		{
			x += dx;
		}
		else if (e->ny < 0)
		{
			if (OnGround == false)
				y += min_ty * dy + ny * 0.1f - 0.3f;
			OnGround = true; // xử lý chạm đất
			isFalling = isFlyingLow = isFlyingHigh = false;
			x += min_tx * dx + nx * 0.4f;
		}
		else if (e->ny > 0)
		{
			if (isFlyingHigh == true)
				y += dy;
		}
	}
	// đi theo kiểu lên đồi ???????? -> y -= dx nx ....
	else
	{
		if (e->obj->ObjType == OBJECT_TYPE_QUESTIONBRICK || e->obj->ObjType == OBJECT_TYPE_ITEMBRICK)
		{
			if (ny != 0) vy = 0;
			if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
				
				// mario nhảy từ dưới lên va chạm gạch 
				if (e->ny > 0)
				{
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
							_PlayScene->objects.push_back(questionbrickitem);
						}
						else
						{
							QuestionBrickItem* questionbrickitem = new QuestionBrickItem(brick->Item, brick->x + 1, brick->y - 3);
							_PlayScene->objects.push_back(questionbrickitem);
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
					if (nx != 0) vx = 0;
					
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
							_PlayScene->objects.push_back(brickitem);
							auto effect = new EffectSmoke(brick->x, brick->y - 16);
							_PlayScene->objects.push_back(effect);
						}
						break;

						case MUSHROOM:
						{
							BrickItem* brickitem = new BrickItem(MUSHROOM, brick->x, brick->y - 3);
							_PlayScene->objects.push_back(brickitem);
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
							_PlayScene->objects.push_back(brickitem);
							brick->CountMoney--;
						}

						}
					}
					vy = 0;
				}
				else if (e->ny < 0) // mario đi trên gạch "?"
				{
					OnGround = true; // xử lý chạm đất
					isFalling = isFlyingLow = isFlyingHigh = false;

					x += min_tx * dx + nx * 0.4f;
				}
				else if (e->nx != 0)
				{
					//if (nx != 0) vx = 0;
					if (ny != 0)vy = 0;
					y += min_ty * dy + ny * 0.4f;
					if (level_of_walking > 10)
						level_of_walking = 10;
				}
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_WARPPIPE)
		{
			if (e->nx != 0)
			{
				isRunning = false;
				if (level_of_running > 0 && canFlyS == false && canFlyX == false)
					level_of_running--;

				if (level_of_walking > 10)
					level_of_walking -= 10;
			}
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
		}
		else if (e->obj->ObjType == OBJECT_TYPE_LISTBRICK)
		{
			if (ny != 0) vy = 0;
			int vitrilist2 = 0;
			ListBrick* listbrick = dynamic_cast<ListBrick*>(e->obj);
			// mario nhảy từ dưới lên va chạm gạch 
			if (e->ny > 0)
			{
				vy = 0;
				if (listbrick->Bricks.size() == 1)
				{
					listbrick->DeleteBrick(0);
				}
				else if (listbrick->Bricks.size() == 2)
				{
					float l, t, r, b;
					int vitri = 0;
					GetBoundingBox(l, t, r, b);
					if (l < listbrick->Bricks.at(0)->x) // sure đụng viên đầu tiên
					{
						vitri = 0;
						listbrick->DeleteBrick(vitri);
						//listbrick->Bricks.erase(listbrick->Bricks.begin());
					}
					else if (r > listbrick->Bricks.at(1)->x + 16) // sure đụng viên cuối
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
						int vitri = (r - listbrick->Bricks.at(0)->x) / 16;
						//if (vitri > listbrick->Bricks.size()) // TH đặc biệt : 4 viên, 64/16 = 4 = vitri trong khi vitri = 3
						//	vitri--;
						int tempx = listbrick->Bricks.at(vitri)->x;
						if (l < listbrick->Bricks.at(vitri)->x && tempx - l > 8)
							vitri--;
						if (listbrick->Bricks.at(vitri)->Item == NORMAL)
						{
							/*ListBrick* listbrick1 = new ListBrick(vitri, listbrick->Bricks.at(0)->x, listbrick->Bricks.at(0)->y);
							ListBrick* listbrick2 = new ListBrick(listbrick->Bricks.size() - 1 - vitri, listbrick->Bricks.at(vitri + 1)->x, listbrick->Bricks.at(vitri + 1)->y);
							*/
							ListBrick* listbrick1 = new ListBrick(listbrick->Bricks, 0, vitri - 1);
							ListBrick* listbrick2 = new ListBrick(listbrick->Bricks, vitri + 1, listbrick->Bricks.size() - 1);
							
							listbrick->canDelete = true;
							_PlayScene->objects.push_back(listbrick1);
							_PlayScene->objects.push_back(listbrick2);
						}
						else
						{

						}
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
			}
			else if (e->nx != 0)
			{
				//if (nx != 0) vx = 0;
				if (ny != 0)vy = 0;
				y += min_ty * dy + ny * 0.4f;
				if (level_of_walking > 10)
					level_of_walking = 10;
			}
		}
	}
	//else if (e->obj->ObjType == OBJECT_TYPE_QUESTIONBRICK || e->obj->ObjType == OBJECT_TYPE_ITEMBRICK)
	//{
	//	if (ny != 0) vy = 0;
	//	if (dynamic_cast<QuestionBrick*>(e->obj))
	//	{
	//		// mario nhảy từ dưới lên va chạm gạch 
	//		if (e->ny > 0)
	//		{
	//			// nếu state normal thì xử lý va chạm, nếu không thì k xử lý
	//			if (e->obj->GetState() == BRICK_STATE_NORMAL)
	//				e->obj->SetState(BRICK_STATE_COLLISION);
	//		}
	//		else if (e->ny < 0) // mario đi trên gạch "?"
	//		{
	//			x += dx;
	//		}
	//		else if (e->nx != 0)
	//		{
	//			y += min_ty * dy + ny * 0.4f;
	//		}
	//	}
	//	else if (dynamic_cast<ItemBrick*>(e->obj))
	//	{
	//		ItemBrick* brick = dynamic_cast<ItemBrick*>(e->obj);
	//		// mario nhảy từ dưới lên va chạm gạch 
	//		if (e->ny > 0)
	//		{
	//			// nếu state normal thì xử lý va chạm, nếu không thì k xử lý
	//			// cả 2 đều làm cho mario k nhảy đươc tiếp + rớt xuống
	//			if (e->obj->GetState() == BRICK_STATE_NORMAL && brick->Item == MUSHROOM)
	//				e->obj->SetState(BRICK_STATE_COLLISION);
	//		}
	//		else if (e->ny < 0) // mario đi trên gạch "?"
	//		{
	//			x += min_tx * dx + nx * 0.4f;
	//		}
	//		else if (e->nx != 0)
	//		{
	//			y -= min_ty * dy + ny * 0.4f;
	//		}
	//	}
	//} 
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
			if (questionbrickitem->Item >= this->level)
				UpLevel();
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
			_PlayScene->objects.push_back(noti);
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