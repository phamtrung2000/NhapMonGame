#include "Camera.h"
#include "Game.h"
#include "PlayScence.h"

Camera* Camera::__instance = NULL;

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
	if (cy < 0)
		cy = 0;
	if (_PlayScene->SceneID == SCENE_ID_HIDDENMAP_1_1)
	{
		SetCamPos(cx, 0.0f);
		_Game->SetCamPos(cx, 0.0f);
	}
	else
	{
		if ( ((_Mario->canFlyS == true || _Mario->canFlyX == true) && _Mario->OnGround == false ) || _Mario->y <= _Map->GetHeight() / 2)
		{
			SetCamPos(cx, cy);
			_Game->SetCamPos(cx, cy);
		}
		else
		{
			SetCamPos(cx, (SCREEN_HEIGHT * 3 / 4) + 40 );
			_Game->SetCamPos(cx, (SCREEN_HEIGHT * 3 / 4) + 40);
		}
	}


	//_Camera->SetCamPos((MapWidth - game->GetScreenWidth()) / 2, (MapHeight - game->GetScreenHeight()) / 4);

	//CGame::GetInstance()->SetCamPos((MapWidth - game->GetScreenWidth()) / 2, (MapHeight - game->GetScreenHeight()) / 4);
}

void Camera::Update1()
{
	
}


Camera::~Camera()
{
}
