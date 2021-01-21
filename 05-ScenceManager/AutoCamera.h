#pragma once
#include "Game.h"

class AutoCamera
{
public:
	AutoCamera();
	~AutoCamera() {};
	void Update(DWORD dt);
	void Render();
	void HandleKeyBoard();
};