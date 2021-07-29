#include "ListQuestionBrick.h"
#include "QuestionBrickItem.h"
#include "PlayScence.h"

void ListQuestionBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}

void ListQuestionBrick::Render()
{
	for (unsigned int i = 0; i < Bricks.size(); i++)
	{
		Bricks.at(i)->Render();
	}
}

void ListQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Bricks.size() == 0)
	{
		canDelete = true;
		return;
	}
	QuestionBrick* firstbrick = Bricks.front();
	x = firstbrick->x;
	Width = Bricks.size() * 16;
	for (unsigned int i = 0; i < Bricks.size(); i++)
	{
		Bricks.at(i)->Update(dt, coObjects);
	}

}

void ListQuestionBrick::DeleteBrick(int vitri)
{
	QuestionBrick* brick = Bricks.at(vitri);
	if (brick->hasItem == true && brick->isCollision == false)
	{
		brick->SetState(BRICK_STATE_COLLISION);
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
			QuestionBrickItem* questionbrickitem = new QuestionBrickItem(brick->Item, brick->x, brick->y - 3);
			_Grid->AddMovingObject(questionbrickitem, brick->x, brick->y - 3);
		}
		else
		{
			QuestionBrickItem* questionbrickitem = new QuestionBrickItem(brick->Item, brick->x + 1, brick->y - 3);
			_Grid->AddMovingObject(questionbrickitem, brick->x + 1, brick->y - 3);
		}
	}
}

int ListQuestionBrick::ViTriGachVaCham(float Obj_x, int width)
{
	if (Bricks.size() == 1)
	{
		return 0;
	}
	else if (Bricks.size() == 2)
	{
		int vitri = 0;
		if ((Obj_x <= Bricks.at(0)->x) || (Obj_x + width / 2 <= Bricks.at(0)->x + 16 && Obj_x + Width / 2 > Bricks.at(0)->x))// sure đụng viên đầu tiên
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if (Obj_x <= Bricks.at(0)->x) // sure đụng viên đầu tiên
		{
			return 0;
		}
		else if (Obj_x + width >= Bricks.at(Bricks.size() - 1)->x + 16) // sure đụng viên cuối
		{
			return Bricks.size() - 1;
		}
		else
		{
			int vitri = int(Obj_x + width - Bricks.at(0)->x) / 16;
			float tempx = Bricks.at(vitri)->x;
			if (Obj_x < Bricks.at(vitri)->x && tempx - Obj_x > 8)
				vitri--;
			return vitri;
		}
	}
	return 0;
}
