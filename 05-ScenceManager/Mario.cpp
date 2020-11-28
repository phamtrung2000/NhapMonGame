#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

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


Mario::Mario(float x, float y) : CGameObject()
{
	ObjType = OBJECT_TYPE_MARIO;
	isDie = false;
	level = MARIO_LEVEL_SMALL;
	SetState(MARIO_STATE_IDLE);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	untouchable = ChangeDirection = isRunning = isMaxRunning = isFlyingHigh = canFlyX = canFlyS  = isFalling = isSitDown = isAttacking = endAttack = false;
	OnGround = true;
	level_of_walking = level_of_running = 1;
	level_of_stopping = 1;
	time_attack = time_fly = 0;
	ani = 0;
	NumberBullet = 2;
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
	// Simple fall down
	if (isFlyingLow == false)
		vy += MARIO_GRAVITY * dt;
	else
	{
		vy += MARIO_GRAVITY_FLYING * dt;
	}

	if (vy >= 0 && OnGround == false)
		isFalling = true;

	if (isAttacking == true)
	{
		time_attack++;
		if (time_attack >= 5 * TIME_ATTACK)
		{
			time_attack = 0;
			isAttacking = false;
		}
	}

	if (isRunning == false && level_of_running > 1)
	{
		/*if (isflying != true &&)
			level_of_running--;*/
	}

	if (isHolding == true && pressA == false)
		canKick = true;

	if(canKick==true)
	{
		time_attack++;
		if (time_attack >= 2 * TIME_ATTACK)
		{
			canKick = false;
			time_attack = 0;
		}
	}

	if (level_of_running == MAX_LEVEL_OF_RUNNING)
		isMaxRunning = true;
	else
		isMaxRunning = false;

	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		Height = MARIO_SMALL_BBOX_HEIGHT; break;
	case MARIO_LEVEL_BIG: case MARIO_LEVEL_FIRE:
		Height = MARIO_BIG_BBOX_HEIGHT; break;
	case MARIO_LEVEL_TAIL:
		Height = MARIO_TAIL_BBOX_HEIGHT; break;
	}

	if (vy < 0 && y < 0)
	{
		y = 0;
		vy = 0;
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
	
		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		/*if (rdx != 0 && rdx!=dx)
			x += 2*nx*abs(5); */
	
		//if (ny != 0 && untouchable == false) vy = 0;
		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/
	
		//DebugOut(L"size = %i\n", coEventsResult.size());
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//DebugOut(L"type = %i\n", e->obj->ObjType);
			// xử lý tạm vụ chạm đất
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
					if (GetState() != MARIO_STATE_DIE) // nếu k có đk này thì khi chết sẽ k bị nhảy lên vì gán trực tiếp vy = 0
						vy = 0;
				}
				else
					y += min_ty * dy + ny * 0.4f;
				
			}
			else if (dynamic_cast<WarpPipe*>(e->obj))
			{
				if (ny != 0) vy = 0;
				if (nx != 0) vx = 0;
				if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
					vy = 0;
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
						level_of_running = level_of_walking = 1;
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
					// cả 2 đều làm cho mario k nhảy đươc tiếp + rớt xuống
					if (e->obj->GetState() == BRICK_STATE_NORMAL)
						e->obj->SetState(BRICK_STATE_COLLISION);
					//vy = 0;
				}
				else if (e->ny < 0) // mario đi trên gạch "?"
				{
					x += dx;
				}
				else if (e->nx != 0)
				{
					level_of_running = level_of_walking = 1; // không hiện vụ giảm vận tốc khi thả nút di chuyện
					y += min_ty * dy + ny * 0.4f;
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
					if (level == MARIO_LEVEL_TAIL && isAttacking == true)
						brick->isDie = true;
					else
					{
						y += min_ty * dy + ny * 0.4f;
					}
				}
			//	DebugOut(L"state =%i,vx = %f, vy = %f,level walk = %i, level running = %i, time fly = %i\n", state, vx, vy, level_of_walking, level_of_running, time_fly);
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
				case MARIO_LEVEL_TAIL:
				{
					level++;
					y -= static_cast<__int64>(MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) + 2;
				}
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
			else if (dynamic_cast<FirePiranhaPlant*>(e->obj))
			{
				FirePiranhaPlant* plant = dynamic_cast<FirePiranhaPlant*>(e->obj);
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
				else if (e->nx != 0)
				{
					if (untouchable == false)
						DownLevel();
				}
			}
			else if (dynamic_cast<GreenFirePlant*>(e->obj))
			{
				GreenFirePlant* plant = dynamic_cast<GreenFirePlant*>(e->obj);
				if (e->ny < 0)
				{
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
				else if (e->nx != 0)
				{
					if (untouchable == false)
						DownLevel();
				}
			}
			else if (dynamic_cast<GreenPlant*>(e->obj))
			{
				GreenPlant* plant = dynamic_cast<GreenPlant*>(e->obj);
				if (e->ny < 0)
				{
					//plant->vy = 0;
					//DebugOut(L"aaaaaaaaaa %i\n", INT(plant->Startposy - plant->y));
					if (INT(plant->Startposy - plant->y) <= (plant->WarpPipeHeight - GREENPLANT_BBOX_HEIGHT))
					{
						// cho cây thụt xuống ngang với miệng cống
						plant->y += 1;
						//DebugOut(L"1111111111111111111111\n");
						plant->isBlocked = true;
					}
					/*else
					{
						if (untouchable == false)
							DownLevel();
					}*/
				}
				else if (e->nx != 0)
				{
					if (untouchable == false)
						DownLevel();
				}
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
							y = (int)(y - abs(MARIO_BIG_BBOX_HEIGHT - MARIO_TAIL_BBOX_HEIGHT) * 2);
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
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
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
									koopas->ReviveTime = GetTickCount64();
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
					greenkoopas->ReviveTime = GetTickCount64();

				}
				else if (greenkoopas->vx != 0 && greenkoopas->isShell_2 == true) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
				{
					greenkoopas->SetState(KOOPAS_STATE_SHELL_2);
					greenkoopas->ReviveTime = GetTickCount64();

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
								greenkoopas->ReviveTime = GetTickCount64();
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
							greenflykoopas->ReviveTime = GetTickCount64();

						}
						else if (greenflykoopas->vx != 0 && greenflykoopas->isShell_2 == true) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
						{
							greenflykoopas->SetState(KOOPAS_STATE_SHELL_2);
							greenflykoopas->ReviveTime = GetTickCount64();

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
									greenflykoopas->ReviveTime = GetTickCount64();
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
			else if (dynamic_cast<Coin*>(e->obj))
			{
				Coin* coin = dynamic_cast<Coin*>(e->obj);
				x += dx;
				y += dy;
				coin->isDie = true;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
								else if (isAttacking == true && endAttack == false)
								{
									if (time_attack <= TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_1;
									}
									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_2;
									}
									else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_3;
									}
									else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_4;
									}
									else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_1;
									}
								}
							}
							// trái
							else
							{
								ani = MARIO_ANI_TAIL_IDLE_LEFT;
								if (isHolding == true)
									ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
								else if (isSitDown == true)
									ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
								else if (isAttacking == true && endAttack == false)
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

							if (isAttacking == true && endAttack == false)
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

							if (isAttacking == true && endAttack == false)
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
									ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
								else if (isAttacking == true && endAttack == false)
								{
									if (time_attack <= TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_1;
									}
									else if (time_attack > TIME_ATTACK && time_attack <= 2 * TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_2;
									}
									else if (time_attack > 2 * TIME_ATTACK && time_attack <= 3 * TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_3;
									}
									else if (time_attack > 3 * TIME_ATTACK && time_attack <= 4 * TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_4;
									}
									else if (time_attack > 4 * TIME_ATTACK && time_attack <= 5 * TIME_ATTACK)
									{
										ani = MARIO_ANI_TAIL_ATTACK_1;
									}
								}
							}
							// trái
							else
							{
								ani = MARIO_ANI_TAIL_IDLE_LEFT;
								if (isHolding == true)
									ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
								else if (isSitDown == true)
									ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
								else if (isAttacking == true && endAttack == false)
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

							if (isAttacking == true && endAttack == false)
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

							if (isAttacking == true && endAttack == false)
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

	/*if (isHolding == false)
		DebugOut(L"isHolding == false\t");
	else
		DebugOut(L"isHolding == true\t");

	if (pressA == false)
		DebugOut(L"pressA == false\t");
	else
		DebugOut(L"pressA == true\t");

	if (canKick == false)
		DebugOut(L"canKick == false\t");
	else
	{
		DebugOut(L"canKick == true\t");
		DebugOut(L"5.Render	Ani: %i, state=%i,vx=%f\t", ani,state,vx);
	}*/
	
	//if (canFlyS == false)
	//	DebugOut(L"canFlyS == false\t");
	//else
	//	DebugOut(L"canFlyS == true\t");
	//if (canFlyX == false)
	//	DebugOut(L"canFlyX == false\t");
	//else
	//	DebugOut(L"canFlyX == true\t");
	//if (isFlyingHigh == false)
	//	DebugOut(L"isFlyingHigh == false\t");
	//else
	//	DebugOut(L"isFlyingHigh == true\t");
	//if (isMaxRunning == false)
	//	DebugOut(L"isMaxRunning == false\t");
	//else
	//	DebugOut(L"isMaxRunning == true\t");

	//DebugOut(L"5.Render	Ani: %i\n", ani);
	
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
	//DebugOut(L"\n");
}

void Mario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
	{
		// đi bộ
		if (isRunning == false)
		{
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
					vx = MARIO_FLY_MOVING_SPEED;
				}
				else
				{
					ChangeDirection = false;
					vx = level_of_walking * GIA_TOC;
					if (level_of_walking < MAX_LEVEL_OF_WALKING)
						level_of_walking++;
				}
			}
		}
		// chạy
		else
		{
			if (vx < 0 && isHolding!=true)
			{
				ChangeDirection = true;
				vx += float(5 * GIA_TOC);

				if (level_of_running > 1)
					level_of_running--;

				if (vx >= 0)
					ChangeDirection = false;
			}
			else
			{
				ChangeDirection = false;
				vx = level_of_running * GIA_TOC;

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
		nx = 1;
	}
	break;

	case MARIO_STATE_WALKING_LEFT:
	{
		if (isRunning == false)
		{
			// đang đi qua bên phải (vx > 0) thì đột ngột qua trái -> khúc này xử lý vụ khựng lại r chuyển hướng
			if (vx > 0)
			{
				// bắt đầu chuyển hướng di chuyển
				ChangeDirection = true;

				// trừ bớt vx chứ không gán trực tiếp để nó hiện animation khựng
				// vx -= (level_of_walking * GIA_TOC); ->không nhân với level để vx giảm chậm để hiện animation
				vx -= 2 * GIA_TOC;
				
				// vx đạt tới mức di chuyển sang trái thì hủy chuyển hướng để trở về animation WALKING_LEFT
				// nếu không thì vẫn sẽ là animation STOP_RIGHT đi về bên trái
				if (vx <= 0)
					ChangeDirection = false;
			}
			else 
			{
				if (isFlyingHigh == true)
				{
					vx = -MARIO_FLY_MOVING_SPEED;
				}
				else
				{
					ChangeDirection = false;
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
				//level_of_walking--;
			}

			if (vx < 0)
			{
				vx = 0;
			}

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
				//level_of_walking--;
			}
			if (vx > 0)
				vx = 0;
		}
	
		// đang trong trạng thái bay thì k trừ 
		if (canFlyX == false)
		{
			if (level_of_walking > 1)
				level_of_walking--;
			if (level_of_running > 1)
				level_of_running--;
		}
		if (isSitDown == true)
		{
			y = y - 10;
			isSitDown = false;
		}
		ChangeDirection = false;

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

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	break;

	case MARIO_LEVEL_BIG:
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isSitDown == true)
		{
			bottom = y + MARIO_BIG_BBOX_SITDOWN_HEIGHT;
		}
	}break;

	case MARIO_LEVEL_TAIL:
	{
		/*if (nx == RIGHT)
		{
			left = x + (MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH + 1);
			right = x + MARIO_TAIL_BBOX_WIDTH;
		}
		else
		{
			left = x;
			right = x + (MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH + 4);
		}*/
		right = x + MARIO_TAIL_BBOX_WIDTH;
		if (isAttacking == true && endAttack == false && nx==1)
		{
			//right = x + MARIO_TAIL_BBOX_WIDTH + (MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH);
			//DebugOut(L"MARIO right= %f, x=% f\n", right, x);
		}
		else if  (isAttacking == true && endAttack == false && nx == -1)
		{
			//left = x - (MARIO_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH);
			//DebugOut(L"MARIO left = %f, right= %f, x=% f\n", left, right, x);
		}
		if(isHolding==true)
			right = x + MARIO_TAIL_BBOX_WIDTH;

		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
		if (isSitDown == true)
		{
			bottom = y + MARIO_TAIL_BBOX_SITDOWN_HEIGHT;
		}
	}
	break;

	case MARIO_LEVEL_FIRE:
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isSitDown == true)
		{
			bottom = y + MARIO_BIG_BBOX_SITDOWN_HEIGHT;
		}
	}break;
	}
	
	//if(y<500)
		//DebugOut(L"bottom= %f, y=% f\n",bottom, y);
}

void Mario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	NumberBullet = 2;
	canFlyS = canFlyX = false;
	time_fly = 0;
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
