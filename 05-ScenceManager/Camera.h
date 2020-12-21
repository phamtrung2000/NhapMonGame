#pragma once
#include "Game.h"

class Camera
{
private:
	static Camera* __instance;
	int CySceen;
public:
	float cam_x; // cam_x
	float cam_y; // cam_y
	int width; // width screen
	int height; // height screen
	Camera();
	~Camera();

	void SetCamPos(float x, float y) { this->cam_x = x; this->cam_y = y; }
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }
	void SetCy(int y) { CySceen = y; }
	void SetRect(int width, int height) { width = width; height = height; };
	RECT GetBound(); // get RECT cam

	int GetHeight() { return height; }
	int GetWidth() { return width; }
	void Update();
	void Update1();

	static Camera* GetInstance();

};

