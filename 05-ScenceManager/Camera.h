#pragma once
#include "Game.h"

#define CAMERA_MODE_NORMAL	0 // chế độ bình thường, đi theo mario
#define CAMERA_MODE_RIGHT	1 // chế độ tự động đẩy về phía trước, trái -> phải


#define VIEN_TRANG_HEIGHT 14
#define HUD_HEIGHT		50
#define DISTANCE 50 // khoảng cách giữa mario mà đỉnh màn hình khi bay lên tới đỉnh

class Camera
{
private:
	static Camera* __instance;
public:
	float cam_x; // cam_x
	float cam_y; // cam_y
	int width; // width screen
	int height; // height screen
	int typeMove; // 1: right; -1: left; 2: bot; -2: top; 0: follow player
	float maxTopCam;
	float maxLeftCam;
	float maxRightCam;
	float maxBottomCam;
	bool Shake; // rung lắc màn hình khi đạp nút P
	int ShakeTime; // thời gian rung lắc màn hình
	float SpeedOfPush; // tốc độ đẩy, mỗi stage có 1 tốc độ đẩy khác nhau
	int real_height; // chiều cao thật sự của màn hình game chứ k phải chiều cao màn hình app
	float y_mario_fly; // mức y mà camera sẽ follow mario khi mario bay lên
	float y_first_onground; // lấy y đầu tiên khi mario đang bay mà tiếp đất để tránh 
	Camera();
	~Camera();
	bool test;
	//void SetCamPos(float x, float y) { this->cam_x = x; this->cam_y = y; }
	void SetCamPos(float x, float y);
	void SetCamPos1(float x, float y,float z);
	
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }
	void SetCamMove(int type) {typeMove = type;}
	
	void SetRect(int width, int height) { width = width; height = height; };
	RECT GetBound(); // get RECT cam

	int GetHeight() { return height; }
	int GetWidth() { return width; }

	void Update();
	void SetCamScene(float l, float t, float r, float b);
	
	void SetSpeedOfPush(float s) { SpeedOfPush = (float)s; }


	static Camera* GetInstance();

};

