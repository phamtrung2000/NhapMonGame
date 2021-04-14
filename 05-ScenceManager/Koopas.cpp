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
#include "QuestionBrickItem.h"
#include "ListItemBrick.h"
#include "ListQuestionBrick.h"

Koopas::Koopas() : Enemy()
{
	ObjType = OBJECT_TYPE_KOOPAS;
	Score = KOOPAS_SCORE;
	EnemyType = ENEMY_TYPE_KOOPAS;

	X_max = MAX;
	X_min = MIN;
	CountXmaxXmin = false;
	isKicked = isHold = isShell = isShell_2 = false;
	Width = KOOPAS_BBOX_WIDTH;
	Height = KOOPAS_BBOX_HEIGHT;
}

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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
	if (isDisappear == false)
	{
		if (state == KOOPAS_STATE_DIE || state == ENEMY_STATE_DIE_IS_ATTACKED)
		{
			if (isDie == false)
			{
				_Mario->nScore++;
				_HUD->UpdateScore(this, _Mario->nScore);
				isDie = true;
			}
			vy += ENEMY_GRAVITY * dt;
			if (y > _Map->GetHeight())
				canDelete = true;
			x += dx;
			y += dy;
			return;
		}

		if (isHold == true)
		{
			canRevive = true;
			// Mario đang cầm mai rùa mà thả nút A ra thì đá rùa
			if (_Mario->pressA == false)
			{
				this->isHold = false;
				this->isKicked = true;
				if (_Mario->nx == RIGHT)
				{
					if (isShell == true)
						SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					else
						SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
				}
				else
				{
					if (isShell == true)
						SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
					else
						SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
				}
				return;
			}
			// rùa đang bị cầm và nút A đang giữ
			else
			{
				//this->vx = _Mario->vx;
				this->nx = _Mario->nx;
				if (_Mario->nx == RIGHT)
				{
					if (isShell == true)
					{
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
					else if (isShell_2 == true)
					{
						if (_Mario->level == MARIO_LEVEL_SMALL) // chuẩn
						{
							this->x = float(_Mario->x + MARIO_SMALL_BBOX_WIDTH - 1);
							this->y = float(_Mario->y - 4);
						}
						else if (_Mario->level == MARIO_LEVEL_TAIL) // chuẩn
						{
							this->x = float(_Mario->x + MARIO_TAIL_BBOX_WIDTH - 2);
							this->y = _Mario->y + 6;
						}
						else // chuẩn
						{
							this->x = float(_Mario->x + MARIO_BIG_BBOX_WIDTH - 2);
							this->y = _Mario->y + 6;
						}
					}
				}
				else
				{
					if (isShell == true)
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
					else
					{
						if (_Mario->level == MARIO_LEVEL_SMALL)
						{
							this->x = float(_Mario->x - KOOPAS_BBOX_WIDTH + 3.0f);
							this->y = _Mario->y - 4;
						}
						else
						{
							this->x = float(_Mario->x - KOOPAS_BBOX_WIDTH + 2.0f);
							this->y = _Mario->y + 5;
						}
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
					for (UINT i = 0; i < coObjects->size(); i++)
					{
						switch (coObjects->at(i)->Category)
						{
							case CATEGORY::OBJECT:
							{
								if (coObjects->at(i)->ObjType != OBJECT_TYPE_BLOCK)
								{
									if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true)
										canRevive = false;
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
							if (e->obj->Category == CATEGORY::ENEMY)
							{
								CollisionWithEnemy(e, min_tx, min_ty, nx, ny);
							}
							else
							{
								switch (e->obj->Category)
								{
									case CATEGORY::OBJECT:
										CollisionWithObject(e, min_tx, min_ty, nx, ny);
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
				}
				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

			}

			if (GetTickCount64() - ReviveTime > KOOPAS_REVIVE_TIME)
			{
				ReviveTime = 0;
				if (canRevive == false)
				{
					_Mario->isHolding = false;
					SetState(KOOPAS_STATE_DIE);
				}
				else
				{
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
			/*if(isKicked==true)
				DebugOut(L"isKicked == true\t");
			else
				DebugOut(L"isKicked == false\t");
			if (isShell == true)
				DebugOut(L"isShell == true\t");
			else
				DebugOut(L"isShell == false\t");
			if (isShell_2 == true)
				DebugOut(L"isShell_2 == true\t");
			else
				DebugOut(L"isShell_2 == false\t");*/
			//DebugOut(L"koopas state=%i, vx=%f,vy=%f, nx=%i, y = %f\n", state, vx, vy, nx, y);
			canRevive = false;
			if (isShell == false && isShell_2 == false)
			{
				vy += ENEMY_GRAVITY * dt;
			}
			else if (isHold == false)
			{
				if (isShell == true)
				{
					vy += ENEMY_GRAVITY * dt;
					if (this->vx == 0)
						this->SetState(KOOPAS_STATE_SHELL);
				}
				// vẫy đuôi -> rùa lật ngửa rớt từ trên xuống
				else if (isShell_2 == true)
				{
					vy += ENEMY_GRAVITY * dt;
					if (vx > 0 && isKicked == false)
					{
						vx -= KOOPAS_SHELL_2_ACCELERATION;
						if (vx < 0)
							vx = 0;
					}
					else if (vx < 0 && isKicked == false)
					{
						vx += KOOPAS_SHELL_2_ACCELERATION;
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
			else if ((GetState() == KOOPAS_STATE_SHELL || GetState() == KOOPAS_STATE_SHELL_2 || isHold == true)
				&& ReviveTime != 0 && GetTickCount64() - ReviveTime > KOOPAS_START_REVIVE_TIME)
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

				//if (ny != 0) vy = 0;

				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult[i];

					if (e->obj)
					{
						switch (e->obj->Category)
						{
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

							case CATEGORY::PLAYER:
								CollisionWithPlayer(e, min_tx, min_ty, nx, ny);
								break;
						}
					}
				}
			}
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
}

void Koopas::Render()
{
	int ani = 0;

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

void Koopas::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
		case ENEMY_STATE_DIE_IS_ATTACKED:
		{
			isShell = false;
			isShell_2 = true;
			vy = -ENEMY_DIE_DEFLECT_SPEED;
			vx = nx * abs(vx);
			isDie = true;
		}
		break;

		case ENEMY_STATE_WALKING_RIGHT:
		{
			isHold = isShell = isShell_2 = false;
		}break;

		case ENEMY_STATE_WALKING_LEFT:
		{
			isHold = isShell = isShell_2 = false;
		}break;

		case KOOPAS_STATE_SHELL:
		{
			// nếu k phải mai rùa thì mới tính lại y, vì có trường hợp mai rùa di chuyển bị đạp xuống thì dừng lại
			if (isShell == false && isShell_2 == false)
				y = (INT16)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
			isShell = true;
			isShell_2 = false;
			isKicked = false;
			vx = 0;
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
			isShell = true;
			isShell_2 = false;
			y = (float)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
			vy = 0;
			CountXmaxXmin = false;
		}
		break;

		case KOOPAS_STATE_SHELL_2_HOLD:
		{
			isHold = true;
			isShell = false;
			isShell_2 = true;
			y = (float)(y + KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL - 1);
			vy = 0;
			CountXmaxXmin = false;
		}
		break;

		case KOOPAS_STATE_SHELL_WALKING_RIGHT:
		{
			nx = RIGHT;
			isHold = false;
			CountXmaxXmin = false;
			isShell = true;
			isShell_2 = false;
			if (isAttacked == true)
				vx = ENEMY_WALKING_SPEED;
			else
				vx = KOOPAS_SHELL_SPEED;
		}break;

		case KOOPAS_STATE_SHELL_WALKING_LEFT:
		{
			nx = LEFT;
			isHold = false;
			CountXmaxXmin = false;
			isShell = true;
			isShell_2 = false;
			if (isAttacked == true)
				vx = -ENEMY_WALKING_SPEED;
			else
				vx = -KOOPAS_SHELL_SPEED;
		}break;

		case KOOPAS_STATE_SHELL_2_WALKING_RIGHT:
		{
			nx = RIGHT;
			isHold = false;
			CountXmaxXmin = false;
			isShell = false;
			isShell_2 = true;
			if (isAttacked == true)
				vx = ENEMY_WALKING_SPEED;
			else
				vx = KOOPAS_SHELL_SPEED;
		}break;

		case KOOPAS_STATE_SHELL_2_WALKING_LEFT:
		{
			nx = LEFT;
			isHold = false;
			CountXmaxXmin = false;
			isShell = false;
			isShell_2 = true;
			if (isAttacked == true)
				vx = -ENEMY_WALKING_SPEED;
			else
				vx = -KOOPAS_SHELL_SPEED;
		}break;

		case KOOPAS_STATE_DIE:
		{
			isShell = false;
			isShell_2 = true;
			vx = 0;
			vy = -0.15f;
		}
		break;

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
				switch (enemy->EnemyType)
				{
				case ENEMY_TYPE_GOOMBA:
				{
					if (enemy->Health == 1 && enemy->GetState() != ENEMY_STATE_DIE_IS_ATTACKED)
					{
						enemy->SetState(ENEMY_STATE_DIE_IS_ATTACKED);
						auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
						_PlayScene->objects.push_back(hit);
						_Mario->nScore++;
						_HUD->UpdateScore(enemy, _Mario->nScore);
					}
				}
				break;

				case ENEMY_TYPE_KOOPAS:
				{
					if (enemy->GetState() != ENEMY_STATE_DIE_IS_JUMPED)
					{
						if (enemy->Health == 1)
						{
							enemy->SetState(ENEMY_STATE_DIE_IS_JUMPED);
							enemy->nx = this->nx;
							auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
							_PlayScene->objects.push_back(hit);
							_Mario->nScore++;
							_HUD->UpdateScore(enemy, _Mario->nScore);
						}
						else
						{
							enemy->Health--;
							enemy->SetState(ENEMY_STATE_DIE_IS_JUMPED);
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
				this->SetState(ENEMY_STATE_DIE_IS_JUMPED);
			}

		}
		else if (this->isKicked == true) // rùa ở trạng thái mai rùa và bị mario đá thì mới giết quái, tránh TH quật đuôi nhưng vẫn giết đc quái
		{
			Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
			switch (enemy->EnemyType)
			{
			case ENEMY_TYPE_GOOMBA:
			{
				if (enemy->Health == 1 && enemy->GetState() != ENEMY_STATE_DIE_IS_ATTACKED)
				{
					enemy->nx = this->nx;
					enemy->SetState(ENEMY_STATE_DIE_IS_ATTACKED);

					auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
					_PlayScene->objects.push_back(hit);
					_Mario->nScore++;
					_HUD->UpdateScore(enemy, _Mario->nScore);
				}
			}
			break;

			case ENEMY_TYPE_KOOPAS:
			{
				if (enemy->GetState() != ENEMY_STATE_DIE_IS_JUMPED)
				{
					if (enemy->Health == 1)
					{
						enemy->SetState(ENEMY_STATE_DIE_IS_JUMPED);
						enemy->nx = this->nx;
						auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
						_PlayScene->objects.push_back(hit);
						_Mario->nScore++;
						_HUD->UpdateScore(enemy, _Mario->nScore);
					}
					else
					{
						enemy->Health--;
						enemy->SetState(ENEMY_STATE_DIE_IS_JUMPED);
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
		// lúc mới vào game, rùa sẽ ở trạng thái Init rớt từ trên xuống, khi chạm vật thể thì bắt đầu di chuyển
		// để tránh lỗi vụ rớt lệch khỏi viên gạch
		if(GetState() == ENEMY_STATE_INIT)
			SetState(ENEMY_STATE_WALKING_LEFT);

		if (e->ny < 0)
			OnGroud = true;

		if (e->obj->ObjType == OBJECT_TYPE_GROUND)
		{
			if (isHold == true)
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
			else
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
					if (GetState() == ENEMY_STATE_WALKING_LEFT)
						SetState(ENEMY_STATE_WALKING_RIGHT);
					else if (GetState() == ENEMY_STATE_WALKING_RIGHT)
						SetState(ENEMY_STATE_WALKING_LEFT);
					else if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
						SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
					else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
						SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_RIGHT)
						SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
					else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_LEFT)
						SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
				}
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_BLOCK)
		{
			if (ny != 0) vy = 0;

			if (e->nx != 0)
			{
				x += dx;
			}
			else if (e->ny < 0)
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
				if (GetState() == ENEMY_STATE_WALKING_LEFT)
					SetState(ENEMY_STATE_WALKING_RIGHT);
				else if (GetState() == ENEMY_STATE_WALKING_RIGHT)
					SetState(ENEMY_STATE_WALKING_LEFT);
				else if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
					SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
				else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
					SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
				else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_RIGHT)
					SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
				else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_LEFT)
					SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
			}
			else if (e->ny != 0)
			{
				x += min_tx * dx + nx * 0.4f;
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_QUESTIONBRICK)
		{
			QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
			if (e->nx != 0 && isHold == false)
			{
				if (GetState() == ENEMY_STATE_WALKING_LEFT)
					SetState(ENEMY_STATE_WALKING_RIGHT);
				else if (GetState() == ENEMY_STATE_WALKING_RIGHT)
					SetState(ENEMY_STATE_WALKING_LEFT);
				else if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT)
					SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
				else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT)
					SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
				else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_RIGHT)
					SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
				else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_LEFT)
					SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
				
				if (brick->GetState() == BRICK_STATE_NORMAL && (isShell == true || isShell_2 == true))
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
					}
					QuestionBrickItem* questionbrickitem = new QuestionBrickItem(brick->Item, brick->x, brick->y - 3);
					_PlayScene->objects.push_back(questionbrickitem);
				}
			}
			else if (e->ny < 0)
			{
				this->x += min_tx * dx + nx * 0.4f;
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_ITEMBRICK)
		{
			if (ny != 0) vy = 0;
			ItemBrick* brick = dynamic_cast<ItemBrick*>(e->obj);
			if (e->ny < 0)
			{
				/*this->x += min_tx * dx + nx * 0.4f;
				if (CountXmaxXmin == false)
				{
					X_min = brick->x - ITEMBRICK_WIDTH / 2 - 2;
					X_max = X_min + ITEMBRICK_WIDTH - 2;
					CountXmaxXmin = true;
				}*/
				this->x += min_tx * dx + nx * 0.4f;
				if ( this->x + this->Width >= brick->x + ITEMBRICK_WIDTH)
				{
					SetState(ENEMY_STATE_WALKING_LEFT);
				}
				else if (this->x <= brick->x)
				{
					SetState(ENEMY_STATE_WALKING_RIGHT);
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
					brick->SetState(ITEMBRICK_STATE_DIE);
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
				y += min_ty * dy + ny * 0.1f - 0.4f;
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
			else if (e->ny < 0)
			{
				x += min_tx * dx + nx * 0.4f;
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_LISTITEMBRICK)
		{
			if (ny != 0) vy = 0;
			ListItemBrick* listbrick = dynamic_cast<ListItemBrick*>(e->obj);
			if (e->ny < 0)
			{
				X_min = listbrick->x - 4;
				X_max = X_min + listbrick->Width - 6;
				this->x += min_tx * dx + nx * 0.4f;
				int vitri = listbrick->ViTriGachVaCham(this->x, this->Width);
				if (listbrick->Bricks.at(vitri)->state == BRICK_STATE_COLLISION)
				{
					SetState(KOOPAS_STATE_SHELL_2);
					if (this->nx == LEFT)
						SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
					else
						SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					vy = -0.07f;
					vx = this->nx * 0.04f;
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
					if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT) // đi phải thì đụng trái
					{
						listbrick->DeleteBrick(0);
						SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
					}
					else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT) // đi trái thì đụng phải
					{
						listbrick->DeleteBrick(listbrick->Bricks.size() - 1);
						SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
					else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_RIGHT) // đi phải thì đụng trái
					{
						listbrick->DeleteBrick(0);
						SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
					}
					else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_LEFT) // đi trái thì đụng phải
					{
						listbrick->DeleteBrick(listbrick->Bricks.size() - 1);
						SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
					}
				}
			}
		}
		else if (e->obj->ObjType == OBJECT_TYPE_LISTQUESTIONBRICK)
		{
			if (ny != 0) vy = 0;
			ListQuestionBrick* listbrick = dynamic_cast<ListQuestionBrick*>(e->obj);
			if (e->ny < 0)
			{
				X_min = listbrick->x - 4;
				X_max = X_min + listbrick->Width - 6;
				this->x += min_tx * dx + nx * 0.4f;
				int vitri = listbrick->ViTriGachVaCham(this->x, this->Width);
				if (listbrick->Bricks.at(vitri)->state == BRICK_STATE_COLLISION)
				{
					SetState(KOOPAS_STATE_SHELL_2);
					if(this->nx == LEFT)
						SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
					else
						SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					vy = -0.07f;
					vx = this->nx * 0.04f;
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
					if (GetState() == KOOPAS_STATE_SHELL_WALKING_RIGHT) // đi phải thì đụng trái
					{
						listbrick->DeleteBrick(0);
						SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
					}
					else if (GetState() == KOOPAS_STATE_SHELL_WALKING_LEFT) // đi trái thì đụng phải
					{
						listbrick->DeleteBrick(listbrick->Bricks.size() - 1);
						SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					}
					else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_RIGHT) // đi phải thì đụng trái
					{
						listbrick->DeleteBrick(0);
						SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
					}
					else if (GetState() == KOOPAS_STATE_SHELL_2_WALKING_LEFT) // đi trái thì đụng phải
					{
						listbrick->DeleteBrick(listbrick->Bricks.size() - 1);
						SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
					}
				}
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

void Koopas::CollisionWithPlayer(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Mario* mario = dynamic_cast<Mario*>(e->obj);
	if (e->nx != 0)
	{
		if (mario->untouchable == true)
		{
			x += dx;
		}
		else
		{
			if(isShell == false && isShell_2 == false)
				mario->DownLevel();
		}
		if (ny != 0) vy = 0;
	}
}


