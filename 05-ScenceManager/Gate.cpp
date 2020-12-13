#include "Gate.h"
#include "World1.h"

Gate::Gate(int a)
{
	ObjType = OBJECT_TYPE_GATE;
	GateNumber = a;
	isComplete = false;
}

void Gate::Render()
{
	RenderBoundingBox();
}
