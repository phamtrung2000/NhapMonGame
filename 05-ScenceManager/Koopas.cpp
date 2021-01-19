#include "Koopas.h"
#include "ItemBrick.h"
#include "FireBullet.h"
#include "PlayScence.h"
#include "Coin.h"
#include "Block.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Mario.h"
#include "Ground.h"
#include "EffectHit.h"

Koopas::Koopas() : Enemy()
{
	ObjType = OBJECT_TYPE_KOOPAS;
	SetState(KOOPAS_STATE_WALKING_RIGHT);
	Score = KOOPAS_SCORE;
	TypeEnemy = ENEMYTYPE_KOOPAS;

	X_max = MAX;
	X_min = MIN;
	CountXmaxXmin = false;
	isKicked = isHold = isShell = isShell_2 =false;
	GoAround = false;
}

void Koopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
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

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
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
				if(_Mario->nx==RIGHT)
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
								if (GetState() == KOOPAS_STATE_WALKING_LEFT)
									SetState(KOOPAS_STATE_WALKING_RIGHT);
								else if (GetState() == KOOPAS_STATE_WALKING_RIGHT)
									SetState(KOOPAS_STATE_WALKING_LEFT);
								else if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
									SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
								else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
									SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
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

	/*	if (isHold)
			DebugOut(L"isHold = true\n");
		else
			DebugOut(L"isHold = false\n");

		if (isKicked)
			DebugOut(L"isKicked = true\n");
		else
			DebugOut(L"isKicked = false\n");

		DebugOut(L"vx = %f, dx = %f, state = %i\n", vx, dx, state);*/
}

void Koopas::Render()
{
	int ani = 0;

	if (isShell == true)
	{
		ani = KOOPAS_ANI_SHELL;
		if (canRevive == true)
			ani = KOOPAS_ANI_SHELL_REVIVE;
	}
	else if (isShell_2 == true)
	{
		ani = KOOPAS_ANI_SHELL_2;
		if (canRevive == true)
			ani = KOOPAS_ANI_SHELL_2_REVIVE;
	}
	if (vx > 0 && isHold==false)
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
	else if (vx < 0 && isHold==false)
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

	RenderBoundingBox();
}

void Koopas::SetState(int state)
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
		isHold =  true;
		y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
		vy = 0;
	}
	break;

	case KOOPAS_STATE_DIE:
	{
		isShell = false;
		isShell_2 = true;
		y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
		vx = nx * 0.05f;
		vy = -0.2f;
		isDie = true;
	}
	break;

	case KOOPAS_STATE_WALKING_RIGHT:
	{
		isHold = isShell = isShell_2 = false;
		vx = KOOPAS_WALKING_SPEED;
		nx = 1;
	}break;

	case KOOPAS_STATE_WALKING_LEFT:
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
	}break;

	case KOOPAS_STATE_SHELL_WALKING_LEFT:
	{
		vx = -KOOPAS_SHELL_SPEED;
		nx = LEFT;
		isHold = false;
	}break;

	}

}

void Koopas::CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj != NULL)
	{
		if (GetState() != KOOPAS_STATE_SHELL_WALKING_RIGHT && GetState() != KOOPAS_STATE_SHELL_WALKING_LEFT) // Rùa đi bộ bình thường, k ở trạng thái mai rùa
		{
			if (this->isHold == false)
			{
				if (e->nx != 0)
				{
					x += dx;
					y += min_ty * dy + ny * 0.2f;
				}
				else if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
					y += dy;
				}
			}
			else
			{
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
				switch (enemy->TypeEnemy)
				{
				case ENEMYTYPE_GOOMBA:
				{
					if (enemy->Health == 1 && enemy->GetState() != GOOMBA_STATE_DIE_2)
					{
						enemy->SetState(GOOMBA_STATE_DIE_2);
						auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
						_PlayScene->objects.push_back(hit);
						_Mario->nScore++;
						_HUD->UpdateScore(enemy, _Mario->nScore);
					}
				}
				break;

				case ENEMYTYPE_KOOPAS:
				{
					if (enemy->GetState() != KOOPAS_STATE_DIE)
					{
						if (enemy->Health == 1)
						{
							enemy->SetState(KOOPAS_STATE_DIE);
							enemy->nx = this->nx;
							auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
							_PlayScene->objects.push_back(hit);
							_Mario->nScore++;
							_HUD->UpdateScore(enemy, _Mario->nScore);
						}
						else
						{
							enemy->Health--;
							enemy->SetState(KOOPAS_STATE_DIE);
							enemy->nx = this->nx;
							auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
							_PlayScene->objects.push_back(hit);
							_Mario->nScore++;
							_HUD->UpdateScore(enemy, _Mario->nScore);
						}
					}

				}
				break;

				default:
				{
					enemy->canDelete = true;
					auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
					_PlayScene->objects.push_back(hit);
					_Mario->nScore++;
					_HUD->UpdateScore(enemy, _Mario->nScore);
				}
				break;
				}
				this->SetState(KOOPAS_STATE_DIE);
			}

		}
		else if (this->isKicked == true) // rùa ở trạng thái mai rùa và bị mario đá thì mới giết quái, tránh TH quật đuôi nhưng vẫn giết đc quái
		{
			Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
			switch (enemy->TypeEnemy)
			{
			case ENEMYTYPE_GOOMBA:
			{
				if (enemy->Health == 1 && enemy->GetState() != GOOMBA_STATE_DIE_2)
				{
					enemy->nx = this->nx;
					enemy->SetState(GOOMBA_STATE_DIE_2);
					
					auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
					_PlayScene->objects.push_back(hit);
					_Mario->nScore++;
					_HUD->UpdateScore(enemy, _Mario->nScore);
				}
			}
			break;

			case ENEMYTYPE_KOOPAS:
			{
				if (enemy->GetState() != KOOPAS_STATE_DIE)
				{
					if (enemy->Health == 1)
					{
						enemy->SetState(KOOPAS_STATE_DIE);
						enemy->nx = this->nx;
						auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
						_PlayScene->objects.push_back(hit);
						_Mario->nScore++;
						_HUD->UpdateScore(enemy, _Mario->nScore);
					}
					else
					{
						enemy->Health--;
						enemy->SetState(KOOPAS_STATE_DIE);
						enemy->nx = this->nx;
						auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
						_PlayScene->objects.push_back(hit);
						_Mario->nScore++;
						_HUD->UpdateScore(enemy, _Mario->nScore);
					}
				}

			}
			break;

			default:
			{
				enemy->canDelete = true;
				auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
				_PlayScene->objects.push_back(hit);
				_Mario->nScore++;
				_HUD->UpdateScore(enemy, _Mario->nScore);
			}
			break;
			}
		}
		else
		{
			if (e->nx != 0)
			{
				x += dx;
				y += min_ty * dy + ny * 0.2f;
			}
			else if (e->ny != 0)
			{
				x += min_tx * dx + nx * 0.4f;
				y += dy;
			}
		}
	}
}

void Koopas::CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj != NULL)
	{
		if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
		{
			if (ny != 0) vy = 0;

			if (e->nx != 0)
			{
				x += dx;
			}
			else if (e->ny < 0 )
			{
				if (CountXmaxXmin == false)
				{
					Block* block = dynamic_cast<Block*>(e->obj);
					X_min = block->x;
					X_max = X_min + (block->Width - 16);
					CountXmaxXmin = true;
				}
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
			else if (e->ny != 0)
			{
				x += min_tx * dx + nx * 0.4f;
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

void Koopas::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Enemy::CollisionWithItem(e, min_tx, min_ty, nx, ny);
}

void Koopas::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	if (e->obj->ObjType == OBJECT_TYPE_FIREBULLET)
	{
		if (dynamic_cast<FireBullet*>(e->obj))
		{
			FireBullet* firebullet = dynamic_cast<FireBullet*>(e->obj);
			if (firebullet->FireMario == false)
			{
				x += dx;
				this->y += min_ty * dy + ny * 0.1f - 0.5f;
			}
		}
	}
	else
	{
		x += dx;
		if (e->ny < 0)
			y += dy;
		else
			this->y += min_ty * dy + ny * 0.1f - 0.5f;
	}
}

// x start: 512
// x max: 592
// width : 96

