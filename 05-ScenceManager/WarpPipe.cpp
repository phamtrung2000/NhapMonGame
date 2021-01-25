#include "WarpPipe.h"
#include "PlayScence.h"
#define COLOR_GREEN	0
#define COLOR_BLACK	1

#define Ani_Start_Right		0
#define Ani_Start_LEFT		1

#define Ani_Body_Left		2
#define Ani_Body_Right		3

#define WARPPIPEBLACK_ANI_START_RIGHT		4
#define BLACKAni_Start_LEFT		5

#define BLACKAni_Body_Left		6
#define BLACKAni_Body_Right		7

WarpPipe::WarpPipe(int W, int H)
{
	HiddenWorld = false;
	Width = W;
	Height = H;
	ObjType = OBJECT_TYPE_WARPPIPE;
	Category = CATEGORY::OBJECT;
	Color = COLOR_GREEN;
}

WarpPipe::WarpPipe(int W, int H,bool a)
{
	HiddenWorld = a;
	Width = W;
	Height = H;
	ObjType = OBJECT_TYPE_WARPPIPE;
	Category = CATEGORY::OBJECT;
	Color = COLOR_GREEN;
}

WarpPipe::WarpPipe(int W, int H, int color,int scene)
{
	Width = W;
	Height = H;
	ObjType = OBJECT_TYPE_WARPPIPE;
	Category = CATEGORY::OBJECT;
	Color = color;
	SceneID = scene;
	if (scene == 0)
		HiddenWorld = false;
	else
		HiddenWorld = true;
}

void WarpPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + Width;
	bottom = top + Height;
}

void WarpPipe::Render()
{
	if (Color == COLOR_GREEN)
	{
		int Y = y + OBJECT_BBOX_WIDTH_HEIGHT;
		for (int i = 0; i < (Height / OBJECT_BBOX_WIDTH_HEIGHT) * 2; i++)
		{
			// vẽ phần đầu
			if (i < 2)
				animation_set->at(i)->Render(i * OBJECT_BBOX_WIDTH_HEIGHT + x, y);
			// vẽ bên trái, i = 2, 4 , 6
			else if (i % 2 == 0)
			{
				animation_set->at(Ani_Body_Left)->Render(x, Y);
			}
			//vẽ bên phải
			else
			{
				animation_set->at(Ani_Body_Right)->Render(x + OBJECT_BBOX_WIDTH_HEIGHT, Y);
				Y += OBJECT_BBOX_WIDTH_HEIGHT;
			}
		}
	}
	else
	{
		//for (int i = 0; i < (Height / OBJECT_BBOX_WIDTH_HEIGHT) * 2; i++)
		//{
		//	// vẽ phần đầu
		//	if (i < 2)
		//		animation_set->at(i + 4)->Render(i * OBJECT_BBOX_WIDTH_HEIGHT + x, y);
		//	// vẽ bên trái, i = 2, 4 , 6
		//	else if (i % 2 == 0)
		//	{
		//		animation_set->at(BLACKAni_Body_Left)->Render(x, Y);
		//	}
		//	//vẽ bên phải
		//	else
		//	{
		//		animation_set->at(BLACKAni_Body_Right)->Render(x + OBJECT_BBOX_WIDTH_HEIGHT, Y);
		//		Y += OBJECT_BBOX_WIDTH_HEIGHT;
		//	}
		//}
		//int Y = y + OBJECT_BBOX_WIDTH_HEIGHT;

		int Y = y;
		for (int i = 0; i < (Height / OBJECT_BBOX_WIDTH_HEIGHT) * 2; i++)
		{
			// vẽ phần đầu
			if (i < (Height / OBJECT_BBOX_WIDTH_HEIGHT) * 2 - 2)
			{
				// vẽ bên trái, i = 2, 4 , 6
				if (i % 2 == 0)
				{
					animation_set->at(BLACKAni_Body_Left)->Render(x +1 , y);
				}
				//vẽ bên phải
				else
				{
					animation_set->at(BLACKAni_Body_Right)->Render(x  + OBJECT_BBOX_WIDTH_HEIGHT, Y);
					Y += OBJECT_BBOX_WIDTH_HEIGHT;
				}
			}
			else
				animation_set->at(i + 2)->Render((i - 2) * OBJECT_BBOX_WIDTH_HEIGHT + x, Y);
			
		}
	}
	//RenderBoundingBox();
}
