#include "Camera.h"
#include "Game.h"
#include "PlayScence.h"

Camera* Camera::__instance = NULL;

#define CAMERA_SCREEN_HIGHT	248
#define CAMERA_SCREEN_HIGHT_MAX	200

Camera* Camera::GetInstance()
{
	if (__instance == NULL)
		__instance = new Camera();
	return __instance;
}

Camera::Camera()
{
	CySceen = 0;
	cam_x = cam_y = 0.f;
	this->width = SCREEN_WIDTH;
	this->height = SCREEN_HEIGHT;
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
	//float cx, cy;
	//_Mario->GetPosition(cx, cy);
	//cx -= _Game->GetScreenWidth() / 2;
	//cy -= _Game->GetScreenHeight() / 2;
	//if (cx <= 0)
	//{
	//	cx = 0;
	//}
	///*else if (cx + width >= Map::GetInstance()->GetWidth())
	//{
	//	cx = Map::GetInstance()->GetWidth() - width;
	//}*/
	//
	///*if (cy <= CAMERA_SCREEN_HIGHT_MAX)
	//{
	//	cy -= SCREEN_HEIGHT / 2;
	//	if (cy <= 70)
	//		cy = 70;
	//}
	//else if (cy + height >= Map::GetInstance()->GetHeight())
	//cy = Map::GetInstance()->GetHeight() - height + 48;
	//else 
	//	cy = CySceen;*/
	//
	//SetCamPos(cx, cy);
	//_Game->SetCamPos(cx, cy);



	// Update camera to follow mario
	float cx, cy;
	_Mario->GetPosition(cx, cy);
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
	
	cy -= _Game->GetScreenHeight() / 2;
	
	/*if (cy >= 220)
		cy = 220;*/

	/*if (CPlayScene::SceneID == SCENE_ID_HIDDENMAP_1_1)
		CGame::GetInstance()->SetCamPos(cx, 0.0f);
	else
	{
		if (_Mario->isFlyingHigh == true || _Mario->y <= _Map->GetHeight() / 2)
		{
			SetCamPos(cx, cy);
			_Game->SetCamPos(cx, cy);
		}
		else
		{
			SetCamPos(cx, 220);
			CGame::GetInstance()->SetCamPos(cx, 220);
		}
			
	}*/
	
	if (_Mario->isFlyingHigh == true || _Mario->y <= _Map->GetHeight() / 2)
	{
		SetCamPos(cx, cy);
		_Game->SetCamPos(cx, cy);
	}
	else
	{
		SetCamPos(cx, 200);
		CGame::GetInstance()->SetCamPos(cx, 200);
	}

}


Camera::~Camera()
{
}
