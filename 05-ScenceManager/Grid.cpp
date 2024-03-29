#include "Grid.h"
#include "Mario.h"
#include "Camera.h"
#include "Effects.h"
#include <unordered_set>

Grid* Grid::__instance = NULL;

Grid::Grid()
{
	rows = cols = SizeCell = 0;
}

Area Grid::GetCell(RECT e)
{
	// top left right bottom
	return {
		int(max(0		, e.top / SizeCell)),
		int(max(0		, e.left / SizeCell)),
		int(min(cols - 1, int(e.right / SizeCell))),
		int(min(rows - 1, int(e.bottom / SizeCell)))
	};
}

void Grid::Init()
{
	Cells.clear();
	for (int y = 0; y < rows; ++y)
	{
		auto row = std::vector<Cell*>();
		for (int x = 0; x < cols; ++x)
		{
			row.push_back(new Cell(x, y));
		}
		Cells.push_back(row);
	}
	DebugOut(L"Cell %d %d\n", rows, cols);
}
void Grid::AddMovingObject(LPGAMEOBJECT obj, float x, float y)
{
	RECT e;
	e.top = (LONG)y;
	e.left = (LONG)x;
	e.right = (LONG)(x + obj->Width);
	e.bottom = (LONG)(y + obj->Height);
	obj->GetRect();
	auto area = GetCell(e);
	LOOP(r, area.TopCell, area.BottomCell)
	{
		LOOP(c, area.LeftCell, area.RightCell)
		{
			Cells[r][c]->movingObjects.insert(obj);
			DebugOut(L"[INFO] object Moving TYPE: %d is add in Cell [%d] [%d]\n", obj->ObjType, r, c);
		}
	}
	obj->SetPosition(x, y);
	obj->SetStartPosition(x, y);
}

void Grid::AddMovingObjectByFile(LPGAMEOBJECT obj, int Left, int Top, int Right, int Bottom)
{
	for (int i = Top ; i <= Bottom ; i++)
	{
		for (int j = Left; j <= Right; j++)
		{
			int type = obj->ObjType;
			Cells[i][j]->movingObjects.insert(obj);
		}
	}
}

void Grid::AddStaticObject(LPGAMEOBJECT obj, float x, float y)
{
	RECT e;
	e.top = (LONG)y;
	e.left = (LONG)x;
	e.right = (LONG)(x + obj->Width);
	e.bottom = (LONG)(y + obj->Height);
	auto area = GetCell(e);
	LOOP(r, area.TopCell, area.BottomCell) 
	{
		LOOP(c, area.LeftCell, area.RightCell)
		{
			DebugOut(L"[INFO] object Static TYPE: %d is add in Cell [%d] [%d]\n", obj->ObjType, r, c);
			Cells[r][c]->staticObjects.insert(obj);
		}
	}
	obj->SetPosition(x, y);
	obj->SetStartPosition(x, y);
}

void Grid::AddStaticObjectByFile(LPGAMEOBJECT obj, int Left, int Top, int Right, int Bottom)
{
	for (int i = Top; i <= Bottom; i++)
	{
		for (int j = Left; j <= Right; j++)
		{
			int type = obj->ObjType;
			Cells[i][j]->staticObjects.insert(obj);
		}
	}
}



void Grid::RenderCell()
{
	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);
	auto area = GetCell(_Camera->GetBound());

	RECT rect;

	for (int r = area.TopCell; r <= area.BottomCell; r++)
	{
		for (int c = area.LeftCell; c <= area.RightCell; c++)
		{
			SetRect(&rect,
				Cells[r][c]->posX * SizeCell,
				Cells[r][c]->posY * SizeCell,
				Cells[r][c]->posX * SizeCell + SizeCell,
				Cells[r][c]->posY * SizeCell + SizeCell);
			CGame::GetInstance()->Draw((float)rect.left, (float)rect.top, bbox, rect.left, rect.top, rect.left + SizeCell, rect.top + 1, 255);
			CGame::GetInstance()->Draw((float)rect.left, (float)rect.top, bbox, rect.left, rect.top, rect.left + 1, rect.top + SizeCell, 255);

		}
	}
}


void Grid::CalcObjectInViewPort()
{
	auto area = GetCell(_Camera->GetBound());
	unordered_set<CGameObject*> result;

	for (int r = area.TopCell; r <= area.BottomCell; r++)
	{
		//DebugOut(L"[info] Cell row [%d]\n", r);
		for (int c = area.LeftCell ; c <= area.RightCell; c++)
		{
			result.insert(Cells[r][c]->movingObjects.begin(), Cells[r][c]->movingObjects.end());
			result.insert(Cells[r][c]->staticObjects.begin(), Cells[r][c]->staticObjects.end());
		/*	DebugOut(L"[info] Object in Cell  [%d][%d]: %d\n", r, c, Cells[r][c]->staticObjects.size());
			DebugOut(L"[info] Object move in Cell  [%d][%d]: %d\n", r, c, Cells[r][c]->movingObjects.size());
			DebugOut(L"[info] Cell column [%d]\n",c);
			DebugOut(L"\n");*/
		}
	}
	CurObjectInViewPort = { result.begin(), result.end() };
	//DebugOut(L"[info] Cell left: [%d], top: [%d], right: [%d] bottom: [%d]\n", area.LeftCell, area.TopCell, area.RightCell, area.BottomCell);
	//DebugOut(L"[info] Object in viewport: %d\n", CurObjectInViewPort.size());
}

template<typename T, typename Pred>
void RemoveObjectIf(unordered_set<T>& container, Pred  del)
{
	for (auto it = container.begin(), end = container.end(); it != end; )
	{
		if (del(*it))
			it = container.erase(it);
		else                       
			it++;
	}
}

void Grid::UpdateCellInViewPort()
{
	auto area = GetCell(_Camera->GetBound());
	unordered_set<LPGAMEOBJECT> shouldBeUpdatedObjects;
	bool isDeadObject = false;
	LOOP(r, area.TopCell, area.BottomCell)
		LOOP(c, area.LeftCell, area.RightCell)
	{
		if (Cells[r][c]->movingObjects.size() == 0) continue;
		//DebugOut(L"Obj in cell [%d] %d\n", c, Cells[r][c]->movingObjects.size());
		RemoveObjectIf(Cells[r][c]->movingObjects, [&](auto& obj)
			{
				RECT e;
				e.left = (LONG)obj->x;
				e.top = (LONG)obj->y;
				e.right = (LONG)(obj->x + obj->Width);
				e.bottom = (LONG)(obj->y + obj->Height);
				auto objArea = GetCell(e);
				if (obj->canDelete == true)
					isDeadObject = true;
				if (objArea.TopCell != r || objArea.RightCell != c)
				{
					//DebugOut(L"obj in cell [%d]\n", c);
					shouldBeUpdatedObjects.emplace(obj);
					//obj->SetPosition(obj->StartX, obj->StartY);
					return true;
				}
				return false;
			});
	}
	
	LOOP(r, area.TopCell, area.BottomCell)
		LOOP(c, area.LeftCell, area.RightCell)
	{
		if (Cells[r][c]->staticObjects.size() == 0) continue;
		//DebugOut(L"Obj in cell [%d] %d\n", c, Cells[r][c]->movingObjects.size());
		RemoveObjectIf(Cells[r][c]->staticObjects, [&](auto& obj)
			{
				RECT e;
				e.left = (LONG)obj->x;
				e.top = (LONG)obj->y;
				e.right = (LONG)(obj->x + obj->Width);
				e.bottom = (LONG)(obj->y + obj->Height);
				auto objArea = GetCell(e);
				if (obj->canDelete && obj->ObjType != 7)
				{
					return true;
					//isDeadObject = true;
				}
				return false;
			});
	}
	for (auto& obj : shouldBeUpdatedObjects)
	{
		RECT e;
		e.left = (LONG)obj->x;
		e.top = (LONG)obj->y;
		e.right = (LONG)(obj->x + obj->Width);
		e.bottom = (LONG)(obj->y + obj->Height);
		auto objArea = GetCell(e);
		LOOP(r, objArea.TopCell, objArea.BottomCell)
			LOOP(c, objArea.LeftCell, objArea.RightCell)
		{
			if (obj->IsMovingObject == false)
				Cells[r][c]->staticObjects.emplace(obj);
			else
			{
				Cells[r][c]->movingObjects.emplace(obj);
				//obj->SetPosition(obj->StartX, obj->StartY);
			}
				
			
			//DebugOut(L"obj move to %d   \n",c);
		}
	}
	if (isDeadObject == true) 
		RemoveDeadObject();
	CalcObjectInViewPort();
}

void Grid::RemoveDeadObject()
{
	auto area = GetCell(_Camera->GetBound());
	LOOP(r, area.TopCell, area.BottomCell)
		LOOP(c, area.LeftCell, area.RightCell)
	{
		RemoveObjectIf(Cells[r][c]->movingObjects, [](auto obj) {return obj->canDelete; });
	}
}

void Grid::UnLoad()
{
	Cells.clear();
}

Grid* Grid::GetInstance()
{
	if (__instance == NULL)
		__instance = new Grid();
	return __instance;
}