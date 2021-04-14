#include "FireBullet.h"
#include "Block.h"
#include "Goomba.h"
#include "WarpPipe.h"
#include "Game.h"
#include "Koopas.h"
#include "EffectSmoke.h"
#include "EffectHit.h"
#include "PlayScence.h"
#include "GreenFlyKoopas.h"
#include "BoomerangEnemy.h"

FireBullet::FireBullet(float a, float b)
{
	FireMario = canDelete = false;
	ObjType = OBJECT_TYPE_FIREBULLET;
	x = a;
	y = b;
	this->Direction = _Mario->nx;
	Category = CATEGORY::WEAPON;
	SetSpeed(FIREBULLET_VX_SPEED * _Mario->nx, 0);
	IsMovingObject = true;
}

void FireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREBULLET_BBOX;
	bottom = y + FIREBULLET_BBOX;
}

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// đầu màn hình
	float cam_x = CGame::GetInstance()->GetCamX();
	// chiều dài màn hình
	int cam_w = CGame::GetInstance()->GetScreenWidth();

	// ra khỏi camera -> delete
	if (x > cam_x + static_cast<float>(cam_w) || x < cam_x)
		canDelete = true;

	CGameObject::Update(dt);

	if (FireMario == true)
	{
		vy += FIREBULLET_GRAVITY * dt;

		if (y >= 500)
			canDelete = true;
		if (nx = -1 && x < 0)
		{
			canDelete = true;
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
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


			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				
				if (e->obj != NULL)
				{
					switch (e->obj->Category)
					{
					case CATEGORY::ENEMY:
					{
						Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
					
						switch (enemy->EnemyType)
						{

						case ENEMY_TYPE_GOOMBA:
						{
							Goomba* goomba = dynamic_cast<Goomba*>(e->obj);
							if (e->nx != 0 || e->ny != 0)
							{
								if (goomba->GetState() != ENEMY_STATE_DIE_IS_JUMPED)
								{
									goomba->nx = this->Direction;
									goomba->SetState(ENEMY_STATE_DIE_IS_ATTACKED);
									this->canDelete = true;
									auto hit = new EffectHit(e->obj->x, e->obj->y, TYPE_FIREBULLET);
									_PlayScene->objects.push_back(hit);
									_HUD->UpdateScore(goomba, 1);
								}
							}
						}break;

						case ENEMY_TYPE_KOOPAS:
						{
							Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
							if (koopas->GetState() != ENEMY_STATE_DIE_IS_JUMPED)
							{
								if (e->obj->ObjType == OBJECT_TYPE_GREENFLYKOOPAS)
								{
									GreenFlyKoopas* koopas = dynamic_cast<GreenFlyKoopas*>(e->obj);
									koopas->Health--;
								}
								koopas->nx = this->Direction;
								koopas->SetState(ENEMY_STATE_DIE_IS_ATTACKED);
								this->canDelete = true;
								auto hit = new EffectHit(e->obj->x, e->obj->y, TYPE_FIREBULLET);
								_PlayScene->objects.push_back(hit);
								_HUD->UpdateScore(koopas, 1);
								
							}
						}
						break;

						case ENEMY_TYPE_BOOMERANG_ENEMY:
						{
							BoomerangEnemy* boom = dynamic_cast<BoomerangEnemy*>(e->obj);
							if (boom->isDie == false)
							{
								boom->nx = this->Direction;
								boom->SetState(BOOMERANGENEMY_STATE_DIE_2);
								this->canDelete = true;
								auto hit = new EffectHit(e->obj->x, e->obj->y, TYPE_FIREBULLET);
								_PlayScene->objects.push_back(hit);
								_HUD->UpdateScore(boom, 1);
							}
						}
						break;

						case ENEMY_TYPE_PLANT:
						{
							if (e->nx != 0 || e->ny != 0)
							{
								e->obj->canDelete = true;
								this->canDelete = true;
								auto hit = new EffectHit(e->obj->x, e->obj->y, TYPE_FIREBULLET);
								_PlayScene->objects.push_back(hit);
								_HUD->UpdateScore(e->obj, 1);
							}
						}break;

						default:
							e->obj->canDelete = this->canDelete = true;
							break;
						}
					}break;

					case CATEGORY::OBJECT:
					{
					
						if (e->nx != 0)
						{
							y += min_ty * dy + ny * 0.4f;
							if (dynamic_cast<Block*>(e->obj))
							{
								x += dx;
							}
							else
							{
								this->canDelete = true;
								auto effect = new EffectSmoke(this->x, this->y);
								_PlayScene->objects.push_back(effect);
							}
						}
						else if (e->ny < 0)
						{
							x += min_tx * dx + nx * 0.4f;
							vy = -0.15f;
						}
							
					}break;

					default:
					{
						x += dx;
						y += dy;

			//if (ny != 0) vy = 0;
					}break;


					}
				}
				
				
			}
		}
	}
	else
	{
		x += dx;
		y += dy;
	}

	
}

void FireBullet::Render()
{
	int ani = 0;
	if (nx == 1)
	{
		ani = ANI_BULLET_RIGHT;
	}
	else if (nx == -1)
	{
		ani = ANI_BULLET_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
}