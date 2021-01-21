#include "Camera.h"
#include "Game.h"
#include "PlayScence.h"
#include "Coin.h"

Camera* Camera::__instance = NULL;

Camera* Camera::GetInstance()
{
	if (__instance == NULL)
		__instance = new Camera();
	return __instance;
}

Camera::Camera()
{
	cam_x = cam_y = 0.f;
	typeMove = 0;
	this->width = SCREEN_WIDTH;
	this->height = SCREEN_HEIGHT;
	test = false;
}
RECT Camera::GetBound()
{
	RECT bound;

	bound.left = cam_x;
	bound.top = cam_y;
	bound.right = cam_x + width;;
	bound.bottom = cam_y + height;
	return bound;
}

void Camera::Update()
{
	float cx, cy;
	_Mario->GetPosition(cx, cy);
	float a = _Map->GetHeight(); // 176
	float b = _Game->GetScreenHeight(); // 242
	if (a < b)
	{
		cy = 0;
	}
	

	if (typeMove == 1) 
	{
		if (cam_x < 0)
			cam_x = 0;
		if (cam_x <= maxRightCam)
			//cam_x += _Map->GetWidth() / 2000;
			cam_x += 0.5f;
		
		if (cx < cam_x) // bị đẩy
		{
			_Mario->x = cam_x;
		}

		//_Game->SetCamPos(cx, cy);
	}
	else
	{
		cx -= _Game->GetScreenWidth() / 2;
		if (cx >= MAP_MAX_WIDTH - _Game->GetScreenWidth())
			cx = MAP_MAX_WIDTH - _Game->GetScreenWidth();
		// xóa khúc vàng ở sau lúc bắt đầu _Game
		if (cx < 0)
		{
			cx = 0;
		}
		if (cx > MAP_MAX_WIDTH)
			cx = MAP_MAX_WIDTH;

		if (a > b)
		{
			cy -= _Game->GetScreenHeight() / 2;
		}
		
		if (cy < 0)
			cy = 0;
		if (_PlayScene->SceneID == SCENE_ID_HIDDENMAP_1_1)
		{
			SetCamPos(cx, 0.0f);
			_Game->SetCamPos(cx, 0.0f);
		}
		else
		{
			if (((_Mario->canFlyS == true || _Mario->canFlyX == true) && _Mario->OnGround == false) || _Mario->y <= _Map->GetHeight() / 2)
			{
				SetCamPos(cx, cy);
			}
			else
			{
				//SetCamPos(cx, (SCREEN_HEIGHT * 3 / 4) + 40);
				SetCamPos(cx, cy + 10);
			}
		}

	}
}

void Camera::Update1()
{
	
}


Camera::~Camera()
{
}

void Camera::SetCamPos(float x, float y)
{
	this->cam_x = x; this->cam_y = y;
	_Game->SetCamPos(x, y);
}

void Camera::SetCamPos1(float x, float y, float z)
{
	this->cam_x = x; this->cam_y = y;
	_Game->SetCamPos(z, y);
}
