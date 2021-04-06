#pragma once
#include "Cell.h"
#define LOOP(x, y, z) for (int x = y; x <= z; x++)
struct  Area
{
	int  TopCell, LeftCell, RightCell, BottomCell;
};
class Grid
{
private:
	static Grid* __instance;
public:
	static Grid* GetInstance();
	Grid();
	~Grid() {};
	//RECT rect;
	int rows, cols;

	int SizeCell;
	vector<vector<Cell*>> Cells;
	vector<LPGAMEOBJECT> CurObjectInViewPort;
	Area GetCell(RECT e);
	void Init();
	void SetSizeCell(int s) { this->SizeCell = s; }
	void LoadObjects(LPGAMEOBJECT& obj, int Left, int Top, int Right, int Bottom);
	void AddStaticObject(LPGAMEOBJECT obj, float x, float y);
	void AddMovingObject(LPGAMEOBJECT obj, float x, float y);
	void AddStaticObjectByFile(LPGAMEOBJECT obj, int Left, int Top, int Right, int Bottom);
	void AddMovingObjectByFile(LPGAMEOBJECT obj, int Left, int Top, int Right, int Bottom);
	void UpdateCellInViewPort();
	void RenderCell();
	void RemoveDeadObject();

	void CalcObjectInViewPort();
	vector<LPGAMEOBJECT> GetObjectInViewPort() { return CurObjectInViewPort; }
};

