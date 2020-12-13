#include "FireBullet.h"
#include "Block.h"
#include "Goomba.h"
#include "WarpPipe.h"
#include "Game.h"
#include "Koopas.h"
void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// đầu màn hình
	float cam_x = CGame::GetInstance()->GetCamX();
	// chiều dài màn hình
	int cam_w = CGame::GetInstance()->GetScreenWidth();

	// ra khỏi camera -> delete
	if (x > cam_x + static_cast<float>(cam_w) || x < cam_x)
		isDie = true;

	CGameObject::Update(dt);

	if (FireMario)
	{
		vy += FIREBULLET_GRAVITY * dt;

		if (y >= 500)
			isDie = true;
		if (nx = -1 && x < 0)
		{
			isDie = true;
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

			x += min_tx * dx + nx * 0.4f;
			//y += min_ty * dy + ny * 0.4f;

			if (ny != 0) vy = 0;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				CATEGORY category = e->obj->Category;
				switch (category)
				{
					case CATEGORY::ENEMY:
					{
						if (dynamic_cast<CGoomba*>(e->obj))
						{
							CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
							if (e->nx != 0 || e->ny != 0)
							{
								if (goomba->GetState() != GOOMBA_STATE_DIE)
									goomba->SetState(GOOMBA_STATE_DIE_2);

								this->isDie = true;
							}
						}
						else if (dynamic_cast<Koopas*>(e->obj))
						{
							Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
							if (e->nx != 0 || e->ny != 0)
							{
								if (koopas->GetState() != KOOPAS_STATE_DIE)
									koopas->SetState(KOOPAS_STATE_DIE);

								this->isDie = true;
								vx = 0;
								vy = 0;
							}
						}
						else
						{
							e->obj->isDie = this->isDie = true;
						}
							
					}break;

					case CATEGORY::OBJECT:
					{
						if (e->nx != 0)
						{
							if (dynamic_cast<Block*>(e->obj))
							{
								x += dx;
							}
							else
								this->isDie = true;
						}
						else
							vy = -0.15f;
					}break;

					case CATEGORY::GROUND:
					{
						vy = -0.15f;
					}break;

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

	RenderBoundingBox();
}
