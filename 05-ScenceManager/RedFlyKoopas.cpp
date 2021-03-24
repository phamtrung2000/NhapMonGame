#include "RedFlyKoopas.h"
#include "FireBullet.h"
#include "Block.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Mario.h"
#include "Ground.h"
#include "Koopas.h"
#include "PlayScence.h"
#include "ItemBrick.h"

RedFlyKoopas::RedFlyKoopas() : Koopas()
{
	ObjType = OBJECT_TYPE_REDFLYKOOPAS;
	Health = 2;
	SetState(KOOPAS_STATE_FLY_DOWN);
	StartChangeUpDown = GetTickCount64();
	isDisappear = true;
}

RedFlyKoopas::~RedFlyKoopas()
{
}

void RedFlyKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (isShell == true || isShell_2 == true)
	{
		right = x + KOOPAS_BBOX_WIDTH_SHELL;
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else
	{
		right = x + KOOPAS_BBOX_WIDTH;
		bottom = y + KOOPAS_BBOX_HEIGHT;
	}

	if (isHold == true)
	{
		left = left - 1.f;
		right = right - 1.f;
	}
}

void RedFlyKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// đầu màn hình
	float cam_x = _Camera->cam_x;
	float camxx = _Game->GetCamX();
	// chiều dài màn hình
	int cam_w = CGame::GetInstance()->GetScreenWidth();

	//DebugOut(L"x = %f, cam_x = %i ,cam_w = %i\n", x, cam_x, cam_w);
	// ra khỏi camera -> delete
	if (x > camxx + static_cast<float>(cam_w) + 80 || x + Width < camxx - 80 || y > _Map->GetHeight())
	{
		isDisappear = true;
		return;
	}
	else
	{
		if (isDisappear == true)
		{
			SetPosition(StartX, StartY);
			if (Health == 2)
			{
				SetState(KOOPAS_STATE_FLY_DOWN);
				StartChangeUpDown = GetTickCount64();
			}
			else
				SetState(ENEMY_STATE_WALKING_LEFT);
			isDisappear = false;
		}
	}
	isDisappear = false;
	if (isAttacked == true)
	{
		if (Time_isAttacked != 0 && GetTickCount64() - Time_isAttacked > ENEMY_TIME_ISATTACKED)
		{
			isAttacked = false;
			Time_isAttacked = 0;
		}
	}

	if (isDisappear == false)
	{
		if (Health == 2)
		{
			if (StartChangeUpDown != 0 && GetTickCount64() - StartChangeUpDown > 2000)
			{
				int state = GetState();
				if (GetState() == KOOPAS_STATE_FLY_DOWN)
					SetState(KOOPAS_STATE_FLY_UP);
				else if (GetState() == KOOPAS_STATE_FLY_UP)
					SetState(KOOPAS_STATE_FLY_DOWN);
				StartChangeUpDown = GetTickCount64();
			}
			
			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();

			if (state != ENEMY_STATE_DIE_IS_JUMPED)
				CalcPotentialCollisions(coObjects, coEvents);

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

					if (e->obj)
					{
						switch (e->obj->Category)
						{
						case CATEGORY::GROUND:
						{
							if (ny != 0) vy = 0;
							if (e->ny < 0 && Health == 2)
								vy = -0.2f;
							if (dynamic_cast<Ground*>(e->obj))
							{
								if (e->ny < 0)
								{
									x += min_tx * dx + nx * 0.4f;
								}
								else if (e->nx != 0)
								{
									y += min_ty * dy + ny * 0.1f - 0.3f;
								}
							}
						}
						break;

						case CATEGORY::OBJECT:
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
							x += dx;
							y += dy;
						}
						break;

						}
					}
				}
			}

			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
		else
		{
			if (GetState() == KOOPAS_STATE_SHELL_HOLD)
			{
				if (_Mario->pressA == false)
				{
					this->isHold = false;
					this->isKicked = true;
					if (_Mario->nx == 1)
						this->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					else
						this->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
				}
				else
				{
					this->vx = _Mario->vx;
					this->nx = _Mario->nx;
					if (_Mario->nx == RIGHT)
					{
						//koopas->x = _Mario->x + MARIO_SMALL_BBOX_WIDTH + 1;
						if (_Mario->level == MARIO_LEVEL_SMALL) // chuẩn
						{
							this->x = float(_Mario->x + MARIO_SMALL_BBOX_WIDTH + 1);
							this->y = _Mario->y - 2;
						}
						else if (_Mario->level == MARIO_LEVEL_TAIL) // chuẩn
						{
							this->x = _Mario->x + MARIO_TAIL_BBOX_WIDTH - 1.0f;
							this->y = _Mario->y + 6;
						}
						else // chuẩn
						{
							this->x = float(_Mario->x + MARIO_BIG_BBOX_WIDTH - 1.0f);
							this->y = _Mario->y + 6;
						}
					}
					else
					{
						if (_Mario->level == MARIO_LEVEL_SMALL)
						{
							this->x = float(_Mario->x - KOOPAS_BBOX_WIDTH + 1.0f);
							this->y = _Mario->y - 2;
						}
						else
						{
							this->x = float(_Mario->x - KOOPAS_BBOX_WIDTH + 1.0f);
							this->y = _Mario->y + 5;
						}
					}

					vector<LPCOLLISIONEVENT> coEvents;
					vector<LPCOLLISIONEVENT> coEventsResult;

					coEvents.clear();

					if (state != ENEMY_STATE_DIE_IS_JUMPED)
						CalcPotentialCollisions(coObjects, coEvents);

					if (coEvents.size() != 0)
					{
						float min_tx, min_ty, nx = 0, ny;
						float rdx = 0;
						float rdy = 0;

						// TODO: This is a very ugly designed function!!!!
						FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

						//if (ny != 0) vy = 0;

						for (UINT i = 0; i < coEventsResult.size(); i++)
						{
							LPCOLLISIONEVENT e = coEventsResult[i];

							if (e->obj)
							{
								isInit = true;
								switch (e->obj->Category)
								{
								case CATEGORY::GROUND:
								{
									if (dynamic_cast<Ground*>(e->obj))
									{
										X_min = MIN;
										X_max = MAX;
										if (ny != 0) vy = 0;
										if (e->ny < 0)
										{
											x += min_tx * dx + nx * 0.4f;
										}
										else if (e->nx != 0)
										{
											y += min_ty * dy + ny * 0.1f - 0.3f;
										}
									}
								}
								break;

								case CATEGORY::OBJECT:
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
									x += dx;
									y += dy;
								}
								break;

								}
							}
						}
					}
					for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

				}

				if (GetTickCount64() - ReviveTime > KOOPAS_START_REVIVE_TIME)
				{
					canRevive = true;
					if (GetTickCount64() - ReviveTime > KOOPAS_REVIVE_TIME)
					{
						ReviveTime = 0;
						canRevive = false;
						y = (INT16)(y - (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) - 1);
						if (_Mario->nx == RIGHT)
							SetState(ENEMY_STATE_WALKING_LEFT);
						else
							SetState(ENEMY_STATE_WALKING_RIGHT);
					}
				}
			}
			else
			{
				//DebugOut(L"x max %f, x min %f, x %f, state %i\n", X_max, X_min, x, state);
				canRevive = false;
				if (isShell == false && isShell_2 == false)
				{
					vy += KOOPAS_GRAVITY * dt;
				}
				else if (GetState() != KOOPAS_STATE_SHELL_HOLD)
				{
					if (isShell == true)
					{
						vy += KOOPAS_SHELL_2_GRAVITY * dt;
						if (this->vx == 0)
							this->SetState(KOOPAS_STATE_SHELL);
					}
					// vẫy đuôi -> rùa lật ngửa rớt từ trên xuống
					else if (isShell_2 == true)
					{
						vy += KOOPAS_SHELL_2_GRAVITY * dt;
						if (vx > 0)
						{
							vx -= 0.0005f;
							if (vx < 0)
								vx = 0;
						}
						else if (vx < 0)
						{
							vx += 0.0005f;
							if (vx > 0)
								vx = 0;
						}
						else
						{
							if (GetState() == ENEMY_STATE_DIE_IS_JUMPED)
							{
								if (this->y > _Map->GetHeight())
									canDelete = true;
							}
							else
							{
								if (this->vx == 0)
									this->SetState(KOOPAS_STATE_SHELL_2);
							}
						}
					}
				}

				if (GetState() == ENEMY_STATE_WALKING_RIGHT || GetState() == ENEMY_STATE_WALKING_LEFT)
				{
					if (x >= X_max && vx > 0)
					{
						SetState(ENEMY_STATE_WALKING_LEFT);
					}
					else if (x <= X_min && vx < 0)
					{
						SetState(ENEMY_STATE_WALKING_RIGHT);
					}
				}
				else if ((GetState() == KOOPAS_STATE_SHELL || GetState() == KOOPAS_STATE_SHELL_2 || GetState() == KOOPAS_STATE_SHELL_HOLD)
					&& GetTickCount64() - ReviveTime > KOOPAS_START_REVIVE_TIME)
				{
					canRevive = true;
					if (GetTickCount64() - ReviveTime > KOOPAS_REVIVE_TIME)
					{
						ReviveTime = 0;
						canRevive = false;
						y = (INT16)(y - (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) - 1);
						SetState(ENEMY_STATE_WALKING_RIGHT);
					}
				}

				vector<LPCOLLISIONEVENT> coEvents;
				vector<LPCOLLISIONEVENT> coEventsResult;

				coEvents.clear();

				if (state != ENEMY_STATE_DIE_IS_JUMPED)
					CalcPotentialCollisions(coObjects, coEvents);

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

						if (e->obj)
						{
							switch (e->obj->Category)
							{
							case CATEGORY::GROUND:
							{
								if (ny != 0) vy = 0;
								if (e->ny < 0 && Health == 2)
									vy = -0.2f;
								if (dynamic_cast<Ground*>(e->obj))
								{
									if (e->ny < 0)
									{
										x += min_tx * dx + nx * 0.4f;
									}
									else if (e->nx != 0)
									{
										y += min_ty * dy + ny * 0.1f - 0.3f;
									}
								}
							}
							break;

							case CATEGORY::OBJECT:
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
								x += dx;
								y += dy;
							}
							break;

							}
						}
					}
				}

				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
			}
		}
		
	}
}

void RedFlyKoopas::Render()
{
	int ani = 0;
	if (Health == 2)
	{
		ani = REDFLYKOOPAS_ANI_WALKING_LEFT;
		animation_set->at(ani)->Render(x, y);
	}
	else
	{
		if (isShell == true)
		{
			ani = KOOPAS_ANI_SHELL;
			if (ReviveTime != 0 && GetTickCount64() - ReviveTime > KOOPAS_START_REVIVE_TIME)
				ani = KOOPAS_ANI_SHELL_REVIVE;
		}
		else if (isShell_2 == true)
		{
			ani = KOOPAS_ANI_SHELL_2;
			if (ReviveTime != 0 && GetTickCount64() - ReviveTime > KOOPAS_START_REVIVE_TIME)
				ani = KOOPAS_ANI_SHELL_2_REVIVE;
		}
		if (vx > 0 && isHold == false)
		{
			ani = KOOPAS_ANI_WALKING_RIGHT;
			if (isShell == true)
			{
				ani = KOOPAS_ANI_SHELL_WALKING_RIGHT;
			}
			else if (isShell_2 == true)
			{
				ani = KOOPAS_ANI_SHELL_2_WALKING_RIGHT;
			}
		}
		else if (vx < 0 && isHold == false)
		{
			ani = KOOPAS_ANI_WALKING_LEFT;
			if (isShell == true)
			{
				ani = KOOPAS_ANI_SHELL_WALKING_LEFT;
			}
			else if (isShell_2 == true)
			{
				ani = KOOPAS_ANI_SHELL_2_WALKING_LEFT;
			}
		}


		animation_set->at(ani)->Render(x, y);
	}
}

void RedFlyKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
	{
		// nếu k phải mai rùa thì mới tính lại y, vì có trường hợp mai rùa di chuyển bị đạp xuống thì dừng lại
		if (isShell == false && isShell_2 == false)
			y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
		isShell = true;
		isShell_2 = false;
		vx = 0;
		isKicked = false;

	}
	break;

	case KOOPAS_STATE_SHELL_2:
	{
		isShell = false;
		isShell_2 = true;
		isKicked = false;
		vx = 0;
	}
	break;

	case KOOPAS_STATE_SHELL_HOLD:
	{
		isHold = true;
		y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
		vy = 0;
		CountXmaxXmin = false;
	}
	break;

	case ENEMY_STATE_DIE_IS_JUMPED:
	{
		isShell = false;
		isShell_2 = true;
		y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
		vx = nx * 0.05f;
		vy = -0.2f;
		isDie = true;
	}
	break;

	case ENEMY_STATE_WALKING_RIGHT:
	{
		isHold = isShell = isShell_2 = false;
		vx = KOOPAS_WALKING_SPEED;

		nx = 1;
		//CountXmaxXmin = false;
	}break;

	case ENEMY_STATE_WALKING_LEFT:
	{
		isHold = isShell = isShell_2 = false;
		vx = -KOOPAS_WALKING_SPEED;
		nx = -1;
	}break;


	case KOOPAS_STATE_SHELL_WALKING_RIGHT:
	{
		vx = KOOPAS_SHELL_SPEED;
		nx = RIGHT;
		isHold = false;
		//CountXmaxXmin = false;
	}break;

	case KOOPAS_STATE_SHELL_WALKING_LEFT:
	{
		vx = -KOOPAS_SHELL_SPEED;
		nx = LEFT;
		isHold = false;
	}break;

	case KOOPAS_STATE_FLY_DOWN:
	{
		Health = 2;
		vy = 0.05f;
		vx = 0;
	}
	break;

	case KOOPAS_STATE_FLY_UP:
	{
		Health = 2;
		vy = -0.05f;
		vx = 0;
	}
	break;

	}
}

void RedFlyKoopas::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (Health == 1)
	{
		Koopas::CollisionWithEnemy(e, min_tx, min_ty, nx, ny);
	}
	else
	{
		if (e->nx != 0)
		{
			x += dx;
			y += min_ty * dy + ny * 0.4f;
		}
		else if (e->ny < 0)
		{
			x += min_tx * dx + nx * 0.4f;
			y += dy;
		}
	}
}

void RedFlyKoopas::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (Health == 2)
	{
		if (e->ny < 0)
			vy = -0.2f;
		if (e->obj != NULL)
		{
			if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
			{
				if (e->nx != 0)
				{
					x += dx;
				}
				else if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
				}
			}
			else if (e->obj->ObjType == OBJECT_TYPE_WARPPIPE)
			{
				if (ny != 0) vy = 0;
				if (e->nx != 0)
				{
					y += min_ty * dy + ny * 0.2f - 0.4f;
					if (isShell == false && isShell_2 == false)
					{
						if (GetState() == ENEMY_STATE_WALKING_RIGHT)
							SetState(ENEMY_STATE_WALKING_LEFT);
						else if (GetState() == ENEMY_STATE_WALKING_LEFT)
							SetState(ENEMY_STATE_WALKING_RIGHT);
					}
					else
					{
						if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
				}
			}
			else if (e->obj->ObjType == OBJECT_TYPE_QUESTIONBRICK)
			{
				QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
				if (e->nx != 0)
				{
					if (isShell == false && isShell_2 == false)
					{
						if (GetState() == ENEMY_STATE_WALKING_RIGHT)
							SetState(ENEMY_STATE_WALKING_LEFT);
						else if (GetState() == ENEMY_STATE_WALKING_LEFT)
							SetState(ENEMY_STATE_WALKING_RIGHT);
					}
					else
					{
						if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
					if (brick->GetState() == BRICK_STATE_NORMAL && brick->Item < 10 && (isShell == true || isShell_2 == true))
						brick->SetState(BRICK_STATE_COLLISION);
				}
				else if (e->ny < 0)
				{
					this->x += min_tx * dx + nx * 0.4f;
				}
			}
			else if (e->obj->ObjType == OBJECT_TYPE_ITEMBRICK)
			{
				ItemBrick* brick = dynamic_cast<ItemBrick*>(e->obj);
				if (e->ny < 0)
				{
					this->x += min_tx * dx + nx * 0.4f;
					if (CountXmaxXmin == false)
					{
						X_min = brick->x - ITEMBRICK_WIDTH / 2 - 2;
						X_max = X_min + ITEMBRICK_WIDTH - 2;
						CountXmaxXmin = true;
					}
				}
				else if (e->nx != 0)
				{
					if (isShell == false && isShell_2 == false)
					{
						if (GetState() == ENEMY_STATE_WALKING_RIGHT)
							SetState(ENEMY_STATE_WALKING_LEFT);
						else if (GetState() == ENEMY_STATE_WALKING_LEFT)
							SetState(ENEMY_STATE_WALKING_RIGHT);
					}
					else
					{
						brick->canDelete = true;
						if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
				}

			}
		}
	}
	else
	{
		Koopas::CollisionWithObject(e, min_tx, min_ty, nx, ny);
	}


}

void RedFlyKoopas::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Koopas::CollisionWithItem(e, min_tx, min_ty, nx, ny);
}

void RedFlyKoopas::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Koopas::CollisionWithWeapon(e, min_tx, min_ty, nx, ny);
}

