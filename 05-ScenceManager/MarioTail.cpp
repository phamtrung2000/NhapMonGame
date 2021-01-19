#include "MarioTail.h"
#include "Goomba.h"
#include "Game.h"
#include "Koopas.h"
#include"PlayScence.h"
#include "ItemBrick.h"
#include "Mario.h"
#include "BrickItem.h"
#include "EffectSmoke.h"
#include "EffectHit.h"
#include "EffectScore.h"
#include "Enemy.h"
#include "QuestionBrick.h"
#include"GreenFlyKoopas.h"
MarioTail::MarioTail(float x, float y)
{
	ObjType = OBJECT_TYPE_MARIO_TAIL;
	vx = vy = 0;
	SetPosition(x, y);
	Category = CATEGORY::WEAPON;
	isInvisible = false;
}
void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (_Mario->isLevelUp == true)
	{
		_Mario->ChangeLevelTime = GetTickCount64();
		auto effect = new EffectSmoke(_Mario->x, _Mario->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
		_PlayScene->objects.push_back(effect);
		this->canDelete = true;
		_PlayScene->Stop = true;
		return;
	}
	else if (_Mario->isLevelDown == true)
	{
		_Mario->ChangeLevelTime = GetTickCount64();
		auto effect = new EffectSmoke(_Mario->x, _Mario->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
		_PlayScene->objects.push_back(effect);
		this->canDelete = true;
		_PlayScene->Stop = true;
		return;
	}
	else if (_Mario->GoHiddenWorld == true || _Mario->level != MARIO_LEVEL_TAIL)
	{
		this->canDelete = true;
		return;
	}
	
	isInvisible = false;
	if (_Mario->nx == RIGHT )
	{
		float temp_x = _Mario->x - 5;
		if (_Mario->isAttacking == false)
		{
			this->x = temp_x;
			if (_Mario->ani == MARIO_ANI_TAIL_JUMP_RIGHT)
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 3;
			else if (_Mario->ani == MARIO_ANI_TAIL_FALLING_RIGHT)
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 5;
			else if ( _Mario->isSitDown==true)
			{
				this->x = temp_x - 1;
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 8;
			}
			else if (_Mario->isFlyingLow == true)
			{
				this->x = temp_x - 1;
				this->y = _Mario->y + MARIO_TAIL_BBOX_SITDOWN_HEIGHT - 1;
			}
			else 
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 2;
		}
		else
		{
			this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 3;
			if (_Mario->time_attack <= TIME_ATTACK || _Mario->time_attack > 4 * TIME_ATTACK && _Mario->time_attack <= 5 * TIME_ATTACK)
				x = temp_x - 2;
			//else if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_3)
			else if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario-> time_attack <= 3 * TIME_ATTACK)
 				x = _Mario->x + MARIO_TAIL_BBOX_WIDTH;
		}

		if (_Mario->canFlyX == true || _Mario->canFlyS == true)
		{
			if (_Mario->isMaxRunning == true && _Mario->OnGround == false)
			{
				//if (_Mario->isFlyingHigh == true)
				isInvisible = true;
			}
		}
	}
	else if (_Mario->nx == LEFT )
	{
		if (_Mario->isAttacking == false)
		{
			this->x = _Mario->x + MARIO_BIG_BBOX_WIDTH + 2;
			if (_Mario->ani == MARIO_ANI_TAIL_JUMP_LEFT)
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 3;
			else if (_Mario->ani == MARIO_ANI_TAIL_FALLING_LEFT)
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 5;
			else if (_Mario->isSitDown == true)
			{
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 8;
			}
			else if (_Mario->isFlyingLow == true)
			{
				this->y = _Mario->y + MARIO_TAIL_BBOX_SITDOWN_HEIGHT - 1;
			}
			else
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 2;
		}
		else
		{
			if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
				this->SetPosition(_Mario->x - 7.0f, _Mario->y + 18);
				//this->SetPosition(_Mario->x - 10.0f, _Mario->y + 18);
			else if (_Mario->time_attack <= TIME_ATTACK || _Mario->time_attack > 4 * TIME_ATTACK && _Mario->time_attack <= 5 * TIME_ATTACK)
				this->SetPosition(_Mario->x + MARIO_TAIL_BBOX_WIDTH, _Mario->y + 18);
		}
	}

	if (_Mario->isAttacking == true)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			// lấy render box của 2 obj để kiểm tra xem chúng có nằm bên trong nhau hay không
			if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true)
			{
				if (coObjects->at(i)->ObjType == OBJECT_TYPE_ITEMBRICK)
				{
					ItemBrick* brick = dynamic_cast<ItemBrick*>(coObjects->at(i));
					if(brick->Item==NORMAL)
						coObjects->at(i)->canDelete = true;
					else if (brick->Item == BUTTONP && brick->hasItem == true)
					{
						brick->SetState(BRICK_STATE_COLLISION);
						BrickItem* brickitem = new BrickItem(BUTTONP, brick->x , brick->y - 16);
						_PlayScene->objects.push_back(brickitem);
						auto effect = new EffectSmoke(brick->x , brick->y - 16);
						_PlayScene->objects.push_back(effect);
						brick->hasItem = false;
					}
				}
				else if (coObjects->at(i)->ObjType == OBJECT_TYPE_QUESTIONBRICK)
				{
					QuestionBrick* brick = dynamic_cast<QuestionBrick*>(coObjects->at(i));
					if (brick->GetState() == BRICK_STATE_NORMAL)
					{
						brick->SetState(BRICK_STATE_COLLISION);
					}
				}

				else if (coObjects->at(i)->Category == CATEGORY::ENEMY)
				{
					Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));
					if (enemy->isAttacked == false)
					{
						auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
						_PlayScene->objects.push_back(hit);
						enemy->isAttacked = true;
						enemy->Time_isAttacked = GetTickCount64();
					}
				
					if (enemy->TypeEnemy != ENEMYTYPE_KOOPAS)
					{
						_Mario->nScore++;
						_HUD->UpdateScore(enemy,_Mario->nScore);
					}
					
					if (coObjects->at(i)->ObjType == OBJECT_TYPE_GOOMBA)
					{
						Goomba* goomba = dynamic_cast<Goomba*>(coObjects->at(i));
						if (goomba->GetState() == GOOMBA_STATE_WALKING_RIGHT || goomba->GetState() == GOOMBA_STATE_WALKING_LEFT)
						{
							goomba->SetState(GOOMBA_STATE_DIE_2);
						}
					}
					else if(coObjects->at(i)->ObjType == OBJECT_TYPE_KOOPAS || coObjects->at(i)->ObjType == OBJECT_TYPE_GREENKOOPAS)					
					{
						Koopas* koopas = dynamic_cast<Koopas*>(coObjects->at(i));
						koopas->vy = -0.2f;
						if (_Mario->nx == RIGHT)
						{
							koopas->SetState(KOOPAS_STATE_SHELL_2);
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
						}
						else
						{
							koopas->SetState(KOOPAS_STATE_SHELL_2);
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						}
						koopas->vx = _Mario->nx * 0.05f;
						koopas->ReviveTime = GetTickCount64();
					}
					else if (coObjects->at(i)->ObjType == OBJECT_TYPE_GREENFLYKOOPAS)
					{
						GreenFlyKoopas * koopas = dynamic_cast<GreenFlyKoopas*>(coObjects->at(i));
						koopas->Health--;
						koopas->vy = -0.2f;
						if (_Mario->nx == RIGHT)
						{
							koopas->SetState(KOOPAS_STATE_SHELL_2);
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
						}
						else
						{
							koopas->SetState(KOOPAS_STATE_SHELL_2);
							koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
						}
						koopas->vx = _Mario->nx * 0.05f;
						koopas->ReviveTime = GetTickCount64();
					}
					else
						coObjects->at(i)->canDelete = true;
				}
			}
		}
	}
	

	/*if (isInvisible == true)
		DebugOut(L"isInvisible == true\n");
	else
		DebugOut(L"isInvisible == false\n");*/
}

void MarioTail::Render()
{
	int ani = 16;
	if (isInvisible == true)
	{
		ani = MARIOTAIL_ANI_INVISIBLE;
	}
	else
	{
		if (_Mario->nx == RIGHT)
		{
			if (_Mario->isAttacking == true)
			{
				ani = MARIOTAIL_ANI_7_RIGHT;
				if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_1)
					ani = MARIOTAIL_ANI_7_RIGHT;
				else if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_2 || _Mario->ani == MARIO_ANI_TAIL_ATTACK_4)
					ani = MARIOTAIL_ANI_INVISIBLE;
				else if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_3)
					ani = MARIOTAIL_ANI_7_LEFT;
			}
			else
			{
				//if (_Mario->canFlyX == false)
				{
					ani = MARIOTAIL_ANI_6_RIGHT;
					if (_Mario->ani == MARIO_ANI_TAIL_IDLE_RIGHT)
						ani = MARIOTAIL_ANI_6_RIGHT;
					else if (_Mario->ani == MARIO_ANI_TAIL_WALKING_RIGHT || _Mario->isHolding == true)
						ani = MARIOTAIL_ANI_WALKING_RIGHT;
					else if (_Mario->ani == MARIO_ANI_TAIL_JUMP_RIGHT)
						ani = MARIOTAIL_ANI_7_RIGHT;
					else if (_Mario->ani == MARIO_ANI_TAIL_FALLING_RIGHT)
						ani = MARIOTAIL_ANI_1_RIGHT;
					else if (_Mario->ani == MARIO_ANI_TAIL_SITDOWN_RIGHT)
						ani = MARIOTAIL_ANI_0_RIGHT;
					else if (_Mario->isFlyingLow == true)
						ani = MARIOTAIL_ANI_FLYINGLOW_RIGHT;
					if (_Mario->canFlyS == true || _Mario->ani == MARIO_ANI_TAIL_STOP_RIGHT || _Mario->ani == MARIO_ANI_TAIL_RUNNING_RIGHT || _Mario->ani == MARIO_ANI_TAIL_KICK_RIGHT)
						ani = MARIOTAIL_ANI_INVISIBLE;
				}
				//else
			}
		}
		else
		{
			if (_Mario->isAttacking == true)
			{
				ani = MARIOTAIL_ANI_7_LEFT;
				if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_1)
					ani = MARIOTAIL_ANI_7_RIGHT;
				else if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_2 || _Mario->ani == MARIO_ANI_TAIL_ATTACK_4)
					ani = MARIOTAIL_ANI_INVISIBLE;
				else if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_3)
					ani = MARIOTAIL_ANI_7_LEFT;
			}
			else
			{
				//if (_Mario->canFlyX == false)
				{
					ani = MARIOTAIL_ANI_6_LEFT;
					if (_Mario->ani == MARIO_ANI_TAIL_IDLE_LEFT)
						ani = MARIOTAIL_ANI_6_LEFT;
					else if (_Mario->ani == MARIO_ANI_TAIL_WALKING_LEFT || _Mario->isHolding == true)
						ani = MARIOTAIL_ANI_WALKING_LEFT;
					else if (_Mario->ani == MARIO_ANI_TAIL_JUMP_LEFT)
						ani = MARIOTAIL_ANI_7_LEFT;
					else if (_Mario->ani == MARIO_ANI_TAIL_FALLING_LEFT)
						ani = MARIOTAIL_ANI_1_LEFT;
					else if (_Mario->ani == MARIO_ANI_TAIL_SITDOWN_LEFT)
						ani = MARIOTAIL_ANI_0_LEFT;
					else if (_Mario->isFlyingLow == true)
						ani = MARIOTAIL_ANI_FLYINGLOW_LEFT;
					if (_Mario->canFlyS == true || _Mario->ani == MARIO_ANI_TAIL_STOP_LEFT || _Mario->ani == MARIO_ANI_TAIL_RUNNING_LEFT || _Mario->ani == MARIO_ANI_TAIL_KICK_LEFT)
						ani = MARIOTAIL_ANI_INVISIBLE;
				}
			}
		}
	}
	//DebugOut(L"RENDER Tail ani = %i\n", ani);
	animation_set->at(ani)->Render(x, y, 255);
	RenderBoundingBox();
}

void MarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (isInvisible == false)
	{
		right = x + MARIO_TAIL_WIDTH;
		bottom = y + MARIO_TAIL_HEIGHT;
	}
	else
	{
		right = x;
		bottom = y;
	}
}