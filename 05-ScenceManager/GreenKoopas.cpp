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
#include "QuestionBrickItem.h"
#include "ListItemBrick.h"
#include "ListQuestionBrick.h"
GreenKoopas::GreenKoopas() : Koopas()
{
	ObjType = OBJECT_TYPE_GREENKOOPAS;
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
	Koopas::Update(dt, coObjects);
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
		// lúc mới vào game, rùa sẽ ở trạng thái Init rớt từ trên xuống, khi chạm vật thể thì bắt đầu di chuyển
		// để tránh lỗi vụ rớt lệch khỏi viên gạch
		if (GetState() == ENEMY_STATE_INIT)
			SetState(ENEMY_STATE_WALKING_LEFT);

		if (e->ny < 0)
			OnGroud = true;

		if (e->obj->ObjType == OBJECT_TYPE_GROUND)
		{
			if (isHold == true)
			{
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
					_Grid->AddMovingObject(questionbrickitem, brick->x, brick->y - 3);
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
				if (this->x + this->Width >= brick->x + ITEMBRICK_WIDTH)
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
