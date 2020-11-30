#include "WarpPipe.h"
#include "PlayScence.h"

WarpPipe::WarpPipe(int W, int H)
{
	isCovered = true;
	Width = W;
	Height = H;
	ObjType = OBJECT_TYPE_WARPPIPE;
}

void WarpPipe::Render()
{
	int Y = y + TileSize;

	for (int i = 0; i < (Height / TileSize) * 2; i++)
	{
		// vẽ phần đầu
		if (i < 2)
			animation_set->at(i)->Render(i * TileSize + x, y);
		// vẽ bên trái, i = 2, 4 , 6
		else if (i % 2 == 0)
		{
			animation_set->at(Ani_Body_Left)->Render(x, Y);
		}
		//vẽ bên phải
		else
		{
			animation_set->at(Ani_Body_Right)->Render(x + TileSize, Y);
			Y += TileSize;
		}
	}

	RenderBoundingBox();
}
