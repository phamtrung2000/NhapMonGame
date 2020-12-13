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
	isDie = false;
	level = MARIO_LEVEL_TAIL;
	SetState(MARIO_STATE_IDLE);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	GoHiddenWorld = untouchable = ChangeDirection = isRunning = isMaxRunning = isFlyingHigh = canFlyX = canFlyS  = isFalling = isSitDown = isAttacking = endAttack = false;
	OnGround = true;
	level_of_walking = level_of_running = 0;
	level_of_stopping = 1;
	time_attack = time_fly = 0;
	ani = 0;
	NumberBullet = 2;
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		Width = MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
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
		/*right = x + MARIO_TAIL_BBOX_WIDTH;
		Width = MARIO_TAIL_BBOX_WIDTH;
		if(isHolding==true)
			right = x + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
		if (isSitDown == true && GoHiddenWorld == false)
		{
			bottom = y + MARIO_TAIL_BBOX_SITDOWN_HEIGHT;
		}*/
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
	}
	break;

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

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
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
			level_of_running = 10;
			vy = 0;
			SetState(MARIO_STATE_IDLE);
		}
	}
	else if (canFlyS == true)
	{
		time_fly++;
		if (time_fly > TIME_FLY)
		{
			canFlyS = isFlyingHigh = false;
			level_of_running = 10;
			vy = 0;
			SetState(MARIO_STATE_IDLE);
		}
	}

	if (GoHiddenWorld == false ) // để mario rớt từ từ khi đi vào map ẩn
	{
		if (isFlyingLow == false) // rớt bình thường
			vy += MARIO_GRAVITY * dt;
		else // rớt chậm khi bay
			vy += MARIO_GRAVITY_FLYING * dt;
	}
	
	if (isFalling == false)
	{
		// mario rớt xuống sau khi nhảy
		if (vy >= 0 && OnGround == false)
			isFalling = true;
	}
	else
	{
	/*	if (vy <= -0.23)
			vy = 0;*/
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

	if (isHolding == true && pressA == false) // đang giữ rùa, thả nút a thì chuyển sang đá
		canKick = true;

	if(canKick==true) // reset lại canKick và hiện ani đá khi canKick=true
	{
		time_attack++;
		if (time_attack >= 2 * TIME_ATTACK)
		{
			canKick = false;
			time_attack = 0;
		}
	}

	if (GetState() == MARIO_STATE_WALKING_RIGHT)
	{
		// đi bộ
		if (isRunning == false)
		{
			if (level_of_running > 0) // sau khi thả nút A thì trừ level_of_running, tránh trường hợp đợi đến state IDLE mới trừ
				level_of_running--;
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
					if(level_of_walking >= 5)
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
				vx += float(5 * GIA_TOC);

				if (level_of_running > 1)
					level_of_running--;

				if (vx >= 0)
					ChangeDirection = false;
			}
			// đang đứng yên / di chuyển sang phải bình thường
			else
			{
				ChangeDirection = false;
				vx = level_of_running * GIA_TOC;
				if (vx >= abs(MARIO_RUNNING_MAX_SPEED))
					vx = MARIO_RUNNING_MAX_SPEED;
				if (level_of_running < MAX_LEVEL_OF_RUNNING)
					level_of_running++;
				else
					isMaxRunning = true;
			}
		}
		if (isSitDown == true)
		{
			y = y - 10;
			isSitDown = false;
		}
	}
	else if (GetState() == MARIO_STATE_WALKING_LEFT)
	{
		if (isRunning == false)
		{
			if (level_of_running > 0) // sau khi thả nút A thì trừ level_of_running, tránh trường hợp đợi đến state IDLE mới trừ
				level_of_running--;
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
					if (level_of_walking >= 5)
						vx = -(level_of_walking * GIA_TOC);\
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
				vx -= float(5 * GIA_TOC);

				if (level_of_running > 1)
					level_of_running--;

				if (vx <= 0)
					ChangeDirection = false;
			}
			else
			{
				ChangeDirection = false;

				vx = -(level_of_running * GIA_TOC);
				if (vx >= abs(MARIO_RUNNING_MAX_SPEED))
					vx = -MARIO_RUNNING_MAX_SPEED;
				if (level_of_running < MAX_LEVEL_OF_RUNNING)
					level_of_running++;
				else
					isMaxRunning = true;
			}
		}
		if (isSitDown == true)
		{
			y = y - 10;
			isSitDown = false;
		}
	}

	if (isMaxRunning == true && abs(vx) > MARIO_RUNNING_MAX_SPEED)
	{
		if (nx == RIGHT)
		{
			vx = MARIO_RUNNING_MAX_SPEED;
			if (OnGround == false)
			{
				vx = MARIO_RUNNING_MAX_SPEED ;
				//vy += 0.0003 * dt;
			}
				
		}
			
		else
			vx = -MARIO_RUNNING_MAX_SPEED;
	}

	if (level_of_running == MAX_LEVEL_OF_RUNNING) // đạt trạng thái max running
		isMaxRunning = true;
	else
		isMaxRunning = false;

	switch (level)
	{
	case MARIO_LEVEL_SMALL:
	{
		Height = MARIO_SMALL_BBOX_HEIGHT;
		canFlyS = canFlyX = isFlyingHigh = isFlyingLow = false;
	}break;
	case MARIO_LEVEL_BIG: case MARIO_LEVEL_FIRE:
	{
		Height = MARIO_BIG_BBOX_HEIGHT;
		canFlyS = canFlyX = isFlyingHigh = isFlyingLow = false;
	}break;
	case MARIO_LEVEL_TAIL:
	{
		Height = MARIO_TAIL_BBOX_HEIGHT;
	}break;
	}

	if (vy < 0 && y < 0)
	{
		y = 0;
		//vy = 0;
	}
		
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
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
	
		//if (ny != 0 && untouchable == false) vy = 0;
		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			
			// xử lý chạm đất
			if (e->ny < 0)
			{
				OnGround = true;
				isFalling = false;
				isFlyingLow = isFlyingHigh = false;
			}
	
			if (dynamic_cast<Ground*>(e->obj))
			{
				if (ny != 0 && untouchable == false) vy = 0;
				Ground* ground = dynamic_cast<Ground*>(e->obj);
				if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
					//if (GetState() != MARIO_STATE_DIE) // nếu k có đk này thì khi chết sẽ k bị nhảy lên vì gán trực tiếp vy = 0
					//	y += min_ty * dy + ny * 0.4f;
				}
				else if (e->nx != 0)
				{
					vx = 0;
					y += min_ty * dy + ny * 0.4f;
				}
					
				
			}
			else if (dynamic_cast<WarpPipe*>(e->obj))
			{
				WarpPipe* pipe = dynamic_cast<WarpPipe*>(e->obj);
				
				if (nx != 0) vx = 0;
				if (e->ny != 0)
				{
					x += min_tx * dx + nx * 0.4f;
					if (pipe->HiddenWorld == true)
					{
						if (e->ny < 0)
						{
							if (isSitDown == true)
							{
								GoHiddenWorld = true;
								vy = 0.03;
							}
							else
								if (ny != 0) vy = 0;
						}
						else if (e->ny > 0)
						{
							GoHiddenWorld = true;
							vy = -0.03f;
							y += dy;
						}
						
					}
					else
						if (ny != 0) vy = 0;
				}
				else if (e->nx != 0)
				{
					y += min_ty * dy + ny * 0.4f;
				}
			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				if (ny != 0) vy = 0;
				x += dx;
				if (isFlyingHigh == true)
					y += dy;
			}
			else if (dynamic_cast<Brick*>(e->obj))
			{
				if (ny != 0) vy = 0;
				Brick* brick = dynamic_cast<Brick*>(e->obj);
				if (brick->Type == CLOUD) // mây
				{
					if (isFlyingHigh == true) // đang bay thì xuyên qua
					{
						x += dx;
						y += dy;
					}
					else // bình thường
					{
						/*x += min_tx * dx + nx * 0.4f;
						y += min_ty * dy + ny * 0.4f;*/
						x += dx;
					}
				}
				else // gạch
				{
					if (e->nx != 0)
					{
						level_of_running = level_of_walking = 0;
						y += min_ty * dy + ny * 0.4f;
					}
				}
			}
			else if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				if (ny != 0) vy = 0;
				// mario nhảy từ dưới lên va chạm gạch 
				if (e->ny > 0)
				{
					// nếu state normal thì xử lý va chạm, nếu không thì k xử lý
					if (e->obj->GetState() == BRICK_STATE_NORMAL)
						e->obj->SetState(BRICK_STATE_COLLISION);
				}
				else if (e->ny < 0) // mario đi trên gạch "?"
				{
					x += dx;
				}
				else if (e->nx != 0)
				{
					if (GetState() == MARIO_STATE_ATTACK)
					{
						if (e->obj->GetState() == BRICK_STATE_NORMAL)
							e->obj->SetState(BRICK_STATE_COLLISION);
					}
					else
					{
						level_of_running = level_of_walking = 0; // không hiện vụ giảm vận tốc khi thả nút di chuyện
						vx = 0.f;
						y += min_ty * dy + ny * 0.4f;
					}
				}
			}
			else if (dynamic_cast<ItemBrick*>(e->obj))
			{
				ItemBrick* brick = dynamic_cast<ItemBrick*>(e->obj);
				if (ny != 0) vy = 0;
				if (nx != 0) vx = 0;
				// mario nhảy từ dưới lên va chạm gạch 
				if (e->ny > 0)
				{
					// nếu state normal thì xử lý va chạm, nếu không thì k xử lý
					// cả 2 đều làm cho mario k nhảy đươc tiếp + rớt xuống
					if (e->obj->GetState() == BRICK_STATE_NORMAL && brick->Item == MUSHROOM)
						e->obj->SetState(BRICK_STATE_COLLISION);
				}
				else if (e->ny < 0) // mario đi trên gạch "?"
				{
					x += min_tx * dx + nx * 0.4f;
				}
				else if (e->nx != 0)
				{
					y += min_ty * dy + ny * 0.4f;
				}
			}
			else if (dynamic_cast<QuestionBrickItem*>(e->obj))
			{
				QuestionBrickItem* questionbrickitem = dynamic_cast<QuestionBrickItem*>(e->obj);
				////va chạm nấm khi đi hoặc nấm rớt từ trên xuống
				//if (e->nx != 0 || e->ny > 0)
				//{
				//	//if (questionbrickitem->GetState() == QUESTIONBRICKITEM_STATE_MOVE)
				//	if (questionbrickitem->vx != 0)
				//	{
				//		questionbrickitem->isDie = true;
				//		switch (level)
				//		{
				//		case MARIO_LEVEL_SMALL:
				//		{
				//			level++;
				//			y -= static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) + 2;
				//		}
				//		break;
				//
				//		case MARIO_LEVEL_BIG:
				//		{
				//			level++;
				//			y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
				//		}
				//		break;
				//
				//		case MARIO_LEVEL_TAIL:
				//		{
				//			level++;
				//			y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
				//		}
				//		break;
				//
				//		default:
				//		{
				//			y -= 2;
				//			break;
				//		}
				//		}
				//	}
				//}
				questionbrickitem->isDie = true;
				switch (level)
				{
				case MARIO_LEVEL_SMALL:
				{
					level++;
					y -= static_cast<__int64>(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) + 2;
				}
				break;
				case MARIO_LEVEL_BIG:
				{
					level++;
					y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
				}
				break;
				/*case MARIO_LEVEL_TAIL:
				{
					level++;
					y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
				}*/
				break;
				default:
				{
					y -= 2;
					break;
				}
				}
			}
			else if (dynamic_cast<BrickItem*>(e->obj))
			{
				BrickItem* brickitem = dynamic_cast<BrickItem*>(e->obj);
					brickitem->isDie = true;
			}
			else if (dynamic_cast<FireBullet*>(e->obj))
			{
			FireBullet* firebullet = dynamic_cast<FireBullet*>(e->obj);
			// jump on top >> kill firebullet and deflect a bit 
			//if ( firebullet->FireMario == false && (e->nx != 0 || e->ny != 0))
			if (firebullet->FireMario == false)
			{
				if (untouchable == false)
				{
					switch (level)
					{
					case MARIO_LEVEL_FIRE:
					{
						y = static_cast<float>((int)(y - abs(MARIO_BIG_BBOX_HEIGHT - MARIO_TAIL_BBOX_HEIGHT) * 2));
						level--;
						StartUntouchable();

					}
					break;

					case MARIO_LEVEL_TAIL:
					{
						level--;
						StartUntouchable();
					}
					break;
					case MARIO_LEVEL_BIG:
					{
						y = (int)(y + abs(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT - 1));
						level--;
						StartUntouchable();
					}
					break;
					case MARIO_LEVEL_SMALL:
					{
						SetState(MARIO_STATE_DIE);
					}
					break;
					}
				}
				else
				{
					if (level == MARIO_LEVEL_SMALL)
						y -= 1;
				}
			}
			else
			{
			}
			}
			else if (dynamic_cast<Coin*>(e->obj))
			{
				Coin* coin = dynamic_cast<Coin*>(e->obj);
				coin->isDie = true;
				_HUD->Money++;
				_HUD->Score += 50;
				x += dx;
				y += dy;
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				if(p->GetSceneId() != 1)
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
				else
					CGame::GetInstance()->SwitchScene2(p->GetSceneId());
			}

			else if (dynamic_cast<FirePiranhaPlant*>(e->obj))
			{
				y += min_ty * dy + ny * 0.4f;
				FirePiranhaPlant* plant = dynamic_cast<FirePiranhaPlant*>(e->obj);
				if (e->ny < 0)
				{
					if (INT(plant->Startposy - plant->y) <= (plant->WarpPipeHeight - FIREPIRANHAPLANT_BBOX_HEIGHT))
					{
						// cho cây thụt xuống ngang với miệng cống
						plant->y += 1;
						plant->Stop = true;
						continue;
					}
				}
				if (untouchable == false)
					DownLevel();
			}
			else if (dynamic_cast<GreenFirePlant*>(e->obj))
			{
				y += min_ty * dy + ny * 0.4f;
				GreenFirePlant* plant = dynamic_cast<GreenFirePlant*>(e->obj);
				if (e->ny < 0)
				{
					if (INT(plant->Startposy - plant->y) <= (plant->WarpPipeHeight - GREENFIREPLANT_BBOX_HEIGHT))
					{
						// cho cây thụt xuống ngang với miệng cống
						plant->y += 1;
						plant->Stop = true;
						continue;
					}
				}
				if (untouchable == false)
					DownLevel();
			}
			else if (dynamic_cast<GreenPlant*>(e->obj))
			{
				y += min_ty * dy + ny * 0.4f;
				GreenPlant* plant = dynamic_cast<GreenPlant*>(e->obj);
				if (e->ny < 0)
				{
					if (INT(plant->Startposy - plant->y) <= (plant->WarpPipeHeight - GREENPLANT_BBOX_HEIGHT))
					{
						// cho cây thụt xuống ngang với miệng cống
						plant->y += 1;
						plant->isBlocked = true;
						continue;
					}
				}
				if (untouchable == false)
					DownLevel();
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->ny < 0)
				{
					if (goomba->GetState() == GOOMBA_STATE_WALKING)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == false)
					{
						if (goomba->GetState() == GOOMBA_STATE_WALKING)
						{
							if (level == MARIO_LEVEL_TAIL && isAttacking == true)
								goomba->SetState(GOOMBA_STATE_DIE_2);
							else
								DownLevel();
						}
					}
				}
			} 
			else if (dynamic_cast<Koopas*>(e->obj)) // if e->obj is Goomba 
			{
				Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
				if (e->ny < 0) // nhảy lên đầu rùa
				{
					//if (koopas->GetState() == KOOPAS_STATE_WALKING_RIGHT || koopas->GetState() == KOOPAS_STATE_WALKING_LEFT) // rùa đang di chuyển
					if (koopas->vx != 0  && koopas->isShell_2 == false) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
					{
						koopas->SetState(KOOPAS_STATE_SHELL);
						koopas->ReviveTime = GetTickCount64();
						
					}
					else if (koopas->vx != 0 && koopas->isShell_2 == true) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
					{
						koopas->SetState(KOOPAS_STATE_SHELL_2);
						koopas->ReviveTime = GetTickCount64();
						
					}
					else if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2 || koopas->GetState() == KOOPAS_STATE_SHELL_HOLD)
					{
						if (this->x <= koopas->x)
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
						else
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
					}
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				else 
				{
					if (untouchable == false)
					{
						if (e->ny > 0) 	// mario nhảy lên chạm chân rùa
						{
							//if (koopas->GetState()== KOOPAS_STATE_WALKING_RIGHT || koopas->GetState()==KOOPAS_STATE_WALKING_LEFT)
							if (koopas->vx != 0)
								DownLevel();
						}
						else if (e->nx != 0) // va chạm rùa
						{
							if (koopas->vx != 0)
							{
								if (level == MARIO_LEVEL_TAIL && isAttacking == true)
								{
									koopas->SetState(KOOPAS_STATE_SHELL_2);
									koopas->ReviveTime = GetTickCount();
								}
								else
									DownLevel();
							}
							else if ((koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_2) && koopas->vy >= 0) // tránh trường hợp mai rùa đang trên trời vẫn đá được
							{
								if (e->nx < 0)
								{
									if (pressA == true)
									{
										isHolding = true;
										koopas->nx = 1;
										koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
									}
									else if (koopas->vy >= 0)
									{
										canKick = true;
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
										canKick = true;
										koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
									}
								}
							}
						}
					}
					else
					{
						y += min_ty * dy + ny * 0.4f;
						//y += dy;
						x += min_tx * dx + nx * 0.4f;
					}
				}
			}
			else if (dynamic_cast<GreenKoopas*>(e->obj)) // if e->obj is Goomba 
			{
				GreenKoopas* greenkoopas = dynamic_cast<GreenKoopas*>(e->obj);
				if (e->ny < 0) // nhảy lên đầu rùa
				{
				//if (greenkoopas->GetState() == KOOPAS_STATE_WALKING_RIGHT || greenkoopas->GetState() == KOOPAS_STATE_WALKING_LEFT) // rùa đang di chuyển
				if (greenkoopas->vx != 0 && greenkoopas->isShell_2 == false) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
				{
					greenkoopas->SetState(KOOPAS_STATE_SHELL);
					greenkoopas->ReviveTime = GetTickCount();

				}
				else if (greenkoopas->vx != 0 && greenkoopas->isShell_2 == true) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
				{
					greenkoopas->SetState(KOOPAS_STATE_SHELL_2);
					greenkoopas->ReviveTime = GetTickCount();

				}
				else if (greenkoopas->GetState() == KOOPAS_STATE_SHELL || greenkoopas->GetState() == KOOPAS_STATE_SHELL_2 || greenkoopas->GetState() == KOOPAS_STATE_SHELL_HOLD)
				{
					if (this->x <= greenkoopas->x)
						greenkoopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					else
						greenkoopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
				}
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else
			{
				if (untouchable == false)
				{
					if (e->ny > 0) 	// mario nhảy lên chạm chân rùa
					{
						//if (greenkoopas->GetState()== KOOPAS_STATE_WALKING_RIGHT || greenkoopas->GetState()==KOOPAS_STATE_WALKING_LEFT)
						if (greenkoopas->vx != 0)
							DownLevel();
					}
					else if (e->nx != 0) // va chạm rùa
					{
						if (greenkoopas->vx != 0)
						{
							if (level == MARIO_LEVEL_TAIL && isAttacking == true)
							{
								greenkoopas->SetState(KOOPAS_STATE_SHELL_2);
								greenkoopas->ReviveTime = GetTickCount();
							}
							else
								DownLevel();
						}
						else if ((greenkoopas->GetState() == KOOPAS_STATE_SHELL || greenkoopas->GetState() == KOOPAS_STATE_SHELL_2) && greenkoopas->vy >= 0) // tránh trường hợp mai rùa đang trên trời vẫn đá được
						{
							if (e->nx < 0)
							{
								if (pressA == true)
								{
									isHolding = true;
									greenkoopas->nx = 1;
									greenkoopas->SetState(KOOPAS_STATE_SHELL_HOLD);
								}
								else if (greenkoopas->vy >= 0)
								{
									canKick = true;
									greenkoopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
								}
							}
							else if (e->nx == RIGHT)
							{
								if (pressA == true)
								{
									isHolding = true;
									greenkoopas->nx = -1;
									greenkoopas->SetState(KOOPAS_STATE_SHELL_HOLD);
								}
								else if (greenkoopas->vy >= 0)
								{
									canKick = true;
									greenkoopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
								}
							}
						}
					}
				}
				else
				{
					y += min_ty * dy + ny * 0.4f;
					//y += dy;
					x += min_tx * dx + nx * 0.4f;
				}
			}
			}
			else if (dynamic_cast<GreenFlyKoopas*>(e->obj)) // if e->obj is Goomba 
			{
				GreenFlyKoopas* greenflykoopas = dynamic_cast<GreenFlyKoopas*>(e->obj);
				if (e->ny < 0) // nhảy lên đầu rùa
				{
					if (greenflykoopas->Health == 2)
					{
						greenflykoopas->Health--;
						greenflykoopas->vy = 0;
					}
					else
					{
						//if (greenflykoopas->GetState() == KOOPAS_STATE_WALKING_RIGHT || greenflykoopas->GetState() == KOOPAS_STATE_WALKING_LEFT) // rùa đang di chuyển
						if (greenflykoopas->vx != 0 && greenflykoopas->isShell_2 == false) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
						{
							greenflykoopas->SetState(KOOPAS_STATE_SHELL);
							greenflykoopas->ReviveTime = GetTickCount();

						}
						else if (greenflykoopas->vx != 0 && greenflykoopas->isShell_2 == true) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
						{
							greenflykoopas->SetState(KOOPAS_STATE_SHELL_2);
							greenflykoopas->ReviveTime = GetTickCount();

						}
						else if (greenflykoopas->GetState() == KOOPAS_STATE_SHELL || greenflykoopas->GetState() == KOOPAS_STATE_SHELL_2 || greenflykoopas->GetState() == KOOPAS_STATE_SHELL_HOLD)
						{
							if (this->x <= greenflykoopas->x)
								greenflykoopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
							else
								greenflykoopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						}

					}
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				else
				{
					if (untouchable == false)
					{
						if (e->ny > 0) 	// mario nhảy lên chạm chân rùa
						{
							//if (greenflykoopas->GetState()== KOOPAS_STATE_WALKING_RIGHT || greenflykoopas->GetState()==KOOPAS_STATE_WALKING_LEFT)
							if (greenflykoopas->vx != 0)
								DownLevel();
						}
						else if (e->nx != 0) // va chạm rùa
						{
							if (greenflykoopas->vx != 0)
							{
								if (level == MARIO_LEVEL_TAIL && isAttacking == true)
								{
									greenflykoopas->SetState(KOOPAS_STATE_SHELL_2);
									greenflykoopas->ReviveTime = GetTickCount();
								}
								else
									DownLevel();
							}
							else if ((greenflykoopas->GetState() == KOOPAS_STATE_SHELL || greenflykoopas->GetState() == KOOPAS_STATE_SHELL_2) && greenflykoopas->vy >= 0) // tránh trường hợp mai rùa đang trên trời vẫn đá được
							{
								if (e->nx < 0)
								{
									if (pressA == true)
									{
										isHolding = true;
										greenflykoopas->nx = 1;
										greenflykoopas->SetState(KOOPAS_STATE_SHELL_HOLD);
									}
									else if (greenflykoopas->vy >= 0)
									{
										canKick = true;
										greenflykoopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
									}
								}
								else if (e->nx == RIGHT)
								{
									if (pressA == true)
									{
										isHolding = true;
										greenflykoopas->nx = -1;
										greenflykoopas->SetState(KOOPAS_STATE_SHELL_HOLD);
									}
									else if (greenflykoopas->vy >= 0)
									{
										canKick = true;
										greenflykoopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
									}
								}
							}
						}
					}
					else
					{
						y += min_ty * dy + ny * 0.4f;
						//y += dy;
						x += min_tx * dx + nx * 0.4f;
					}
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
		//level_of_running = level_of_walking = 1;
	}
	Debug();
}

void Mario::Render()
{
	ani = 0;
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
				if (vx == 0)
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
						ani = MARIO_ANI_SMALL_STOP_RIGHT;
				}
				// đi bộ qua phải
				else if (vx > 0)
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
				else
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
			// chưa nhảy / đang rớt xuống
			if (vy >= 0)
			{
				// đang chạm đất
				if (OnGround == true)
				{
					// đứng yên
					if (vx == 0)
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
								else if (isSitDown == true)
									ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
							}
							// trái
							else
							{
								ani = MARIO_ANI_BIG_IDLE_LEFT;
								if (isHolding == true)
									ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
								else if (isSitDown == true)
									ani = MARIO_ANI_BIG_SITDOWN_LEFT;
							}
						}
					}
					// đi bộ qua phải
					else if (vx > 0)
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
							if (isHolding != true)
								ani = MARIO_ANI_BIG_STOP_RIGHT;
						}
					}
					//  đi bộ qua trái
					else
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

			// đá
			if (canKick == true)
			{
				if (nx == RIGHT)
					ani = MARIO_ANI_BIG_KICK_RIGHT;
				else
					ani = MARIO_ANI_BIG_KICK_LEFT;
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
						if (vx == 0)
						{
							// phải 
							if (nx == RIGHT)
							{
								ani = MARIO_ANI_TAIL_IDLE_RIGHT;
								if (isHolding == true)
									ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
								else if (isSitDown == true)
									ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
							}
							// trái
							else
							{
								ani = MARIO_ANI_TAIL_IDLE_LEFT;
								if (isHolding == true)
									ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
								else if (isSitDown == true)
									ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
							}
						}
						// đi bộ qua phải
						else if (vx > 0)
						{
							if (ChangeDirection == false)
							{
								ani = MARIO_ANI_TAIL_WALKING_RIGHT;
								if (isHolding == true)
									ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
							}
							else if (ChangeDirection == true)
								ani = MARIO_ANI_TAIL_STOP_RIGHT;
						}
						//  đi bộ qua trái
						else
						{
							if (ChangeDirection == false)
							{
								ani = MARIO_ANI_TAIL_WALKING_LEFT;
								if (isHolding == true)
									ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
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
						if (vx == 0)
						{
							// phải 
							if (nx == RIGHT)
							{
								ani = MARIO_ANI_TAIL_IDLE_RIGHT;
								if (isHolding == true)
									ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
								else if (isSitDown == true)
								{
									ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
								}
							}
							// trái
							else
							{
								ani = MARIO_ANI_TAIL_IDLE_LEFT;
								if (isHolding == true)
									ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
								else if (isSitDown == true)
								{
									ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
								}
							}
						}
						// đi bộ qua phải
						else if (vx > 0)
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
						else
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
								if(isFlyingLow == true)
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
		
		}
		break;

		case MARIO_LEVEL_FIRE:
		{
			// chưa nhảy // đang rớt xuống
			if (vy >= 0)
			{
				if (OnGround == true)
				{
					// đứng yên
					if (vx == 0)
					{
						if (ChangeDirection == false)
						{
							// phải 
							if (nx == RIGHT)
							{
								ani = MARIO_ANI_FIRE_IDLE_RIGHT;
								if (isSitDown == true)
								{
									ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
								}
								else if (isAttacking == true && endAttack == false)
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
								if (isSitDown == true)
								{
									ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
								}
								else if (isAttacking == true && endAttack == false)
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
							ani = MARIO_ANI_FIRE_STOP_RIGHT;
						}

					}
					// đi bộ qua phải
					else if (vx > 0)
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
					else if(vx < 0)
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
		break;
		}
	}
	
	int alpha = 255;
	if (untouchable) alpha = 128;

	if (level == MARIO_LEVEL_TAIL)
	{
		if (nx == RIGHT && ani >= MARIO_ANI_TAIL_ATTACK_2)
			animation_set->at(ani)->Render(x + 7, y, alpha);
		else if (nx == LEFT && ani == MARIO_ANI_TAIL_ATTACK_1)
			animation_set->at(ani)->Render(x - 7, y, alpha);
		else
			animation_set->at(ani)->Render(x, y, alpha);
	}
	else
		animation_set->at(ani)->Render(x, y, alpha);
	//RenderBoundingBox();
	
	//DebugOut(L"ani =%i\n", ani);
}

void Mario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
	{
		nx = 1;
	}
	break;

	case MARIO_STATE_WALKING_LEFT:
	{
		
		nx = -1;
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
		if (vx > 0)
		{
			if (level_of_running > 1)
			{
				vx -= 2 * GIA_TOC;
			}
			// đi bộ
			else if (level_of_walking > 0)
			{
				vx -= GIA_TOC;
			}

			if (vx < 0)
				vx = 0;

		}
		else if (vx < 0)
		{
			if (level_of_running > 1)
			{
				vx += 2 * GIA_TOC;
			}
			else if (level_of_walking > 0)
			{
				vx += GIA_TOC;
			}
			if (vx > 0)
				vx = 0;
		}
	
		// đang trong trạng thái bay thì k trừ 
		if (canFlyX == false)
		{
			if (level_of_walking > 0)
				level_of_walking--;
			else if (level_of_walking == 0)
				vx = 0;
			if (level_of_running > 0)
				level_of_running--;
			else if (level_of_running == 0)
				vx = 0;
		}
		if (isSitDown == true)
		{
			y = y - 10;
			isSitDown = false;
		}
		if (ChangeDirection == true)
		{
			if (nx == LEFT)
			{
				if (level_of_walking == 0)
				{
					ChangeDirection = false;
				}
			}
			else
			{

			}
		}


		if (isFlyingHigh != true)
		{
			//isMaxRunning = false;
		}
		if (pressA != true)
		{
			isHolding = isRunning = false;
		}
		
	}
	break;

	case MARIO_STATE_DIE:
	{
		vx = 0;
		vy = -MARIO_DIE_DEFLECT_SPEED;
	}
	break;

	case MARIO_STATE_FLY_LOW:
	{
		/*if (canFlyX == true)
		{
			if (time_fly <= TIME_FLY)
			{
				if(isMaxRunning==true)
					vy = -MARIO_FLY_SPEED;
				isFlyingHigh = true;
				time_fly++;
			}
			else
			{
				isMaxRunning = false;
				level_of_running = 10;
				if (vy < 0)
				{
					vy = 0;
				}
			}
		}
		else if (canFlyS == true)
		{
			DebugOut(L"time fly = %i\n",time_fly);
			if (time_fly <= TIME_FLY_S)
			{
				time_fly++;
				isFlyingHigh = true;
			}
			else
			{
				isMaxRunning = false;
				level_of_running = 10;
				if (vy < 0)
				{
					vy = 0;
				}
			}
		}*/
		isFlyingLow = true;
		if (canFlyX == true)
		{
			
		}
		else if (canFlyS == true)
		{

		}
	}
	break;

	case MARIO_STATE_FLY_HIGH:
	{
		isFlyingHigh = true;
		vy = -MARIO_FLY_SPEED;
		if (nx == RIGHT)
			vx = MARIO_FLY_MOVING_SPEED;
		else
			vx = -MARIO_FLY_MOVING_SPEED;
		/*if (canFlyX == true)
		{
			if (time_fly <= TIME_FLY)
			{
				vy = -MARIO_FLY_SPEED;
				time_fly++;
			}
			else
			{
				level_of_running = 10;
				if (vy < 0)
				{
					vy = 0;
				}
			}
		}
		else if (canFlyS == true)
		{
			DebugOut(L"time fly = %i\n", time_fly);
			if (time_fly <= TIME_FLY_S)
			{
				if (isMaxRunning == true)
					vy = -MARIO_FLY_SPEED;
				time_fly++;
				isFlyingHigh = true;
			}
			else
			{
				isMaxRunning = false;
				level_of_running = 10;
				if (vy < 0)
				{
					vy = 0;
				}
			}
		}*/
	}
	break;

	case MARIO_STATE_SITDOWN:
	{
		if (isSitDown == false)
		{
			isSitDown = true;
			// sprite ngồi heigh chỉ có 18 còn sprite idle thì heigh = 28 nên phải trừ bớt để k bị lỗi sprite
			// sau khi +10 thì phải -10 lại bên idle để không bị lỗi mario thụt xuống dưới
			if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
				y = y + (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_BBOX_SITDOWN_HEIGHT);
			else
				y = y + (MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_BBOX_SITDOWN_HEIGHT);

			vx = 0;
		}
	}
	break;

	case MARIO_STATE_ATTACK:
	{
		isAttacking = true;
		//time_attack = 0;
	}
	break;
	
	}

	// DebugOut(L"level of running=%i\n",level_of_running);
	
}

void Mario::Reset()
{
	SetSpeed(0, 0);
	isDie = false;
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	GoHiddenWorld = untouchable = ChangeDirection = isRunning = isMaxRunning = isFlyingHigh = canFlyX = canFlyS = isFalling = isSitDown = isAttacking = endAttack = false;
	OnGround = true;
	level_of_walking = level_of_running = 0;
	level_of_stopping = 1;
	time_attack = time_fly = 0;
	ani = 0;
	NumberBullet = 2;
}

void Mario::DownLevel()
{
	switch (level)
	{
		case MARIO_LEVEL_SMALL:
		{
			SetState(MARIO_STATE_DIE);
		}break;

		case MARIO_LEVEL_BIG:
		{
			y += MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		}
		case MARIO_LEVEL_TAIL:
		{
			y -= MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
		}
		case MARIO_LEVEL_FIRE:
		{
			y -= MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
		}
		level--;
		StartUntouchable();
	}
}

void Mario::Debug()
{
	if (level_of_walking >= 0) {
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
		}
		if (isRunning == false)
			DebugOut(L"isRunning == false\t");
		else
			DebugOut(L"isRunning == true\t");
		/*if (canFlyX == false)
			DebugOut(L"canFlyX == false\t");
		else
			DebugOut(L"canFlyX == true\t");*/

		DebugOut(L"level running %i, vx = %f, vy = %f, nx = %i,x = %f\n", level_of_running, vx, vy, nx, x);
	}
	
}