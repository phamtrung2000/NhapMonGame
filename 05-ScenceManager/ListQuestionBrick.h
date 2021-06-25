#pragma once
#include "GameObject.h"
#include "QuestionBrick.h"
#include "Game.h"

class ListQuestionBrick : public CGameObject
{
public:
	int NumberBrick;
	vector<QuestionBrick*>Bricks;
	ListQuestionBrick()
	{
		NumberBrick = 0;
		Width = NumberBrick * 16;
		Height = 16;
		ObjType = OBJECT_TYPE_LISTQUESTIONBRICK;
		Category = CATEGORY::OBJECT;
	}
	ListQuestionBrick(int number, vector<int>list, float startx, float starty)
	{
		NumberBrick = number;
		Width = NumberBrick * 16;
		Height = 16;
		ObjType = OBJECT_TYPE_LISTQUESTIONBRICK;
		Category = CATEGORY::OBJECT;
		StartX = startx;
		StartY = starty;
		for (int i = 0; i < NumberBrick; i++)
		{
			int type = list.at(i);
			QuestionBrick* brick = new QuestionBrick(type, StartX + i * 16, StartY);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_QUESTIONBRICK);
			brick->SetAnimationSet(ani_set);
			//_PlayScene->objects.push_back(brick);
			Bricks.push_back(brick);
		}
	}
	ListQuestionBrick(vector<QuestionBrick*>list, int vitridau, int vitricuoi)
	{
		NumberBrick = vitricuoi - vitridau + 1;
		Width = NumberBrick * 16;
		Height = 16;
		ObjType = OBJECT_TYPE_LISTQUESTIONBRICK;
		Category = CATEGORY::OBJECT;
		for (int i = vitridau; i <= vitricuoi; i++)
		{
			QuestionBrick* brick = list.at(i);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_QUESTIONBRICK);
			brick->SetAnimationSet(ani_set);
			Bricks.push_back(brick);
		}
		QuestionBrick* firstbrick = Bricks.front();
		x = firstbrick->x;
		y = firstbrick->y;
	}

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void DeleteBrick(int vitri);
	int ViTriGachVaCham(float x, int width);
};

