#include "Camera.h"
#include "Game.h"
#include "PlayScence.h"
#include "Coin.h"
#include <math.h>

#define VIEN 14
#define HUD		60
#define DISTANCE 20 // khoảng cách giữa mario mà đỉnh màn hình khi bay lên tới đỉnh

float roundDec(float var) {
	var = round((var + std::numeric_limits<float>::epsilon()) * 1000) / 1000;

	return var;
}

Camera* Camera::__instance = NULL;

Camera* Camera::GetInstance()
{
	if (__instance == NULL)
		__instance = new Camera();
	return __instance;
}

Camera::Camera()
{
	maxBottomCam = maxLeftCam = maxRightCam = maxTopCam =
	cam_x = cam_y = SpeedOfPush =  0.0f;
	ShakeTime = typeMove = 0;
	this->width = SCREEN_WIDTH;
	this->height = SCREEN_HEIGHT;
	Shake = test = false;
}

RECT Camera::GetBound()
{
	RECT bound;

	bound.left = (long)cam_x;
	bound.top = (long)cam_y;
	bound.right = (long)cam_x + width;;
	bound.bottom = (long) cam_y + height;
	return bound;
}

//void Camera::Update()
//{
//	float cx, cy;
//	_Mario->GetPosition(cx, cy);
//	
//	if (typeMove == 1) 
//	{
//		float a = _Map->GetHeight(); // 176
//		float b = _Game->GetScreenHeight(); // 242
//		if (a < b)
//		{
//			cy = 10;
//		}
//		if (cam_x < 0)
//			cam_x = 0;
//		if (cam_x <= maxRightCam)
//			//cam_x += _Map->GetWidth() / 2000;
//			cam_x += 0.5f;
//		else
//			cam_x = maxRightCam;
//		//_Game->SetCamPos(cam_x, cy);
//		_Game->SetCamPos(cx, cy);
//		if (cx < cam_x) // bị đẩy
//		{
//			_Mario->x = cam_x;
//		}
//
//	}
//	else
//	{
//		float a = _Map->GetHeight(); // 192
//		float b = _Game->GetScreenHeight(); // 222
//		cx -= SCREEN_WIDTH / 2;
//		// map ẩn
//		if (a < b)
//		{
//			cy -= SCREEN_HEIGHT / 2;
//			if (cx <= maxLeftCam)
//			{
//				cx = maxLeftCam; 
//			}
//			else if (cx >= maxRightCam)
//			{
//				cx = maxRightCam;
//			}
//
//			if (cy <= maxTopCam)
//			{
//				cy = maxTopCam; //
//			}
//			else if (cy >= maxBottomCam)
//			{
//				cy = maxBottomCam;
//			}
//			else
//			{
//				if (cy >= maxBottomCam - SCREEN_HEIGHT / 2)
//					cy = maxBottomCam - SCREEN_HEIGHT / 2;
//			}
//			SetCamPos(cx, cy + 10);
//		} 
//
//		else
//		{
//			if (cy <= 200)
//			{
//				SetCamPos(cx, 50);
//			}
//			else
//			{
//				bool fall = false;
//				fall = _Mario->isFalling;
//				if (fall==true)
//				{
//					if (cy > maxBottomCam - 32 - 150)
//						cy = maxBottomCam - 32 - 150;
//					SetCamPos(cx, cy);
//				}
//				else
//				{
//					cy -= SCREEN_HEIGHT / 2;
//					if (cx <= maxLeftCam)
//					{
//						cx = maxLeftCam; //
//					}
//					else if (cx >= maxRightCam)
//					{
//						cx = maxRightCam;
//					}
//
//					if (cy <= maxTopCam)
//					{
//						cy = maxTopCam; //
//					}
//					else if (cy >= maxBottomCam)
//					{
//						cy = maxBottomCam;
//					}
//					else
//					{
//						float a = maxBottomCam - SCREEN_HEIGHT / 2 - 30;
//						/*if (cy < maxBottomCam - SsCREEN_HEIGHT / 2 - 30)
//							cy = maxBottomCam - SCREEN_HEIGHT / 2 - 30;*/
//						float y1 = _Mario->y;
//						float y2 = _Map->GetHeight() / 2;
//						if (((_Mario->canFlyS == true || _Mario->canFlyX == true) && _Mario->OnGround == false))
//						{
//
//						}
//						else if (y1 <= y2)
//						{
//							cy = cy + 40;
//						}
//						else
//						{
//							cy = maxBottomCam - SCREEN_HEIGHT / 2 - 30;
//						}
//					}
//					SetCamPos(cx, cy);
//				}
//			}
//			
//		}
//	}
//	DebugOut(L"cx %f, cy %f\n", cx, cy);
//	//DebugOut(L"X %f, Y %f\n", _Mario->x + _Mario->Width, _Mario->y);
//}

void Camera::Update()
{
	float cx, cy;
	_Mario->GetPosition(cx, cy);
	float temp = 0;
	if (typeMove == CAMERA_MODE_RIGHT)
	{
		if (cam_x < 0)
			cam_x = 0; // ép khoảng trái ngoài cùng để k lộ màu vàng khi mới bắt đầu

		if (cam_x <= maxRightCam) // đẩy camera qua phải
		{
			cam_x += float(SpeedOfPush);
		}
		else
			cam_x = maxRightCam;

		if (cx < cam_x) // mario bị đẩy tới khi ở ngoài cùng bên trái
		{
			_Mario->x = cam_x;
		}
		else // chặn đầu không cho mario đi tiếp khi chạm ngoài cùng bên phải
		{
			int mariowidth = _Mario->Width;
			float a = float(cx + mariowidth);
			float b = float(cam_x + this->width) - 40;
			if (a > b && _Mario->vx > 0) // mario đi tới thì chặn, đi về lại bên trái thì nhả ra
			{
				_Mario->x = b - mariowidth;
			}
		}

		cam_y = cy;
		float h = _Game->GetScreenHeight() - VIEN - HUD; //148 : chiều cao chính xác của màn hình
		// 160 -> 368 : 208 = 222 - cái viền : 14 - 60 : hud
		float a = cam_y + h;
		if (_Mario->canFlyX == true || _Mario->canFlyS == true)
		{
			// ô cuối: 26 , ô mục tiêu: 18 , chênh nhau 8 ô
			if (cam_y - DISTANCE < maxTopCam)
			{
				a = maxTopCam;
				if (_Mario->y < maxTopCam + DISTANCE)
				{
					_Mario->y = maxTopCam + DISTANCE;
				}
			}
			else
			{
				if (cam_y < 288) // đến ngưỡng camera bay theo
				{
					a = cam_y - DISTANCE; // mario cách đỉnh màn hình 1 khoàng
				}
				else // vẫn chưa đến ngưỡng camera bay theo
					a = maxBottomCam - h;
			}
		}
		else
		{
			if (cam_y >= maxTopCam && cam_y < (maxTopCam + h) / 2)
				// mario vừa bay xong và bắt đầu rớt xuống, nên nó nằm trong khoảng từ maxtop -> maxtop + chiều rộng màn hình
				// camera bắt đầu đi xuống khi mario vượt khỏi (maxTopCam + h)/2
			{
				a = 0;
			}
			else if (cam_y >= (maxTopCam + h) / 2 && cam_y + ((maxTopCam + h) / 2) < maxBottomCam && _Mario->hasFly == true)
				// khi mario vượt qua (maxTopCam + h)/2 thì khoảng (maxTopCam + h)/2 chính là khoảng cố định tiếp theo khi mario rớt xuống
				// ép thêm dk hasFly để tránh TH mario nhảy lên thì kéo camera lên
			{
				a = cam_y - ((maxTopCam + h) / 2);
			}
			else
			{
				// ép giới hạn bên dưới, k cho lòi phần màu vàng
				a = maxBottomCam - h;
			}
		}
		SetCamPos(cam_x, a);
	}
	else if(typeMove == CAMERA_MODE_NORMAL)
	{
		cx -= width / 2;
		if (cx <= maxLeftCam)
		{
			cx = maxLeftCam;
		}
		else if (cx >= maxRightCam)
		{
			cx = maxRightCam;
		}

		cam_y = cy;
		float h = _Game->GetScreenHeight() - VIEN - HUD; //148 : chiều cao chính xác của màn hình
		// 160 -> 368 : 208 = 222 - cái viền : 14 - 60 : hud
		float a = cam_y + h;
		if (_Mario->canFlyX == true || _Mario->canFlyS == true)
		{
			// ô cuối: 26 , ô mục tiêu: 18 , chênh nhau 8 ô
			if (cam_y - DISTANCE < maxTopCam)
			{
				a = maxTopCam;
				if (_Mario->y < maxTopCam + DISTANCE)
				{
					_Mario->y = maxTopCam + DISTANCE;
				}
			}
			else
			{
				if (cam_y < 288) // đến ngưỡng camera bay theo
				{
					a = cam_y - DISTANCE; // mario cách đỉnh màn hình 1 khoàng
				}
				else // vẫn chưa đến ngưỡng camera bay theo
					a = maxBottomCam - h;
			}
		}
		else
		{
			if (cam_y >= maxTopCam && cam_y < (maxTopCam + h) / 2)
				// mario vừa bay xong và bắt đầu rớt xuống, nên nó nằm trong khoảng từ maxtop -> maxtop + chiều rộng màn hình
				// camera bắt đầu đi xuống khi mario vượt khỏi (maxTopCam + h)/2
			{
				a = 0;
			}
			else if (cam_y >= (maxTopCam + h) / 2 && cam_y + ((maxTopCam + h) / 2) < maxBottomCam && _Mario->hasFly == true)
				// khi mario vượt qua (maxTopCam + h)/2 thì khoảng (maxTopCam + h)/2 chính là khoảng cố định tiếp theo khi mario rớt xuống
				// ép thêm dk hasFly để tránh TH mario nhảy lên thì kéo camera lên
			{
				a = cam_y - ((maxTopCam + h) / 2);
			}
			else
			{
				// ép giới hạn bên dưới, k cho lòi phần màu vàng
				a = maxBottomCam - h;
			}
		}
		cy = a;
		if (Shake == false)
			SetCamPos(cx, cy);
		else
		{
			ShakeTime++;
			if (ShakeTime < 5)
				SetCamPos(cx, cy - 10);
			else if (ShakeTime < 10)
				SetCamPos(cx, cy + 10);
			else if (ShakeTime < 15)
				SetCamPos(cx, cy - 10);
			else if (ShakeTime < 20)
				SetCamPos(cx, cy + 10);
			else
				Shake = false;
		}
	}
	//DebugOut(L"X %f, Y %f\n", _Mario->x + _Mario->Width, _Mario->y);
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
