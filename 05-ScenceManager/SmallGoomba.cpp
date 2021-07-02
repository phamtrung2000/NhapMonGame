#include "SmallGoomba.h"
#include "FlyGoomba.h"

#define RIGHT_MARIO (_Mario->x + _Mario->Width) - 1
#define LEFT_MARIO	 _Mario->x - 3
#define MAX_JUMP_X	3
#define MAX_JUMP_S	5

SmallGoomba::SmallGoomba(float a, float b) : Enemy()
{
	ObjType = OBJECT_TYPE_SMALLGOOMBA;
	EnemyType = ENEMY_TYPE_GOOMBA;
	x = a;
	Start_X = a;
	y = b;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(31);
	this->SetAnimationSet(ani_set);
	SetState(ENEMY_STATE_WALKING_RIGHT);
	phase = 0;
	Width = 9;
	Height = 8;
}

void SmallGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != ENEMY_STATE_DIE_IS_ATTACKED)
	{
		left = x;
		top = y;
		right = x + OBJECT_BBOX_WIDTH_HEIGHT / 2;
		bottom = y + OBJECT_BBOX_WIDTH_HEIGHT / 2;
	}
}

void SmallGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	if (state != SMALLGOOMBA_STATE_FOLLOWMARIO)
	{
		if (state == ENEMY_STATE_DIE_IS_ATTACKED || state == ENEMY_STATE_DIE_IS_JUMPED)
		{
			vy += (ENEMY_GRAVITY * dt);
			if (y > _Map->GetHeight())
				canDelete = true;
		}
		else
		{
			vy += (FLYGOOMBA_FLYING_GRAVITY * dt) / 2;
			if (x - Start_X >= 8)
			{
				SetState(ENEMY_STATE_WALKING_LEFT);
			}
			else if (x - Start_X <= 0)
			{
				SetState(ENEMY_STATE_WALKING_RIGHT);
			}
		}
		if (vx < 0 && x < 0)
		{
			x = 0;
			vx = -vx;
			nx = -nx;
		}
	}
	else
	{
		if (_Mario->jump_count_X >= MAX_JUMP_X || _Mario->jump_count_S >= MAX_JUMP_S)
		{
			SetState(ENEMY_STATE_DIE_IS_JUMPED);
			if(_Mario->NumberSmallGoomba > 0)
				_Mario->NumberSmallGoomba--;
			return;
		}
		
		TimeFollow++;
		int level = _Mario->level;
		switch (level)
		{
			case MARIO_LEVEL_SMALL:
			{
				if (phase == 1) // trái qua phải, trên -> xuống
				{
					float x_result = LEFT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - Height + 1;
					x_result += vx * TimeFollow * 20;
					y_result += vy * TimeFollow * 15;

					if (x_result >= RIGHT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}
					x = x_result;
					y = y_result;
					nx = RIGHT;
				}
				else if (phase == 2) // phải qua trái, trên -> xuống, sau lưng
				{
					float x_result = RIGHT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - Height + 4;
					x_result -= vx * TimeFollow * 20;
					y_result += vy * TimeFollow * 15;

					if (x_result <= LEFT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = LEFT;
				}
				else if (phase == 3) // trái qua phải, dưới -> trên, trước mặt
				{
					float x_result = LEFT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - Height + 7;
					x_result += vx * TimeFollow * 20;
					y_result -= vy * TimeFollow * 10;

					if (x_result >= RIGHT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = RIGHT;
				}
				//// bắt đầu đi từ nửa người lên
				else if (phase == 4) // phải qua trái , dưới -> trên , về lại chỗ cũ , đi vòng sau lưng
				{
					float x_result = RIGHT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - Height + 5;
					x_result -= vx * TimeFollow * 20;
					y_result -= vy * TimeFollow * 10;

					if (x_result <= LEFT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = LEFT;
				}
				else if (phase == 5) // trái qua phải , dưới -> trên , ở ngay giữa người, trước mặt
				{
					float x_result = LEFT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - Height + 3;
					x_result += vx * TimeFollow * 20;
					y_result -= vy * TimeFollow * 10;

					if (x_result >= RIGHT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = RIGHT;
				}
				else if (phase == 6) // phải qua trái , dưới -> trên ,từ mũi, sau lưng
				{
					float x_result = RIGHT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - Height + 1;
					x_result -= vx * TimeFollow * 20;
					y_result -= vy * TimeFollow * 10;

					if (x_result <= LEFT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = LEFT;
				}
				else if (phase == 7) // trái qua phải ,trên -> dưới, trên cùng, trước mặt
				{
					float x_result = LEFT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - Height - 2;
					x_result += vx * TimeFollow * 20;
					y_result += vy * TimeFollow * 5;

					if (x_result >= RIGHT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = RIGHT;
				}
				else if (phase == 8) // phải qua trái , trên -> dưới ,trên giữa người, sau lưng
				{
					float x_result = RIGHT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - Height + 1;
					x_result -= vx * TimeFollow * 20;
					y_result += vy * TimeFollow * 5;

					if (x_result <= LEFT_MARIO)
					{
						TimeFollow = 0;
						phase = 1;
					}

					x = x_result;
					y = y_result;
					nx = LEFT;
				}
			}
			break;
			case MARIO_LEVEL_BIG: case MARIO_LEVEL_FIRE:
			{
				if (phase == 1) // trái qua phải, trên -> xuống
				{
					float x_result = LEFT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2;
					x_result += vx * TimeFollow * 20;
					y_result += vy * TimeFollow * 15;

					if (x_result >= RIGHT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = RIGHT;
				}
				else if (phase == 2) // phải qua trái, trên -> xuống, sau lưng
				{
					float x_result = RIGHT_MARIO;
					float y_result = (_Mario->y + _Mario->Height / 2) + 3;
					x_result -= vx * TimeFollow * 20;
					y_result += vy * TimeFollow * 15;

					if (x_result <= LEFT_MARIO)
					{
						TimeFollow = 0;
						phase = 3;
					}

					x = x_result;
					y = y_result;
					nx = LEFT;
				}
				else if (phase == 3) // trái qua phải, dưới -> trên
				{
					float x_result = LEFT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 + 7;
					x_result += vx * TimeFollow * 20;
					y_result -= vy * TimeFollow * 20;

					if (x_result >= RIGHT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = RIGHT;
				}
				else if (phase == 4) // phải qua trái , dưới -> trên , về lại chỗ cũ , đi vòng sau lưng
				{
					float x_result = RIGHT_MARIO;
					float y_result = (_Mario->y + _Mario->Height / 2) + 3;
					x_result -= vx * TimeFollow * 20;
					y_result -= vy * TimeFollow * 15;

					if (x_result <= LEFT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = LEFT;
				}
				// bắt đầu đi từ nửa người lên
				else if (phase == 5) // trái qua phải , dưới -> trên , ở ngay giữa người, trước mặt
				{
					float x_result = LEFT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2;
					x_result += vx * TimeFollow * 20;
					y_result -= vy * TimeFollow * 25;

					if (x_result >= RIGHT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = RIGHT;
				}
				else if (phase == 6) // phải qua trái , dưới -> trên ,từ mũi, sau lưng
				{
					float x_result = RIGHT_MARIO;
					float y_result = (_Mario->y + _Mario->Height / 2) - 6;
					x_result -= vx * TimeFollow * 20;
					y_result -= vy * TimeFollow * 15;

					if (x_result <= LEFT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = LEFT;
				}
				else if (phase == 7) // trái qua phải ,trên -> dưới, trên cùng, trước mặt
				{
					float x_result = LEFT_MARIO;
					float y_result = (_Mario->y + _Mario->Height / 2) - 9;
					x_result += vx * TimeFollow * 20;
					y_result += vy * TimeFollow * 15;

					if (x_result >= RIGHT_MARIO)
					{
						TimeFollow = 0;
						phase++;
					}

					x = x_result;
					y = y_result;
					nx = RIGHT;
				}
				else if (phase == 8) // phải qua trái , trên -> dưới ,trên giữa người, sau lưng
				{
					float x_result = RIGHT_MARIO;
					float y_result = _Mario->y + _Mario->Height / 2 - 6;
					x_result -= vx * TimeFollow * 20;
					y_result += vy * TimeFollow * 20;

					if (x_result <= LEFT_MARIO)
					{
						TimeFollow = 0;
						phase = 1;
					}

					x = x_result;
					y = y_result;
					nx = LEFT;
				}
			}
			break;
			
			case MARIO_LEVEL_TAIL:
			{
				{
					if (phase == 1) // trái qua phải, trên -> xuống
					{
						float x_result = LEFT_MARIO;
						float y_result = _Mario->y + _Mario->Height / 2;
						x_result += vx * TimeFollow * 20;
						y_result += vy * TimeFollow * 15;

						if (x_result >= RIGHT_MARIO)
						{
							TimeFollow = 0;
							phase++;
						}

						x = x_result;
						y = y_result;
						nx = RIGHT;
					}
					else if (phase == 2) // phải qua trái, trên -> xuống, sau lưng
					{
						float x_result = RIGHT_MARIO;
						float y_result = (_Mario->y + _Mario->Height / 2) + 3;
						x_result -= vx * TimeFollow * 20;
						y_result += vy * TimeFollow * 15;

						if (x_result <= LEFT_MARIO)
						{
							TimeFollow = 0;
							phase = 3;
						}

						x = x_result;
						y = y_result;
						nx = LEFT;
					}
					else if (phase == 3) // trái qua phải, dưới -> trên
					{
						float x_result = LEFT_MARIO;
						float y_result = _Mario->y + _Mario->Height / 2 + 7;
						x_result += vx * TimeFollow * 20;
						y_result -= vy * TimeFollow * 20;

						if (x_result >= RIGHT_MARIO)
						{
							TimeFollow = 0;
							phase++;
						}

						x = x_result;
						y = y_result;
						nx = RIGHT;
					}
					else if (phase == 4) // phải qua trái , dưới -> trên , về lại chỗ cũ , đi vòng sau lưng
					{
						float x_result = RIGHT_MARIO;
						float y_result = (_Mario->y + _Mario->Height / 2) + 3;
						x_result -= vx * TimeFollow * 20;
						y_result -= vy * TimeFollow * 15;

						if (x_result <= LEFT_MARIO)
						{
							TimeFollow = 0;
							phase++;
						}

						x = x_result;
						y = y_result;
						nx = LEFT;
					}
					// bắt đầu đi từ nửa người lên
					else if (phase == 5) // trái qua phải , dưới -> trên , ở ngay giữa người, trước mặt
					{
						float x_result = LEFT_MARIO;
						float y_result = _Mario->y + _Mario->Height / 2;
						x_result += vx * TimeFollow * 20;
						y_result -= vy * TimeFollow * 25;

						if (x_result >= RIGHT_MARIO)
						{
							TimeFollow = 0;
							phase++;
						}

						x = x_result;
						y = y_result;
						nx = RIGHT;
					}
					else if (phase == 6) // phải qua trái , dưới -> trên ,từ mũi, sau lưng
					{
						float x_result = RIGHT_MARIO;
						float y_result = (_Mario->y + _Mario->Height / 2) - 6;
						x_result -= vx * TimeFollow * 20;
						y_result -= vy * TimeFollow * 15;

						if (x_result <= LEFT_MARIO)
						{
							TimeFollow = 0;
							phase++;
						}

						x = x_result;
						y = y_result;
						nx = LEFT;
					}
					else if (phase == 7) // trái qua phải ,trên -> dưới, trên cùng, trước mặt
					{
						float x_result = LEFT_MARIO;
						float y_result = (_Mario->y + _Mario->Height / 2) - 9;
						x_result += vx * TimeFollow * 20;
						y_result += vy * TimeFollow * 15;

						if (x_result >= RIGHT_MARIO)
						{
							TimeFollow = 0;
							phase++;
						}

						x = x_result;
						y = y_result;
						nx = RIGHT;
					}
					else if (phase == 8) // phải qua trái , trên -> dưới ,trên giữa người, sau lưng
					{
						float x_result = RIGHT_MARIO;
						float y_result = _Mario->y + _Mario->Height / 2 - 6;
						x_result -= vx * TimeFollow * 20;
						y_result += vy * TimeFollow * 20;

						if (x_result <= LEFT_MARIO)
						{
							TimeFollow = 0;
							phase = 1;
						}

						x = x_result;
						y = y_result;
						nx = LEFT;
					}
				}
			}break;

		default:
			break;
		}
		
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != ENEMY_STATE_DIE_IS_JUMPED)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0 && state != SMALLGOOMBA_STATE_FOLLOWMARIO)
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
					case CATEGORY::PLAYER:
					{
						CollisionWithPlayer(e, min_tx, min_ty, nx, ny);
					}
					break;

					default:
					{
						y += dy;
						x += dx;
					}
					break;
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"vy = %f, dy = %f, state = %i\n", vy, dy, state);
}

void SmallGoomba::Render()
{
	if (phase != 2 && phase != 4 && phase != 6 && phase != 8)
	{
		int ani = -1;
		if (state == ENEMY_STATE_DIE_IS_JUMPED || state == ENEMY_STATE_DIE_IS_ATTACKED)
		{
			ani = SMALLGOOMBA_ANI_DIE;
		}
		else
			ani = SMALLGOOMBA_ANI_FLYING;
		animation_set->at(ani)->Render(x, y);
	}
	else
	{
		if (TimeFollow < 20 || TimeFollow > 70)
		{
			int ani = -1;
			if (state == ENEMY_STATE_DIE_IS_JUMPED || state == ENEMY_STATE_DIE_IS_ATTACKED)
			{
				ani = SMALLGOOMBA_ANI_DIE;
			}
			else
				ani = SMALLGOOMBA_ANI_FLYING;
			animation_set->at(ani)->Render(x, y);
		}
	}

	/*int ani = -1;
	if (state == ENEMY_STATE_DIE_IS_JUMPED || state == ENEMY_STATE_DIE_IS_ATTACKED)
	{
		ani = SMALLGOOMBA_ANI_DIE;
	}
	else
		ani = SMALLGOOMBA_ANI_FLYING;
	animation_set->at(ani)->Render(x, y);*/
}

void SmallGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case ENEMY_STATE_DIE_IS_JUMPED : case ENEMY_STATE_DIE_IS_ATTACKED:
		{
			vy = -ENEMY_DIE_DEFLECT_SPEED;
			vx = nx * abs(0.02f);
			isDie = true;
		}
		break;

		case ENEMY_STATE_INIT:
		{
			vy = ENEMY_INIT_GRAVITY;
		}
		break;

		case ENEMY_STATE_WALKING_RIGHT:
		{
			nx = RIGHT;
			vx = nx * ENEMY_WALKING_SPEED;
		}
		break;

		case ENEMY_STATE_WALKING_LEFT:
		{
			nx = LEFT;
			vx = nx * ENEMY_WALKING_SPEED;
		}
		break;

		case SMALLGOOMBA_STATE_FOLLOWMARIO:
		{
			vx = 0.01f;
			vy = 0.003f;
			x = _Mario->x;
			y = _Mario->y + _Mario->Height / 2;
			TimeFollow++;
			phase = 1;
		}
		break;
	}
}

void SmallGoomba::CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Enemy::CollisionWithItem(e, min_tx, min_ty, nx, ny);
}

void SmallGoomba::CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Enemy::CollisionWithWeapon(e, min_tx, min_ty, nx, ny);
}

void SmallGoomba::CollisionWithPlayer(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny)
{
	Mario* mario = dynamic_cast<Mario*>(e->obj);
	if (e->nx != 0)
	{
		if (mario->untouchable == true)
		{
			x += dx;
		}
		if (ny != 0) vy = 0;
	}
	else if (e->ny < 0)
	{
		if (this->state != SMALLGOOMBA_STATE_FOLLOWMARIO)
		{
			_Mario->NumberSmallGoomba++;
			this->SetState(SMALLGOOMBA_STATE_FOLLOWMARIO);
		}
		
	}
}