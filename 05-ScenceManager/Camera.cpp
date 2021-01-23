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
	
	
	if (typeMove == 1) 
	{
		float a = _Map->GetHeight(); // 176
		float b = _Game->GetScreenHeight(); // 242
		if (a < b)
		{
			cy = 10;
		}
		if (cam_x < 0)
			cam_x = 0;
		if (cam_x <= maxRightCam)
			//cam_x += _Map->GetWidth() / 2000;
			cam_x += 0.5f;
		else
			cam_x = maxRightCam;
		_Game->SetCamPos(cam_x, cy);
		//_Game->SetCamPos(cx, cy);
		if (cx < cam_x) // bị đẩy
		{
			_Mario->x = cam_x;
		}

	}
	else
	{
		float a = _Map->GetHeight(); // 192
		float b = _Game->GetScreenHeight(); // 222
		cx -= SCREEN_WIDTH / 2;
		if (a < b)
		{
			cy -= SCREEN_HEIGHT / 2;
			if (cx <= maxLeftCam)
			{
				cx = maxLeftCam; 
			}
			else if (cx >= maxRightCam)
			{
				cx = maxRightCam;
			}

			if (cy <= maxTopCam)
			{
				cy = maxTopCam; //
			}
			else if (cy >= maxBottomCam)
			{
				cy = maxBottomCam;
			}
			else
			{
				if (cy >= maxBottomCam - SCREEN_HEIGHT / 2)
					cy = maxBottomCam - SCREEN_HEIGHT / 2;
			}
			SetCamPos(cx, cy + 10);
		}
		else
		{
			cy -= SCREEN_HEIGHT / 2;
			if (cx <= maxLeftCam)
			{
				cx = maxLeftCam; //
			}
			else if (cx >= maxRightCam)
			{
				cx = maxRightCam;
			}

			if (cy <= maxTopCam)
			{
				cy = maxTopCam; //
			}
			else if (cy >= maxBottomCam)
			{
				cy = maxBottomCam;
			}
			else
			{
				float a = maxBottomCam - SCREEN_HEIGHT / 2 - 30;
				/*if (cy < maxBottomCam - SsCREEN_HEIGHT / 2 - 30)
					cy = maxBottomCam - SCREEN_HEIGHT / 2 - 30;*/
				float y1 = _Mario->y;
				float y2 = _Map->GetHeight() / 2;
				if (((_Mario->canFlyS == true || _Mario->canFlyX == true) && _Mario->OnGround == false))
				{
					
				}
				else if (y1 <= y2)
				{
					cy = cy + 40;
				}
				else
				{
					cy = maxBottomCam - SCREEN_HEIGHT / 2 - 30;
				}
			}
			SetCamPos(cx, cy);
		}
			

		

	}
	/*DebugOut(L"cx %f, cy %f\n", cx, cy);
	DebugOut(L"X %f, Y %f\n", _Mario->x + _Mario->Width, _Mario->y);*/
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
