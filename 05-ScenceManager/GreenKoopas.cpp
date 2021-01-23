#include "GreenKoopas.h"
#include "FireBullet.h"
#include "Block.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Mario.h"
#include "Ground.h"
#include "PlayScence.h"
#include "ItemBrick.h"
GreenKoopas::GreenKoopas() : Koopas()
{
	ObjType = OBJECT_TYPE_GREENKOOPAS;
	SetState(KOOPAS_STATE_WALKING_LEFT);
}

GreenKoopas::~GreenKoopas()
{
}

void GreenKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	Koopas::GetBoundingBox(left, top, right, bottom);
}

void GreenKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) // k dùng hàm update của koopas vì nó k giới hạn đi qua đi lại trên BLOCk
{
	Enemy::Update(dt, coObjects);
	if (isDisappear == false)
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
				if (vy < 0)
				{
					OnGroud = false;
				}
				vector<LPCOLLISIONEVENT> coEvents;
				vector<LPCOLLISIONEVENT> coEventsResult;

				coEvents.clear();

				if (state != KOOPAS_STATE_DIE)
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
										OnGroud = true;
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
						SetState(KOOPAS_STATE_WALKING_LEFT);
					else
						SetState(KOOPAS_STATE_WALKING_RIGHT);
				}
			}
		}
		else
		{
			//DebugOut(L"state=%i, vx=%f,vy=%f, nx=%i, y = %f\n", state, vx, vy, nx, y);
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
						if (GetState() == KOOPAS_STATE_DIE)
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

			if (GetState() == KOOPAS_STATE_WALKING_RIGHT || GetState() == KOOPAS_STATE_WALKING_LEFT)
			{
				if (x >= X_max && vx > 0)
				{
					SetState(KOOPAS_STATE_WALKING_LEFT);
				}
				else if (x <= X_min && vx < 0)
				{
					SetState(KOOPAS_STATE_WALKING_RIGHT);
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
					SetState(KOOPAS_STATE_WALKING_RIGHT);
				}
			}

			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();

			if (state != KOOPAS_STATE_DIE)
				CalcPotentialCollisions(coObjects, coEvents);

			if (vy < 0)
			{
				OnGroud = false;
			}

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

				//if (ny != 0) vy = 0;

				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult[i];

					if (e->obj)
					{
						switch (e->obj->Category)
						{
						case CATEGORY::GROUND:
						{
							if (dynamic_cast<Ground*>(e->obj))
							{
								if (ny != 0) vy = 0;
								if (e->ny < 0)
								{
									x += min_tx * dx + nx * 0.4f;
									OnGroud = true;
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

void GreenKoopas::Render()
{
	Koopas::Render();
}

void GreenKoopas::SetState(int state)
{
	Koopas::SetState(state);
}

void GreenKoopas::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Koopas::CollisionWithEnemy(e, min_tx, min_ty, nx, ny);
}

void GreenKoopas::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj != NULL)
	{
		if (e->ny < 0)
			OnGroud = true;
		if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
		{
			if (ny != 0) vy = 0;

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
					if (GetState() == KOOPAS_STATE_WALKING_RIGHT)
						SetState(KOOPAS_STATE_WALKING_LEFT);
					else if (GetState() == KOOPAS_STATE_WALKING_LEFT)
						SetState(KOOPAS_STATE_WALKING_RIGHT);
				}
				else
				{
					if (isKicked == true)
					{
						if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
							SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
							SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
					else
					{
						this->nx = -this->nx;
						this->vx = -this->vx;
					}

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
					if (GetState() == KOOPAS_STATE_WALKING_RIGHT)
						SetState(KOOPAS_STATE_WALKING_LEFT);
					else if (GetState() == KOOPAS_STATE_WALKING_LEFT)
						SetState(KOOPAS_STATE_WALKING_RIGHT);
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
					if (GetState() == KOOPAS_STATE_WALKING_RIGHT)
						SetState(KOOPAS_STATE_WALKING_LEFT);
					else if (GetState() == KOOPAS_STATE_WALKING_LEFT)
						SetState(KOOPAS_STATE_WALKING_RIGHT);
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
		else if (e->obj->ObjType == OBJECT_TYPE_BRICK)
		{
			if (ny != 0) vy = 0;
			Brick* brick = dynamic_cast<Brick*>(e->obj);

			if (e->nx != 0)
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.1f - 0.4f;
			}
			else if (e->ny < 0)
			{
				x += min_tx * dx + nx * 0.4f;
			}
		}
	}
}

void GreenKoopas::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Koopas::CollisionWithItem(e, min_tx, min_ty, nx, ny);
}

void GreenKoopas::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Koopas::CollisionWithWeapon(e, min_tx, min_ty, nx, ny);
}
