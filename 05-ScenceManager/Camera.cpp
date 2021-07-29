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
	maxBottomCam = maxLeftCam = maxRightCam = maxTopCam =
		cam_x = cam_y = SpeedOfPush = y_mario_fly = y_first_onground = 0.0f;
	ShakeTime = typeMove = 0;
	this->width = SCREEN_WIDTH;
	this->height = SCREEN_HEIGHT;
	Shake = test = false;
	real_height = _Game->GetScreenHeight() - VIEN_TRANG_HEIGHT - HUD_HEIGHT;
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
//	if (typeMove == CAMERA_MODE_RIGHT)
//	{
//		if (cam_x < 0)
//			cam_x = 0; // ép khoảng trái ngoài cùng để k lộ màu vàng khi mới bắt đầu
//
//		if (cam_x <= maxRightCam) // đẩy camera qua phải
//		{
//			cam_x += float(SpeedOfPush);
//		}
//		else
//			cam_x = maxRightCam;
//
//		if (cx < cam_x) // mario bị đẩy tới khi ở ngoài cùng bên trái
//		{
//			_Mario->x = cam_x;
//		}
//		else // chặn đầu không cho mario đi tiếp khi chạm ngoài cùng bên phải
//		{
//			int mariowidth = _Mario->Width;
//			float a = float(cx + mariowidth);
//			float b = float(cam_x + this->width) - 40;
//			if (a > b && _Mario->vx > 0) // mario đi tới thì chặn, đi về lại bên trái thì nhả ra
//			{
//				_Mario->x = b - mariowidth;
//			}
//		}
//
//		cx = cam_x;
//	}
//	else if (typeMove == CAMERA_MODE_NORMAL)
//	{
//		cx -= width / 2;
//		if (cx <= maxLeftCam)
//		{
//			cx = maxLeftCam;
//		}
//		else if (cx >= maxRightCam)
//		{
//			cx = maxRightCam;
//		}
//	}
//
//	cam_y = cy;
//	//148 : chiều cao chính xác của màn hình
//   // 160 -> 368 : 208 = 222 - cái viền : 14 - 60 : hud
//	float a = cam_y + real_height;
//	if (_Mario->canFlyX == true || _Mario->canFlyS == true)
//	{
//		// ô cuối: 26 , ô mục tiêu: 18 , chênh nhau 8 ô
//		if (cam_y - DISTANCE < maxTopCam)
//		{
//			a = maxTopCam;
//			if (_Mario->y < maxTopCam + DISTANCE)
//			{
//				_Mario->y = maxTopCam + DISTANCE;
//			}
//		}
//		else
//		{
//			if (cam_y < y_mario_fly) // đến ngưỡng camera bay theo
//			{
//				a = cam_y - DISTANCE; // mario cách đỉnh màn hình 1 khoàng
//			}
//			else // vẫn chưa đến ngưỡng camera bay theo
//				a = maxBottomCam - real_height;
//		}
//	}
//	else
//	{
//		if (cam_y >= maxTopCam && cam_y < (maxTopCam + real_height) / 2)
//			// mario vừa bay xong và bắt đầu rớt xuống, nên nó nằm trong khoảng từ maxtop -> maxtop + chiều rộng màn hình
//			// camera bắt đầu đi xuống khi mario vượt khỏi (maxTopCam + h)/2
//		{
//			a = 0;
//		}
//		else if (cam_y >= (maxTopCam + real_height) / 2 && cam_y + ((maxTopCam + real_height) / 2) < maxBottomCam && _Mario->hasFly == true)
//			// khi mario vượt qua (maxTopCam + h)/2 thì khoảng (maxTopCam + h)/2 chính là khoảng cố định tiếp theo khi mario rớt xuống
//			// ép thêm dk hasFly để tránh TH mario nhảy lên thì kéo camera lên
//		{
//			a = cam_y - ((maxTopCam + real_height) / 2);
//		}
//		else
//		{
//			// ép giới hạn bên dưới, k cho lòi phần màu vàng
//			a = maxBottomCam - real_height;
//		}
//	}
//	cy = a;
//	if (Shake == false)
//		SetCamPos(cx, cy);
//	else
//	{
//		ShakeTime++;
//		if (ShakeTime < 5)
//			SetCamPos(cx, cy - 10);
//		else if (ShakeTime < 10)
//			SetCamPos(cx, cy + 10);
//		else if (ShakeTime < 15)
//			SetCamPos(cx, cy - 10);
//		else if (ShakeTime < 20)
//			SetCamPos(cx, cy + 10);
//		else
//			Shake = false;
//	}
//
//
//	//DebugOut(L"X %f, Y %f\n", _Mario->x + _Mario->Width, _Mario->y);
//}

void Camera::Update()
{
	float cx, cy;
	_Mario->GetPosition(cx, cy);

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

		cx = cam_x;
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
	}

	cam_y = cy;
	//148 : chiều cao chính xác của màn hình
    //160 -> 368 : 208 = 222 - cái viền : 14 - 60 : hud
	// camera vẽ từ góc trái trên cùng vẽ xuống
	// 1. nếu setcam theo cy gốc thì camera sẽ vẽ từ đỉnh đầu mario xuống
	// -> do đó a phải trừ thêm real_height để camera dịch lên trên( chiều cao màn hình để vẽ camera đúng nhất, k hiện phần màu vàng)
	// 2. mario khi tới 1 mức y nào đó thì quy định maxBottomCam
	// 3. bắt buộc phải chia thành 2 TH : mario bay và k bay, vì maxbottom(216) < mức y camera phải follow mario khi bay ( tầm 280 -> 300 )
	float bot = cy + _Mario->Height ;
	float top = cy;
	if (_Mario->canFlyX == true || _Mario->canFlyS == true)
	{
		//DebugOut(L"canFly\t");
		
		if (top < maxTopCam)
		{
			//DebugOut(L"111111111111111\t");
			cy = maxTopCam;
			if (_Mario->y < maxTopCam)
			{
				_Mario->y = maxTopCam;
			}
		}
		else
		{
			//DebugOut(L"22222222222222\t");
			if (top < 307) // đến ngưỡng camera bay theo
			{
				cy = top - DISTANCE; // mario cách đỉnh màn hình 1 khoàng
				// tính toán lại khúc này, sao cho từ 258 -> mượt mượt thay vì 236 làm màn hình bị giật lên
				// 288 -> 287 - 50 = 237 >< 258
				if (cy < maxTopCam)
				{
					cy = maxTopCam;
				}

			}
			else // vẫn chưa đến ngưỡng camera bay theo
				cy = maxBottomCam;
		}
	}
	else
	{
		//DebugOut(L"Normal\t");
		//DebugOut(L"cy_before = %f\n", cy);
		if ((cy < 0 ) || ( cy >= maxTopCam && cy < (maxTopCam + real_height) / 2))
			// mario vừa bay xong và bắt đầu rớt xuống, nên nó nằm trong khoảng từ maxtop -> maxtop + chiều rộng màn hình
			// camera bắt đầu đi xuống khi mario vượt khỏi (maxTopCam + h)/2
		{
			cy = 0;
		}
		else
		{
			float a1 = (maxTopCam + real_height) / 2;
			float a2 = cy + ((maxTopCam + real_height) / 2);
			//DebugOut(L"a1 = %f, a2 = %f, cy = %f\t", a1, a2, cy);
			if (cy >= a1 && a2 < maxBottomCam)
				// khi mario vượt qua (maxTopCam + h)/2 thì khoảng (maxTopCam + h)/2 chính là khoảng cố định tiếp theo khi mario rớt xuống
				// ép thêm dk hasFly để tránh TH mario nhảy lên thì kéo camera lên
			{
				//DebugOut(L"1111111111111\n");
				cy = cy - ((maxTopCam + real_height) / 2);
			}
			else
			{
				//DebugOut(L"222222222222222\n");
				// ép giới hạn bên dưới, k cho lòi phần màu vàng
				cy = maxBottomCam;
			}
			//if (bot >= maxBottomCam) // ép bottom để k hiện ra phần vàng
			//	cy = maxBottomCam;
			//else
			//	// mario nhảy lên hoặc bay lên vượt qua maxBottomCam
			//{
			//	cy = maxBottomCam;
			//}
		}
	}
	
	
	//DebugOut(L"a = %f, cy = %f\n", bot, cy);
	//cy = bot;
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


	//DebugOut(L"X %f, Y %f\n", _Mario->x + _Mario->Width, _Mario->y);
}

void Camera::SetCamScene(float l, float t, float r, float b)
{
	maxLeftCam = l;
	maxTopCam = t;
	maxRightCam = r;
	maxBottomCam = b - real_height;
	y_mario_fly = b - 8 * 16;
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
