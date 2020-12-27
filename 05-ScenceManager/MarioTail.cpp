﻿#include "MarioTail.h"
#include "Goomba.h"
#include "Game.h"
#include "Koopas.h"
#include"PlayScence.h"
#include "ItemBrick.h"
#include "Mario.h"

MarioTail::MarioTail(float x, float y)
{
	ObjType = OBJECT_TYPE_MARIO_TAIL;
	vx = vy = 0;
	SetPosition(x, y);
	Category = CATEGORY::WEAPON;
	width = 6;
	height = 19;
}
void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (_Mario->nx == RIGHT )
	{
		/*if (_Mario->ani == MARIO_ANI_TAIL_JUMP_RIGHT )
		{
			width = 7;
			height = 6;
		}
		else if (_Mario->ani == MARIO_ANI_TAIL_FALLING_RIGHT)
		{
			width = 7;
			height = 9;
		}
		else if (_Mario->ani == MARIO_ANI_TAIL_IDLE_RIGHT)
		{
			width = 5;
			height = 8;
		}
		else if (_Mario->ani == MARIO_ANI_TAIL_WALKING_RIGHT)
		{
			width = 6;
			height = 8;
		}
		else 
		{
			width = 6;
			height = 7;
		}*/
		
		if (_Mario->isAttacking == false)
		{
			this->x = _Mario->x - 5;
			if (_Mario->ani == MARIO_ANI_TAIL_JUMP_RIGHT)
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 3;
			else if (_Mario->ani == MARIO_ANI_TAIL_FALLING_RIGHT)
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 5;
			else if ( _Mario->isSitDown==true)
			{
				this->x = _Mario->x - 7;
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 5;
			}
			else if (_Mario->isFlyingLow == true)
			{
				this->x = _Mario->x - 6;
				this->y = _Mario->y + MARIO_TAIL_BBOX_SITDOWN_HEIGHT - 1;
			}
			else 
				this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 2;
		}
		else
		{
			this->y = _Mario->y + MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_HEIGHT - 3;
			if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_1)
				x = _Mario->x - 7.0f;
			else if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_3)
				x = _Mario->x + MARIO_TAIL_BBOX_WIDTH;
		}
	}
	else if (_Mario->nx == LEFT )
	{
		/*if (_Mario->ani == MARIO_ANI_TAIL_JUMP_RIGHT)
		{
			width = 7;
			height = 6;
		}
		else if (_Mario->ani == MARIO_ANI_TAIL_FALLING_RIGHT)
		{
			width = 7;
			height = 9;
		}
		else if (_Mario->ani == MARIO_ANI_TAIL_IDLE_RIGHT)
		{
			width = 5;
			height = 8;
		}
		else
		{
			width = 6;
			height = 7;
		}*/

		if (_Mario->isAttacking == false)
		{
			this->x = _Mario->x + MARIO_BIG_BBOX_WIDTH + 2;
			if (_Mario->ani != MARIO_ANI_TAIL_JUMP_LEFT && _Mario->ani != MARIO_ANI_TAIL_FALLING_LEFT)
				this->SetPosition(_Mario->x + MARIO_BIG_BBOX_WIDTH + 2, _Mario->y + MARIO_BIG_BBOX_HEIGHT - 7);
			else if (_Mario->ani == MARIO_ANI_TAIL_JUMP_LEFT)
				this->SetPosition(_Mario->x + MARIO_BIG_BBOX_WIDTH + 2, _Mario->y + MARIO_BIG_BBOX_HEIGHT - 9);
			else if (_Mario->ani == MARIO_ANI_TAIL_FALLING_LEFT)
				this->SetPosition(_Mario->x + MARIO_BIG_BBOX_WIDTH + 2, _Mario->y + MARIO_BIG_BBOX_HEIGHT - 11);
		}
		else
		{
			if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_1)
				this->SetPosition(_Mario->x - 7.0f, _Mario->y + 18);
			else if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_3)
				this->SetPosition(_Mario->x + MARIO_TAIL_BBOX_WIDTH, _Mario->y + 18);
		}
	}


	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// lấy render box của 2 obj để kiểm tra xem chúng có nằm bên trong nhau hay không
		if (IsCollision(this->GetRect(), coObjects->at(i)->GetRect()) == true )
		{
			if (coObjects->at(i)->ObjType == OBJECT_TYPE_ITEMBRICK)
			{
				coObjects->at(i)->isDie = true;
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
				if (coObjects->at(i)->ObjType == OBJECT_TYPE_GOOMBA)
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(coObjects->at(i));
					if (goomba->GetState() == GOOMBA_STATE_WALKING)
					{
						goomba->SetState(GOOMBA_STATE_DIE_2);
					}
				}
				//coObjects->at(i)->isDie = true;
			}
		}
	}
}

void MarioTail::Render()
{
	int ani = -1;
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
			if (_Mario->canFlyX == false)
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
				else if (_Mario->ani == MARIO_ANI_TAIL_STOP_RIGHT || _Mario->ani == MARIO_ANI_TAIL_RUNNING_RIGHT)
					ani = MARIOTAIL_ANI_INVISIBLE;
				else if (_Mario->ani == MARIO_ANI_TAIL_SITDOWN_RIGHT)
					ani = MARIOTAIL_ANI_0_RIGHT;
				else if (_Mario->isFlyingLow == true)
					ani = MARIOTAIL_ANI_FLYINGLOW_RIGHT;
				if(_Mario->canFlyS == true)
					ani = MARIOTAIL_ANI_INVISIBLE;
			}
			else
			{
				ani = MARIOTAIL_ANI_INVISIBLE;
			}
			
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
			if (_Mario->canFlyX == false)
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
				else if (_Mario->ani == MARIO_ANI_TAIL_STOP_LEFT || _Mario->ani == MARIO_ANI_TAIL_RUNNING_LEFT)
					ani = MARIOTAIL_ANI_INVISIBLE;
				else if (_Mario->ani == MARIO_ANI_TAIL_SITDOWN_LEFT)
					ani = MARIOTAIL_ANI_0_LEFT;
				else if (_Mario->isFlyingLow == true)
					ani = MARIOTAIL_ANI_FLYINGLOW_LEFT;
				if (_Mario->canFlyS == true)
					ani = MARIOTAIL_ANI_INVISIBLE;
			}
			else
			{
				ani = MARIOTAIL_ANI_INVISIBLE;
			}

		}
	}
	animation_set->at(ani)->Render(x, y, 255);
	DebugOut(L"RENDER Tail ani = %i\n", ani);
	RenderBoundingBox();
}
