#include "MarioTail.h"
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
}
void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*CPlayScene* playscene = CPlayScene::GetInstance();
	Mario* player = playscene->GetPlayer();*/
	//DebugOut(L"x %f\n", player->x);
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
	RenderBoundingBox();
}
