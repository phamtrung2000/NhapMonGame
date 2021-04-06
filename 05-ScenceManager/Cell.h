#pragma once
#include "GameObject.h"
#include <unordered_set>
class Cell
{
	RECT boundingBox;
public:
	int posX, posY;
	unordered_set<LPGAMEOBJECT> movingObjects;
	unordered_set<LPGAMEOBJECT> staticObjects;
	unordered_set<LPGAMEOBJECT> hiddenObjects;
	Cell(int x, int y)
	{
		this->posX = x;
		this->posY = y;
		movingObjects.clear();
		staticObjects.clear();
	}
	RECT GetBbox() { return boundingBox; }
	~Cell() {}
};

