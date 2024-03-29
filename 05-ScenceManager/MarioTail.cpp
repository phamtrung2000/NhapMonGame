﻿#include "MarioTail.h"
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
#include"QuestionBrickItem.h"
#include "RedFlyKoopas.h"
#include "BoomerangEnemy.h"
#include "FirePiranhaPlant.h"
#include "ListItemBrick.h"
#include "ListQuestionBrick.h"
#include "RedGoomba.h"

MarioTail::MarioTail(float x, float y)
{
	ObjType = OBJECT_TYPE_MARIO_TAIL;
	vx = vy = 0;
	SetPosition(x, y);
	Category = CATEGORY::WEAPON;
	isInvisible = attack = false;
	IsMovingObject = true;

}

void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (_Mario->isLevelUp == true)
	{
		_Mario->ChangeLevelTime = GetTickCount64();
		auto effect = new EffectSmoke(_Mario->x, _Mario->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
		_Grid->AddStaticObject(effect, _Mario->x, _Mario->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
		this->canDelete = true;
		_PlayScene->Stop = true;
		return;
	}
	else if (_Mario->isLevelDown == true)
	{
		_Mario->ChangeLevelTime = GetTickCount64();
		auto effect = new EffectSmoke(_Mario->x, _Mario->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
		_Grid->AddStaticObject(effect, _Mario->x, _Mario->y + (MARIO_TAIL_BBOX_HEIGHT / 5));
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
				//this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 5;
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 4;
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
			if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true && this->attack == false)
			{
				if (coObjects->at(i)->Category == CATEGORY::OBJECT)
				{
					if (coObjects->at(i)->ObjType == OBJECT_TYPE_ITEMBRICK)
					{
						ItemBrick* brick = dynamic_cast<ItemBrick*>(coObjects->at(i));
						if (brick->Item == NORMAL)
						{
							brick->SetState(ITEMBRICK_STATE_DIE);
						}
						else if (brick->GetState() == BRICK_STATE_NORMAL)
						{
							brick->SetState(BRICK_STATE_COLLISION);
							brick->hasItem = false;
							if (brick->Item == BUTTONP)
							{
								BrickItem* brickitem = new BrickItem(BUTTONP, brick->x, brick->y - 16);
								_Grid->AddStaticObject(brickitem, brick->x, brick->y - 16);
								auto effect = new EffectSmoke(brick->x, brick->y - 16);
								_Grid->AddStaticObject(effect, brick->x, brick->y - 16);
							}
							else
							{
								BrickItem* brickitem = new BrickItem(MUSHROOM, brick->x, brick->y - 3);
								_Grid->AddMovingObject(brickitem, brick->x, brick->y - 3);
							}
						}
					}
					else if (coObjects->at(i)->ObjType == OBJECT_TYPE_QUESTIONBRICK)
					{
						QuestionBrick* brick = dynamic_cast<QuestionBrick*>(coObjects->at(i));
						if (brick->GetState() == BRICK_STATE_NORMAL)
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
									brick->Item = QUESTIONBRICKITEM_LEAF;
									break;
								}
							}
							QuestionBrickItem* questionbrickitem = new QuestionBrickItem(brick->Item, brick->x, brick->y - 3);
							_Grid->AddMovingObject(questionbrickitem, brick->x, brick->y - 3);
						}
					}
					else if (coObjects->at(i)->ObjType == OBJECT_TYPE_LISTITEMBRICK)
					{
						ListItemBrick* listbrick = dynamic_cast<ListItemBrick*>(coObjects->at(i));
						if (listbrick->Bricks.size() == 1)
						{
							listbrick->DeleteBrick(0);
						}
						else if (listbrick->Bricks.size() > 1)
						{
							int direction = 0;
							if(_Mario->endAttack == false)
								direction = _Mario->nx;
							if (direction == LEFT)
							{
								if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
									listbrick->DeleteBrick(listbrick->Bricks.size() - 1);
							}
							else
							{
								if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
									listbrick->DeleteBrick(0);
							}
						}
						
					}
					else if (coObjects->at(i)->ObjType == OBJECT_TYPE_LISTQUESTIONBRICK)
					{
						ListQuestionBrick* listbrick = dynamic_cast<ListQuestionBrick*>(coObjects->at(i));
						if (listbrick->Bricks.size() == 1)
						{
							listbrick->DeleteBrick(0);
						}
						else if (listbrick->Bricks.size() > 1)
						{
							int direction = 0;
							if (_Mario->endAttack == false)
								direction = _Mario->nx;
							if (direction == LEFT)
							{
								if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
									listbrick->DeleteBrick(listbrick->Bricks.size() - 1);
							}
							else
							{
								if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
									listbrick->DeleteBrick(0);
							}
						}

					}
					attack = true;
				}
				else if (coObjects->at(i)->Category == CATEGORY::ENEMY)
				{
					Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));
					if (enemy->isAttacked == false && enemy->EnemyType!= ENEMY_TYPE_PLANT)
					{
						auto hit = new EffectHit(enemy->x, enemy->y, TYPE_TAIL);
						_Grid->AddMovingObject(hit, enemy->x, enemy->y);
						enemy->isAttacked = true;
						enemy->Time_isAttacked = GetTickCount64();
						if (enemy->EnemyType != ENEMY_TYPE_KOOPAS)
						{
							_Mario->nScore++;
							_HUD->UpdateScore(enemy, _Mario->nScore);
						}
					}
					if (enemy->EnemyType == ENEMY_TYPE_GOOMBA)
					{
						if (enemy->ObjType == OBJECT_TYPE_GOOMBA)
						{
							Goomba* goomba = dynamic_cast<Goomba*>(coObjects->at(i));
							if (goomba->isDie == false)
							{
								goomba->nx = _Mario->nx;
								goomba->SetState(ENEMY_STATE_DIE_IS_ATTACKED);
							}
						}
						else if (enemy->ObjType == OBJECT_TYPE_REDGOOMBA)
						{
							RedGoomba* goomba = dynamic_cast<RedGoomba*>(coObjects->at(i));
							if (goomba->isDie == false)
							{
								goomba->nx = _Mario->nx;
								goomba->SetState(ENEMY_STATE_DIE_IS_ATTACKED);
							}
						}
					}
					else if(coObjects->at(i)->ObjType == OBJECT_TYPE_KOOPAS || coObjects->at(i)->ObjType == OBJECT_TYPE_GREENKOOPAS)					
					{
						Koopas* koopas = dynamic_cast<Koopas*>(coObjects->at(i));
						koopas->vy = -ENEMY_DIE_DEFLECT_SPEED;
						koopas->OnGroud = false;
						koopas->isKicked = false; // reset lại biến iskicked để tránh TH k giảm tốc sau khi đá (iskicked = true)
						if (_Mario->x <= koopas->x)
						{
							koopas->SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
						}
						else
						{
							koopas->SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
						}
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
					else if (coObjects->at(i)->ObjType == OBJECT_TYPE_REDFLYKOOPAS)
					{
						RedFlyKoopas* koopas = dynamic_cast<RedFlyKoopas*>(coObjects->at(i));
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
					else if (coObjects->at(i)->ObjType == OBJECT_TYPE_BOOMERANGENEMY)
					{
						BoomerangEnemy* boom = dynamic_cast<BoomerangEnemy*>(coObjects->at(i));
						if (boom->isDie == false)
						{
							boom->nx = _Mario->nx;
							boom->vy = -0.2f;
							boom->SetState(ENEMY_STATE_DIE_IS_ATTACKED);
							
						}
					}
					else if (enemy->EnemyType == ENEMY_TYPE_PLANT)
					{
						GreenPlant* plant = dynamic_cast<GreenPlant*>(enemy);
						if (plant->GetState() != FIREPIRANHAPLANT_STATE_HIDE)
						{
							auto hit = new EffectHit(plant->x, plant->y, TYPE_TAIL);
							_Grid->AddStaticObject(hit, plant->x, plant->y);
							plant->isAttacked = true;
							plant->Time_isAttacked = GetTickCount64();
							_Mario->nScore++;
							_HUD->UpdateScore(enemy, _Mario->nScore);
							plant->canDelete = true;
						}
					}
					else
						coObjects->at(i)->canDelete = true;
				}
			}
		}
	}
	else
	{
	attack = false;
	}
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
		if (_Mario->canFlyX == true || _Mario->canFlyS == true)
		{
			// chưa nhảy // đang rớt xuống
			if (_Mario->vy >= 0)
			{
				// đang chạm đất
				if (_Mario->OnGround == true)
				{
					// đứng yên
					if ( _Mario->vx == 0)
					{
						// phải 
						if (_Mario->nx == RIGHT)
						{
							ani = MARIOTAIL_ANI_WALKING_RIGHT;
						}
						// trái
						else
						{
							ani = MARIOTAIL_ANI_WALKING_LEFT;
						}
					}
					// đi bộ qua phải
					else if (_Mario->nx == RIGHT)
					{
						if (_Mario->ChangeDirection == false)
						{
							ani = MARIOTAIL_ANI_WALKING_RIGHT;
						}
						else if (_Mario->ChangeDirection == true)
							ani = MARIOTAIL_ANI_INVISIBLE;
					}
					//  đi bộ qua trái
					else if (_Mario->nx == LEFT)
					{
						if (_Mario->ChangeDirection == false)
						{
							ani = MARIOTAIL_ANI_WALKING_LEFT;
						}
						else if (_Mario->ChangeDirection == true)
							ani = MARIOTAIL_ANI_INVISIBLE;
					}
				}
				// rớt xuống
				else
				{
					// phải 
					if (_Mario->nx == RIGHT)
					{
						ani = MARIOTAIL_ANI_7_RIGHT;
					}
					// trái
					else
					{
						ani = MARIOTAIL_ANI_INVISIBLE;
					}
				}

			}
			// nhảy
			else
			{
				// phải 
				if (_Mario->nx == RIGHT)
				{
					if (_Mario->isMaxRunning == true)
					{
						if (_Mario->isFlyingHigh == true)
							ani = MARIOTAIL_ANI_INVISIBLE;
						else
							ani = MARIOTAIL_ANI_INVISIBLE;
					}
				}
				// trái
				else
				{
					if (_Mario->isMaxRunning == true)
					{
						if (_Mario->isFlyingHigh == true)
							ani = MARIOTAIL_ANI_INVISIBLE;
						else
							ani = MARIOTAIL_ANI_INVISIBLE;
					}
					else
						ani = MARIOTAIL_ANI_INVISIBLE;
				}

			}

			// đá
			if (_Mario->canKick == true)
			{
				if (_Mario->nx == RIGHT)
					ani = MARIOTAIL_ANI_6_RIGHT;
				else
					ani = MARIOTAIL_ANI_6_LEFT;
			}
			else if (_Mario->GoHiddenWorld == true)
				ani = MARIOTAIL_ANI_INVISIBLE;
			else if (_Mario->isAttacking == true && _Mario->endAttack == false)
			{
				if (_Mario->nx == RIGHT)
				{
					if (_Mario->time_attack <= TIME_ATTACK)
						ani = MARIOTAIL_ANI_7_RIGHT;
					else if (_Mario->time_attack > TIME_ATTACK && _Mario->time_attack <= 2 * TIME_ATTACK)
						ani = MARIOTAIL_ANI_INVISIBLE;
					else if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
						ani = MARIOTAIL_ANI_7_LEFT;
					else if (_Mario->time_attack > 3 * TIME_ATTACK && _Mario->time_attack <= 4 * TIME_ATTACK)
						ani = MARIOTAIL_ANI_INVISIBLE;
					else if (_Mario->time_attack > 4 * TIME_ATTACK && _Mario->time_attack <= 5 * TIME_ATTACK)
						ani = MARIOTAIL_ANI_7_RIGHT;
				}
				else
				{
					if (_Mario->time_attack <= TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_7_LEFT;
					}
					else if (_Mario->time_attack > TIME_ATTACK && _Mario->time_attack <= 2 * TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_INVISIBLE;
					}
					else if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_7_RIGHT;
					}
					else if (_Mario->time_attack > 3 * TIME_ATTACK && _Mario->time_attack <= 4 * TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_INVISIBLE;
					}
					else if (_Mario->time_attack > 4 * TIME_ATTACK && _Mario->time_attack <= 5 * TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_7_LEFT;
					}
				}
			}
			else if (_Mario->isSitDown == true)
			{
				if (_Mario->nx == RIGHT)
					ani = MARIOTAIL_ANI_0_RIGHT;
				else
					ani = MARIOTAIL_ANI_0_LEFT;
			}
			else if (_Mario->isHolding == true)
			{
				if (_Mario->nx == RIGHT)
				{
					if (vx == 0)
					{
						ani = MARIOTAIL_ANI_6_RIGHT;
					}
					else
						ani = MARIOTAIL_ANI_WALKING_RIGHT;
					if (_Mario->OnGround == false)
						ani = MARIOTAIL_ANI_6_RIGHT;
				}
				else
				{
					if (vx == 0)
					{
						ani = MARIOTAIL_ANI_6_LEFT;
					}
					else
						ani = MARIOTAIL_ANI_WALKING_LEFT;
					if (_Mario->OnGround == false)
						ani = MARIOTAIL_ANI_6_LEFT;
				}
			}
		}
		else
		{
			// chưa nhảy // đang rớt xuống
			if (_Mario->vy >= 0)
			{
				// đang chạm đất
				if (_Mario->_Mario->OnGround == true)
				{
					// đứng yên
					if (_Mario->vx == 0)
					{
						if (_Mario->ChangeDirection == false)
						{
							// phải 
							if (_Mario->nx == RIGHT)
							{
								if (_Mario->isMaxRunning == true)
									ani = MARIOTAIL_ANI_INVISIBLE;
								else 
									ani = MARIOTAIL_ANI_6_RIGHT;
								if (_Mario->isHolding == true)
									ani = MARIOTAIL_ANI_WALKING_RIGHT;
							}
							// trái
							else
							{
								if (_Mario->isMaxRunning == true)
									ani = MARIOTAIL_ANI_INVISIBLE;
								else
									ani = MARIOTAIL_ANI_6_LEFT;
								if (_Mario->isHolding == true)
									ani = MARIOTAIL_ANI_WALKING_LEFT;
							}
						}
						else
						{
							if (_Mario->nx == LEFT)
								ani = MARIOTAIL_ANI_INVISIBLE;
							else
								ani = MARIOTAIL_ANI_INVISIBLE;
						}


					}
					// đi bộ qua phải
					else if (_Mario->nx == RIGHT)
					{
						if (_Mario->ChangeDirection == false)
						{
							ani = MARIOTAIL_ANI_WALKING_RIGHT;
							if (_Mario->isHolding == true)
								ani = MARIOTAIL_ANI_WALKING_RIGHT;
							else if (_Mario->isMaxRunning == true)
								ani = MARIOTAIL_ANI_INVISIBLE;
						}
						else if (_Mario->ChangeDirection == true)
							ani = MARIOTAIL_ANI_INVISIBLE;

					}
					//  đi bộ qua trái
					else if ( _Mario->nx == LEFT)
					{
						if (_Mario->ChangeDirection == false)
						{
							ani = MARIOTAIL_ANI_WALKING_LEFT;
							if (_Mario->isHolding == true)
								ani = MARIOTAIL_ANI_WALKING_LEFT;
							else if (_Mario->isMaxRunning == true)
								ani = MARIOTAIL_ANI_INVISIBLE;
						}
						else if (_Mario->ChangeDirection == true)
							ani = MARIOTAIL_ANI_INVISIBLE;

					}
				}
				// rớt xuống
				else
				{
					if (_Mario->isHolding == true)
					{
						// phải 
						if (_Mario->nx == RIGHT)
							ani = MARIOTAIL_ANI_WALKING_RIGHT;
						// trái
						else
							ani = MARIOTAIL_ANI_WALKING_LEFT;
					}
					else
					{
						// phải 
						if (_Mario->nx == RIGHT)
						{
							ani = MARIOTAIL_ANI_1_RIGHT;
							if (_Mario->isFlyingLow == true)
								ani = MARIOTAIL_ANI_FLYINGLOW_RIGHT;
						}
						// trái
						else
						{
							ani = MARIOTAIL_ANI_1_LEFT;
							if (_Mario->isFlyingLow == true)
								ani = MARIOTAIL_ANI_FLYINGLOW_LEFT;
						}
					}
				}
			}
			// nhảy
			else
			{
				if (_Mario->isHolding == true)
				{
					// phải 
					if (_Mario->nx == RIGHT)
						ani = MARIOTAIL_ANI_7_RIGHT;
					// trái
					else
						ani = MARIOTAIL_ANI_7_LEFT;
				}
				else
				{
					// phải 
					if (_Mario->nx == RIGHT)
					{
						ani = MARIOTAIL_ANI_7_RIGHT;
					}
					// trái
					else
					{
						ani = MARIOTAIL_ANI_7_LEFT;
					}
				}
			}

			// đá
			if (_Mario->canKick == true)
			{
				if (_Mario->nx == RIGHT)
					ani = MARIOTAIL_ANI_6_RIGHT;
				else
					ani = MARIOTAIL_ANI_6_LEFT;
			}
			else if (_Mario->GoHiddenWorld == true)
				ani = MARIOTAIL_ANI_INVISIBLE;
			else if (_Mario->isAttacking == true && _Mario->endAttack == false)
			{
				if (_Mario->nx == RIGHT)
				{
					if (_Mario->time_attack <= TIME_ATTACK)
						ani = MARIOTAIL_ANI_7_RIGHT;
					else if (_Mario->time_attack > TIME_ATTACK && _Mario->time_attack <= 2 * TIME_ATTACK)
						ani = MARIOTAIL_ANI_INVISIBLE;
					else if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
						ani = MARIOTAIL_ANI_7_LEFT;
					else if (_Mario->time_attack > 3 * TIME_ATTACK && _Mario->time_attack <= 4 * TIME_ATTACK)
						ani = MARIOTAIL_ANI_INVISIBLE;
					else if (_Mario->time_attack > 4 * TIME_ATTACK && _Mario->time_attack <= 5 * TIME_ATTACK)
						ani = MARIOTAIL_ANI_7_RIGHT;
				}
				else
				{
					if (_Mario->time_attack <= TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_7_LEFT;
					}
					else if (_Mario->time_attack > TIME_ATTACK && _Mario->time_attack <= 2 * TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_INVISIBLE;
					}
					else if (_Mario->time_attack > 2 * TIME_ATTACK && _Mario->time_attack <= 3 * TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_7_RIGHT;
					}
					else if (_Mario->time_attack > 3 * TIME_ATTACK && _Mario->time_attack <= 4 * TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_INVISIBLE;
					}
					else if (_Mario->time_attack > 4 * TIME_ATTACK && _Mario->time_attack <= 5 * TIME_ATTACK)
					{
						ani = MARIOTAIL_ANI_7_LEFT;
					}
				}
			}
			else if (_Mario->isSitDown == true)
			{
				if (_Mario->nx == RIGHT)
					ani = MARIOTAIL_ANI_0_RIGHT;
				else
					ani = MARIOTAIL_ANI_0_LEFT;
			}
			else if (_Mario->isHolding == true)
			{
				if (_Mario->nx == RIGHT)
				{
					if (vx == 0)
					{
						ani = MARIOTAIL_ANI_6_RIGHT;
					}
					else
						ani = MARIOTAIL_ANI_6_RIGHT;
					if (_Mario->_Mario->OnGround == false)
						ani = MARIOTAIL_ANI_6_RIGHT;
				}
				else
				{
					if (vx == 0)
					{
						ani = MARIOTAIL_ANI_6_LEFT;
					}
					else
						ani = MARIOTAIL_ANI_6_LEFT;
					if (_Mario->_Mario->OnGround == false)
						ani = MARIOTAIL_ANI_6_LEFT;
				}
			}
		}
	}

	if (_Mario->isLevelDown == true)
	{
		if (_Mario->untouchable == true)
		{
			if (_Mario->UntouchtableTime % 3 == 0)
				animation_set->at(ani)->Render(x, y, 32);
			else
				animation_set->at(ani)->Render(x, y, 255);
		}
		else
			animation_set->at(ani)->Render(x, y, 128);
	}
	else
	{
		animation_set->at(ani)->Render(x, y, 255);
	}
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