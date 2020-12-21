#pragma once
#include "GameObject.h"
class Ground :
	public CGameObject
{
public:
	int Width;
	int Height;
	int BehindGate; // nằm sau cổng số mấy -> xóa obj sau khi cổng đó hoàn thành
	Ground(int W, int H)
	{
		Width = W;
		Height = H;
		ObjType = 7;
		BehindGate = 0;
		Category = CATEGORY::GROUND;
	}
	Ground(int W, int H,int behindgate)
	{
		Width = W;
		Height = H;
		ObjType = 7;
		BehindGate = behindgate;
		Category = CATEGORY::GROUND;
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};

